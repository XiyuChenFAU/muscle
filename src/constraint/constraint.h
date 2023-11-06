/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "constraintshape.h"


class constraint {
public:
    constraint();
    ~constraint();
    std::vector<SX> constraints_fix_muscle_two_side_point(Parm* parm, muscle* Muscle, const std::vector<std::vector<SX>>& gammaallnode);
    std::vector<SX> constraintsnoeq(Parm* parm, const std::vector<std::vector<std::vector<std::vector<SX>>>>& dataall);
    std::vector<SX> constraints_shape_noeta(Parm* parm, const std::vector<std::vector<std::vector<SX>>>& gammaallnode);
    std::vector<SX> constraints_shape_eta(Parm* parm, const std::vector<std::vector<std::vector<SX>>>& gammaallnode, const std::vector<std::vector<std::vector<SX>>>& eta);
    std::vector<SX> constraints_Discrete_Euler_Lagrange_eachmuscle_eachnode(Parm* parm, int musclenum, const std::vector<std::vector<SX>>& gammaallnode, int nodenum, const std::vector<std::vector<SX>>& eta);
    std::vector<SX> constraints_Discrete_Euler_Lagrange_eachmuscle(Parm* parm, int musclenum, const std::vector<std::vector<SX>>& gammaallnode, const std::vector<std::vector<SX>>& eta);
    std::vector<SX> constraints_Discrete_Euler_Lagrange(Parm* parm, const std::vector<std::vector<std::vector<SX>>>& gammaallnode, const std::vector<std::vector<std::vector<SX>>>& eta);
    std::vector<std::vector<std::vector<std::vector<SX>>>> rearrange_gamma_eta(Parm* parm, SX x);
    std::vector<SX> constraintseq(Parm* parm, const std::vector<std::vector<std::vector<std::vector<SX>>>>& dataall);
    std::vector<SX> constraints(Parm* parm, SX x);
    std::vector<SX> geodesic_function(const std::vector<SX>& q_nm1, const std::vector<SX>& q_n, const std::vector<SX>& q_np1, double K);
    std::vector<SX> Jacobian_time_eta(const std::vector<std::vector<SX>>& Jacobian, const std::vector<SX>& eta);
    std::vector<SX> constraintshape_time_eta(const std::vector<SX>& constraintshape, const std::vector<SX>& eta);
    void printvalue(const std::vector<std::vector<std::vector<SX>>>& gmmaall);
        
    constraintshape* getConstraintshape();
    int getconstrainteqnum();
    int getconstraintnoeqnum();
    std::vector<SX> getconstraintall();
    std::vector<double> getlowerlimitall();
    std::vector<double> getupperlimitall();

private:
    constraintshape* Constraintshape;
    int constrainteqnum;
    int constraintnoeqnum;
    std::vector<SX> constraintall;
    std::vector<double> lowerlimitall;
    std::vector<double> upperlimitall;
};

#endif // CONSTRAINT_H
