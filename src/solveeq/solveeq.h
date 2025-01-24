/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef SOLVEEQ_H
#define SOLVEEQ_H

#include "../constraint/constraint.h"
#include "../objective/objective.h"
#include "../initial_guess/initialguess.h"
#include "../parm/parm.h"
#include "ipopt.h"

class solveeq {
public:
    solveeq();
    ~solveeq();
    IPOPT* getipopt();
    void setipoptoption(double tolvalue,int max_itervalue,const std::string& linear_solvervalue,int print_levelvalue,const std::string& hessian_approximationvalue);
    void setstepnum(int stepnumvalue);
    int getstepnum();
    objective* getObjective();
    initialguess* getInitialguess();
    void solvesignorinirotate(Parm* parm, int initialstart=0);
    void solvesignorini(Parm* parm);
    

private:
    IPOPT* ipopt=nullptr;
    constraint* Constraint=nullptr;
    objective* Objective=nullptr;
    initialguess* Initialguess=nullptr;
    int stepnum;
};

#endif // SOLVEEQ_H