/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "ipopt.h"

using namespace std;

IPOPT::IPOPT() {
    SetInitialipopt();
    //PrintParameters();
}

IPOPT::~IPOPT() {
    // Destructor
}

void IPOPT::SetInitialipopt() {
    tol = 1e-8;
    max_iter = 2000;
    linear_solver = "mumps";
    print_level = 0;
    hessian_approximation = "limited-memory";
}

Dict IPOPT::getipoptparm(){
    Dict opts_dict=Dict();
    if(!tol){opts_dict["ipopt.tol"] = tol;}
    if(!max_iter){opts_dict["ipopt.max_iter"] = max_iter;}
    if(!linear_solver.empty()){opts_dict["ipopt.linear_solver"] = linear_solver;}
    opts_dict["ipopt.print_level"] = print_level;
    if(!hessian_approximation.empty()){opts_dict["ipopt.hessian_approximation"] = hessian_approximation;}
    return opts_dict;
}

void IPOPT::PrintParameters() {
    std::cout << "ipopt tol: "<< tol << std::endl;
    std::cout << "ipopt max_iter: "<< max_iter << std::endl;
    std::cout << "ipopt linear_solver: "<< linear_solver << std::endl;
    std::cout << "ipopt print_level: "<< print_level << std::endl;
    std::cout << "ipopt hessian_approximation: "<< hessian_approximation << std::endl;
}

void IPOPT::settol(double tolvalue){
    tol=tolvalue;
}
    
void IPOPT::setmax_iter(int max_itervalue){
    max_itervalue=max_itervalue;
}
    
void IPOPT::setlinear_solver(const std::string& linear_solvervalue){
    linear_solver=linear_solvervalue;
}

void IPOPT::setprint_level(int print_levelvalue){
    print_level=print_levelvalue;
}
    
void IPOPT::sethessian_approximation(const std::string& hessian_approximationvalue){
    hessian_approximation=hessian_approximationvalue;
}

double IPOPT::gettol(){
    return tol;
}

int IPOPT::getmax_iter(){
    return max_iter;
}

std::string IPOPT::getlinear_solver(){
    return linear_solver;
}

int IPOPT::getprint_level(){
    return print_level;
}

std::string IPOPT::gethessian_approximation(){
    return hessian_approximation;

}