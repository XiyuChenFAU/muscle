/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "solveeq.h"
// #include <omp.h> // new for parallelization

solveeq::solveeq(){
    ipopt=new IPOPT();
    Constraint=new constraint();
    Objective=new objective();
    Initialguess=new initialguess(0,0);
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

constraint* solveeq::getConstraint(){
    return Constraint;
}

objective* solveeq::getObjective(){
    return Objective;
}

initialguess* solveeq::getInitialguess(){
    return Initialguess;
}

void solveeq::solvesignorinirotate(Parm* parm){
    
    std::vector<std::vector<double>> jointnaxisall;
    for(int i=0;i<parm->getn_joints();i++){
        jointnaxisall.push_back(parm->getjointindex(i)->getabsolute_pos().back());
    }
    
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    int variablenumall=parm->getvariable();
    Constraint->set_dict_constraint_all({});
    if(all_muscle_together){
        //variable
        MX x = MX::sym("x", variablenumall);
        //minimum
        MX f = 0;
        int start_index_x=0;
        for(int i=0;i<parm->getn_muscles();i++){
            int variablenum=allmuscle[i]->getvariablenum(parm->getn_bodies());
            MX x_single= x(Slice(start_index_x, start_index_x + variablenum));
            f = f + Objective->getobjective(parm, x_single,jointnaxisall,i);
            //constraints.
            std::vector<MX> singlemuscleconstraint=Constraint->constraints(parm,x_single,i);
            start_index_x=start_index_x+variablenum;
        }
        std::vector<MX> allconstraint = Constraint->put_constraints_together();
        MX g = vertcat(allconstraint);
        //set ipopt nlp
        MXDict nlp = {{"x", x}, {"f", f}, {"g", g}};
        Dict opts_dict=ipopt->getipoptparm();
        Function solver = nlpsol("solver", "ipopt", nlp, opts_dict);
        std::map<std::string, DM> arg, res;
        // Set constraint limit
        std::vector<std::vector<double>> lower_upper_limit = Constraint->set_lower_upper_limit();
        arg["lbg"] = lower_upper_limit[0];
        arg["ubg"] = lower_upper_limit[1];
        std::vector<double> x0;

        for(int i=0;i<parm->getn_muscles();i++){
            std::vector<double> initial=Initialguess->get_initialguessvalueindex(i);
            x0.insert(x0.end(), initial.begin(), initial.end());
        }
        if(g_enable_print){
            std::cout<<"x0 size: "<<x0.size() <<std::endl;
        }

        arg["x0"] = x0;
        // Solve the NLP
        res = solver(arg);
        vector<double> solution;
        for (int i = 0; i < x0.size(); i++) {
            solution.push_back(static_cast<double>(res.at("x")(i)));
        }
        //add solution to each muscle
        int start_index=0;
        for(int i=0;i<parm->getn_muscles();i++){
            int variablenum=allmuscle[i]->getvariablenum(parm->getn_bodies());
            std::vector<double> singlemusclesolution(solution.begin()+start_index,solution.begin()+start_index+variablenum);
            allmuscle[i]->addmuscleparm(singlemusclesolution);
            start_index=start_index+variablenum;
        }
    }
    else{
        for(int i=0;i<parm->getn_muscles();i++){
            //variable
            int variablenum=allmuscle[i]->getvariablenum(parm->getn_bodies());
            MX x = MX::sym("x", variablenum);
            //minimum
            MX f = Objective->getobjective(parm, x,jointnaxisall,i);
            //constraints
            std::vector<MX> allconstraint=Constraint->constraints(parm,x,i);
            MX g = vertcat(allconstraint);
            //set ipopt nlp
            MXDict nlp = {{"x", x}, {"f", f}, {"g", g}};
            Dict opts_dict=ipopt->getipoptparm();
            Function solver = nlpsol("solver", "ipopt", nlp, opts_dict);
            std::map<std::string, DM> arg, res;
            // Set constraint limit
            std::vector<std::vector<double>> lower_upper_limit = Constraint->set_lower_upper_limit();
            arg["lbg"] = lower_upper_limit[0];
            arg["ubg"] = lower_upper_limit[1];
            std::vector<double> x0;

            std::vector<double> initial=Initialguess->get_initialguessvalueindex(i);
            x0.insert(x0.end(), initial.begin(), initial.end());
            if(g_enable_print){
                std::cout<<"x0 size: "<<x0.size() <<std::endl;
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
}

void solveeq::solvesignorinistep(Parm* parm, int stepnum){
    if(g_enable_print){parm->set_body_R_initial();}
    if(stepnum!=0){
        Initialguess->setpartition_dynamic(parm);
    }
    parm->rotatebodyupdate(stepnum);
    if(g_enable_print){parm->check_body_R();}
    if(stepnum==0){
        parm->setallmuscleinitialeta_gamma();
        Initialguess->setpartition(parm);
        Initialguess->set_initialguessvalue(parm, 1);

    }else{
        Initialguess->set_initialguessvalue(parm, 0);
    }
    solvesignorinirotate(parm);
}

int solveeq::get_all_muscle_together(){
    return all_muscle_together;
}

void solveeq::set_all_muscle_together(int value){
    all_muscle_together=value;
}

