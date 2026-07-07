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

    MX getlengthdisdiff(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode);
    MX getlengthdisdiff(const std::vector<MX>& gammalaststep, const std::vector<std::vector<MX>>& gammaallnode);
    MX getnodelengthdisdiffall(Parm* parm, MX x, const std::vector<std::vector<double>>& jointposition, int musclenum, int local_mode_number, int use_p_variable, const std::vector<MX>& p_gamma_previous, const std::vector<MX>& p_mass_matrix);

    std::vector<MX> getnodedisdiff(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int local_mode_number);
    std::vector<MX> getnodedisdiff(muscle* Muscle, const std::vector<MX>& gammaprevious, const std::vector<std::vector<MX>>& gammaallnode, int local_mode_number);
    std::vector<double> getmassmatrix(muscle* Muscle, const std::vector<std::vector<double>>& jointposition);
    MX getobjectivemass(const std::vector<MX>& nodediff, const std::vector<double>& massmatrix);
    MX getobjectivemass(const std::vector<MX>& nodediff, const std::vector<MX>& massmatrix);
    MX getnodedisdiffall(Parm* parm, MX x, const std::vector<std::vector<double>>& jointposition, int musclenum, int local_mode_number, int use_p_variable, const std::vector<MX>& p_gamma_previous, const std::vector<MX>& p_mass_matrix);
    std::vector<std::vector<std::vector<MX>>> rearrange_gamma_eta(Parm* parm, MX x, int musclenum);

    std::vector<MX>  getnode_offset_dis(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int local_mode_number);
    std::vector<MX>  getnode_offset_dis(muscle* Muscle, const std::vector<MX>& gammaprevious, const std::vector<std::vector<MX>>& gammaallnode, int local_mode_number);
    MX getnode_offset_dis_all(Parm* parm, MX x, const std::vector<std::vector<double>>& jointposition, int musclenum, int local_mode_number, int use_p_variable, const std::vector<MX>& p_gamma_previous, const std::vector<MX>& p_mass_matrix);

    MX getobjective(Parm* parm, MX x, const std::vector<std::vector<double>>& jointposition, int musclenum, int local_mode_number,int use_p_variable, const std::vector<MX>& p_gamma_previous, const std::vector<MX>& p_mass_matrix);

private:
    int casenum=0;
    double lengthconstant=1.0;
    MX f=0;
};

#endif // OBJECTIVE_H
