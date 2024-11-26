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
    void setlengthconstant(double lengthconstantvalue);
    double getlengthconstant();
    int getcasenum();

    SX getlengthdisdiff(muscle* Muscle, const std::vector<std::vector<SX>>& gammaallnode);
    SX getnodelengthdisdiffall(Parm* parm, SX x, const std::vector<std::vector<double>>& jointposition, int musclenum);

    std::vector<SX> getnodedisdiff(muscle* Muscle, const std::vector<std::vector<SX>>& gammaallnode);
    std::vector<double> getmassmatrix(muscle* Muscle, const std::vector<std::vector<double>>& jointposition);
    SX getobjectivemass(const std::vector<SX>& nodediff, const std::vector<double>& massmatrix);
    SX getnodedisdiffall(Parm* parm, SX x, const std::vector<std::vector<double>>& jointposition, int musclenum);
    std::vector<std::vector<std::vector<SX>>> rearrange_gamma_eta(Parm* parm, SX x, int musclenum);
    SX getobjective(Parm* parm, SX x, const std::vector<std::vector<double>>& jointposition, int musclenum);

private:
    int casenum=0;
    double lengthconstant=1.0;
    SX f=0;
};

#endif // OBJECTIVE_H
