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
    std::vector<std::vector<SX>> Jacobianshape(const std::vector<SX>& gamma, Parm* parm);
    std::vector<SX> Jacobianellipsoid(const std::vector<SX>& gamma, body* Body);
    std::vector<SX> Jacobiancylinder(const std::vector<SX>& gamma, body* Body);
    std::vector<SX> Jacobiantorus(const std::vector<SX>& gamma, body* Body); // new torus

    std::vector<SX> constraint_shape(const std::vector<SX>& gamma, Parm* parm);
    
    SX constraint_ellipsoid(const std::vector<SX>& gamma, body* Body);
    SX constraint_cylinder(const std::vector<SX>& gamma, body* Body);
    SX constraint_torus(const std::vector<SX>& gamma, body* Body); // new torus
};
#endif // SHAPE_CONSTRAINT_H test
