/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "solveeq.h"

solveeq::solveeq(){
    ipopt=new IPOPT();
    Constraint=new constraint();
    setproblemtosolve("", {0.0,0.0,0.0}, 0.0, 0);
    Objective=new objective();
}
    
solveeq::~solveeq(){
    delete ipopt;
    delete Constraint;
    delete Objective;
}

IPOPT* solveeq::getipopt(){
    return ipopt;
}

std::vector<double> solveeq::getrotationmodel(){
    return rotationmodel;
}

void solveeq::setipoptoption(double tolvalue,int max_itervalue,const std::string& linear_solvervalue,int print_levelvalue,const std::string& hessian_approximationvalue){
    ipopt->settol(tolvalue);
    ipopt->setmax_iter(max_itervalue);
    ipopt->setlinear_solver(linear_solvervalue);
    ipopt->setprint_level(print_levelvalue);
    ipopt->sethessian_approximation(hessian_approximationvalue);
}

void solveeq::setproblemtosolve(std::string rotatebodyvalue, std::vector<double> naxisvalue, double rotationanglevalue, double stepnumvalue){
    rotatebody=rotatebodyvalue;
    naxis=naxisvalue;
    rotationangle=rotationanglevalue;
    stepnum=stepnumvalue;
    if(stepnum==0){
        rotationanglestep=0.0;
    }
    else{
        rotationanglestep=rotationangle/stepnum;
    }
}

std::string solveeq::getrotatebody(){
    return rotatebody;
}

std::vector<double> solveeq::getnaxis(){
    return naxis;
}
    
double solveeq::getrotationangle(){
    return rotationangle;
}
    
double solveeq::getrotationanglestep(){
    return rotationanglestep;
}

int solveeq::getstepnum(){
    return stepnum;
}

objective* solveeq::getObjective(){
    return Objective;
}

void solveeq::solvesignorinirotate(Parm* parm){
    std::vector<std::vector<double>> jointnaxisall;
    jointnaxisall.push_back(parm->findbody(rotatebody)->getbodybasic()->getposition());
    //variable
    int variablenum=parm->getvariable();
    SX x = SX::sym("x", variablenum);
    //minimum
    SX f = Objective->getobjective(parm, x,jointnaxisall);
    //constraints
    std::vector<SX> allconstraint=Constraint->constraints(parm,x);
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
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    for(int i=0;i<parm->getn_muscles();i++){
        std::vector<std::vector<double>> muscleparmall=allmuscle[i]->getmuscleparm();
        x0.insert(x0.end(), muscleparmall[muscleparmall.size()-1].begin(), muscleparmall[muscleparmall.size()-1].end());
    }
    arg["x0"] = x0;
    // Solve the NLP
    res = solver(arg);
    vector<double> solution;
    for (int i = 0; i < x0.size(); i++) {
        solution.push_back(static_cast<double>(res.at("x")(i)));
    }
    parm->addmuslcesolution(solution);
}

void solveeq::solvesignorini(Parm* parm){
    int loopnum = stepnum+1;
    std::vector<double> rotation;
    for(int i=0;i<loopnum;i++){
        rotation.push_back(rotationanglestep*i);
    }
    for(int i=0;i<loopnum;i++){
        if(i==0){
            parm->rotatebodyupdate(rotatebody, naxis, 0.0);
        }
        else{
            parm->rotatebodyupdate(rotatebody, naxis, rotationanglestep);
        }
        solvesignorinirotate(parm);
    }
    rotationmodel=rotation;
}

