/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef CONSTRAINTVIAPOINT_H
#define CONSTRAINTVIAPOINT_H

#include <iostream>
#include <vector>
#include <casadi/casadi.hpp>
#include "../parm/parm.h"

using namespace std;
using namespace casadi;

class constraintviapoint {
public:
    constraintviapoint();
    ~constraintviapoint();
    
    std::vector<std::vector<MX>> Jacobianvia_auto(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int node_index);
    std::vector<std::vector<MX>> Jacobianvia(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int node_index);
    std::vector<MX> Jacobianvia_time_eta(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int node_index, const std::vector<MX>& eta_via);
    std::vector<MX> constraintvia_phi_noeta(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode);
    std::vector<MX> constraintvia_phi_eta(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, const std::vector<MX>& eta_via, int phi_eta_plus);

};

#endif // CONSTRAINTVIAPOINT_H
