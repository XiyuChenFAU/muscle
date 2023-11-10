/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef SOLVEEQ_H
#define SOLVEEQ_H

#include "../constraint/constraint.h"
#include "../objective/objective.h"
#include "../parm/parm.h"
#include "ipopt.h"

class solveeq {
public:
    solveeq();
    ~solveeq();
    IPOPT* getipopt();
    std::vector<double> getrotationmodel();
    void setipoptoption(double tolvalue,int max_itervalue,const std::string& linear_solvervalue,int print_levelvalue,const std::string& hessian_approximationvalue);
    void setproblemtosolve(std::string rotatebodyvalue, std::vector<double> naxisvalue, double rotationanglevalue, double stepnumvalue);
    std::string getrotatebody();
    std::vector<double> getnaxis();
    double getrotationangle();
    double getrotationanglestep();
    int getstepnum();
    objective* getObjective();
    void solvesignorinirotate(Parm* parm);
    void solvesignorini(Parm* parm);
    

private:
    IPOPT* ipopt=nullptr;
    constraint* Constraint=nullptr;
    objective* Objective=nullptr;
    std::vector<double> rotationmodel;
    std::string rotatebody;
    std::vector<double> naxis;
    double rotationangle;
    double rotationanglestep;
    int stepnum;
};

#endif // SOLVEEQ_H