/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "solveeq.h"

solveeq::solveeq(){
    ipopt=new IPOPT();
    Constraint=new constraint();
    Objective=new objective();
    Initialguess=new initialguess(2,1);
}
    
solveeq::~solveeq(){
    delete ipopt;
    delete Constraint;
    delete Objective;
    delete Initialguess;
}

IPOPT* solveeq::getipopt(){
    return ipopt;
}

void solveeq::setipoptoption(double tolvalue,int max_itervalue,const std::string& linear_solvervalue,int print_levelvalue,const std::string& hessian_approximationvalue){
    ipopt->settol(tolvalue);
    ipopt->setmax_iter(max_itervalue);
    ipopt->setlinear_solver(linear_solvervalue);
    ipopt->setprint_level(print_levelvalue);
    ipopt->sethessian_approximation(hessian_approximationvalue);
}

objective* solveeq::getObjective(){
    return Objective;
}

initialguess* solveeq::getInitialguess(){
    return Initialguess;
}

void solveeq::solvesignorinirotate(Parm* parm, int initialstart){
    std::vector<std::vector<double>> jointnaxisall;
    for(int i=0;i<parm->getn_joints();i++){
        jointnaxisall.push_back(parm->getjointindex(i)->getabsolute_pos().back());
    }
    
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    int variablenumall=parm->getvariable();
    for(int i=0;i<parm->getn_muscles();i++){
        //variable
        int variablenum=allmuscle[i]->getvariablenum(parm->getn_bodies());
        SX x = SX::sym("x", variablenum);
        //minimum
        SX f = Objective->getobjective(parm, x,jointnaxisall,i);
        //constraints
        std::vector<SX> allconstraint=Constraint->constraints(parm,x,i);
        SX g = vertcat(allconstraint);
        //set ipopt nlp
        SXDict nlp = {{"x", x}, {"f", f}, {"g", g}};
        Dict opts_dict=ipopt->getipoptparm();
        Function solver = nlpsol("solver", "ipopt", nlp, opts_dict);
        std::map<std::string, DM> arg, res;
        // Set constraint limit
        arg["lbg"] = Constraint->getlowerlimitall();
        arg["ubg"] = Constraint->getupperlimitall();
        std::vector<double> x0;

        if(initialstart){
            std::vector<std::vector<double>> gammaall_muscle = allmuscle[i]->getgammaall();
            std::vector<std::vector<double>> etaall_muscle = allmuscle[i]->getetaall();
            x0.insert(x0.end(), gammaall_muscle.back().begin(), gammaall_muscle.back().end());
            x0.insert(x0.end(), etaall_muscle.back().begin(), etaall_muscle.back().end());
            allmuscle[i]->deletegammaalllast();
        }
        else{
            std::vector<double> initial=Initialguess->get_initialguessvalueindex(i);
            x0.insert(x0.end(), initial.begin(), initial.end());
        }
        arg["x0"] = x0;
        // Solve the NLP
        res = solver(arg);
        vector<double> solution;
        for (int i = 0; i < x0.size(); i++) {
            solution.push_back(static_cast<double>(res.at("x")(i)));
        }
        allmuscle[i]->addmuscleparm(solution);
    }

}

void solveeq::solvesignorini(Parm* parm){
    int loopnum = parm->get_run_total_step()+1;
    if(parm->get_run_total_step()==0){
        loopnum=0;
    }
    for(int i=0;i<loopnum;i++){
        parm->set_body_R_initial();
        if(i!=0){
            Initialguess->setpartition_dynamic(parm);
        }
        parm->rotatebodyupdate(i);
        //parm->check_body_R();
        if(i==0){
            if(!parm->get_read_muscle_value()){
                parm->setallmuscleinitialeta_gamma();
            }
            Initialguess->setpartition(parm);
            if(!parm->get_read_muscle_value()){
                solvesignorinirotate(parm,1);
            } else {
                Initialguess->set_initialguessvalue(parm);
                solvesignorinirotate(parm,0);
            }
        }else{
            Initialguess->set_initialguessvalue(parm);
            solvesignorinirotate(parm,0);
        }
    }
}

