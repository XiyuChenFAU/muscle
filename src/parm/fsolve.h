/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef FSOLVE_H
#define FSOLVE_H

#include <iostream>

class Fsolve {
public:
    Fsolve();
    ~Fsolve();
    void SetInitialfsolve();
    void PrintParameters();

private:
    double tol;
    int maxiter;
};

#endif // FSOLVE_H
