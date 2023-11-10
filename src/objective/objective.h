/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include <iostream>
#include <vector>
#include <casadi/casadi.hpp>
#include "../parm/parm.h"

using namespace std;
using namespace casadi;


class objective {
public:
    objective();
    ~objective();
    void setcasenum(int casenumvalue);
    int getcasenum();
    SX getlengthdisdiff(Parm* parm, std::vector<std::vector<std::vector<SX>>> gammaallnode);
    SX getnodelengthdisdiffall(Parm* parm, SX x, std::vector<std::vector<double>> jointposition);

    std::vector<SX> getnodedisdiff(Parm* parm, std::vector<std::vector<std::vector<SX>>> gammaallnode);
    std::vector<double> getmassmatrix(Parm* parm, std::vector<std::vector<double>> jointposition);
    SX getobjectivemass(std::vector<SX> nodediff, std::vector<double> massmatrix);
    SX getnodedisdiffall(Parm* parm, SX x, std::vector<std::vector<double>> jointposition);
    std::vector<std::vector<std::vector<std::vector<SX>>>> rearrange_gamma_eta(Parm* parm, SX x);
    SX getobjective(Parm* parm, SX x, std::vector<std::vector<double>> jointposition);

private:
    int casenum=0;
    SX f=0;
};

#endif // OBJECTIVE_H
