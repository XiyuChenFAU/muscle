/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef IPOPT_H
#define IPOPT_H

#include <casadi/casadi.hpp>
#include <iostream>

using namespace casadi;
using namespace std;

class IPOPT {
public:
    IPOPT();
    ~IPOPT();
    void SetInitialipopt();
    Dict getipoptparm();
    void PrintParameters();
    void settol(double tolvalue);
    void setmax_iter(int max_itervalue);
    void setlinear_solver(const std::string& linear_solvervalue);
    void setprint_level(int print_levelvalue);
    void sethessian_approximation(const std::string& hessian_approximationvalue);
    double gettol();
    int getmax_iter();
    std::string getlinear_solver();
    int getprint_level();
    std::string gethessian_approximation();
    

private:
    double tol;
    int max_iter;
    std::string linear_solver;
    int print_level;
    std::string hessian_approximation;
};

#endif // IPOPT_H
