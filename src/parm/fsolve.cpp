/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "fsolve.h"

using namespace std;

Fsolve::Fsolve() {
    SetInitialfsolve();
    //PrintParameters();
}

Fsolve::~Fsolve() {
    // Destructor
}

void Fsolve::SetInitialfsolve() {
    // fsolve
    tol = 1e-8;
    maxiter = 1000;
}

void Fsolve::PrintParameters() {
    std::cout << "fsolve tol: "<< tol << std::endl;
    std::cout << "fsolve maxiter: "<< maxiter << std::endl;
}

