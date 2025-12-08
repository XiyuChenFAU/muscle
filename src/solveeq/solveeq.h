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
    constraint* getConstraint();
    objective* getObjective();
    initialguess* getInitialguess();
    void solvesignorinirotate(Parm* parm);
    void solvesignorinistep(Parm* parm, int stepnum);
    int get_all_muscle_together();
    void set_all_muscle_together(int value);
    

private:
    IPOPT* ipopt=nullptr;
    constraint* Constraint=nullptr;
    objective* Objective=nullptr;
    initialguess* Initialguess=nullptr;
    int all_muscle_together=0;
};

#endif // SOLVEEQ_H