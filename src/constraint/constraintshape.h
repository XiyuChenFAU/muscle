/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef SHAPE_CONSTRAINT_H
#define SHAPE_CONSTRAINT_H

#include <iostream>
#include <vector>
#include <casadi/casadi.hpp>
#include "../parm/parm.h"

using namespace std;
using namespace casadi;

class constraintshape {
public:
    constraintshape();
    std::vector<std::vector<MX>> Jacobianshape(const std::vector<MX>& gamma, Parm* parm);
    std::vector<MX> Jacobianellipsoid(const std::vector<MX>& gamma, body* Body);
    std::vector<MX> Jacobiancylinder(const std::vector<MX>& gamma, body* Body);
    std::vector<MX> Jacobiantorus(const std::vector<MX>& gamma, body* Body); // new torus

    std::vector<MX> constraint_shape(const std::vector<MX>& gamma, Parm* parm);
    
    MX constraint_ellipsoid(const std::vector<MX>& gamma, body* Body);
    MX constraint_cylinder(const std::vector<MX>& gamma, body* Body);
    MX constraint_torus(const std::vector<MX>& gamma, body* Body); // new torus
};
#endif // SHAPE_CONSTRAINT_H test
