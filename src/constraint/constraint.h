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
    
    std::vector<MX> constraints_shape_noeta(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode);
    std::vector<MX> constraintsnoeq(Parm* parm, const std::vector<std::vector<std::vector<MX>>>& dataall, int musclenum);

    std::vector<MX> constraints_fix_muscle_two_side_point(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode);
    std::vector<MX> constraints_shape_eta(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, const std::vector<std::vector<MX>>& eta);
    std::vector<MX> constraints_Discrete_Euler_Lagrange_eachmuscle_eachnode(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int nodenum, const std::vector<std::vector<MX>>& eta);
    std::vector<MX> constraints_Discrete_Euler_Lagrange_eachmuscle(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, const std::vector<std::vector<MX>>& eta);
    std::vector<MX> constraintseq(Parm* parm, const std::vector<std::vector<std::vector<MX>>>& dataall, int musclenum);
    
    std::vector<std::vector<std::vector<MX>>> rearrange_gamma_eta(Parm* parm, MX x, int musclenum);
    std::vector<MX> constraints(Parm* parm, MX x, int musclenum);

    std::vector<MX> geodesic_function(const std::vector<MX>& q_nm1, const std::vector<MX>& q_n, const std::vector<MX>& q_np1, double K);
    std::vector<MX> Jacobian_time_eta(const std::vector<std::vector<MX>>& Jacobian, const std::vector<MX>& eta);
    std::vector<MX> constraintshape_time_eta(const std::vector<MX>& constraintshape, const std::vector<MX>& eta);


    void printvalue(const std::vector<std::vector<std::vector<MX>>>& gmmaall);
        
    constraintshape* getConstraintshape();
    int getconstrainteqnum();
    int getconstraintnoeqnum();
    std::vector<MX> getconstraintall();
    std::vector<double> getlowerlimitall();
    std::vector<double> getupperlimitall();

private:
    constraintshape* Constraintshape;
    int constrainteqnum;
    int constraintnoeqnum;
    std::vector<MX> constraintall;
    std::vector<double> lowerlimitall;
    std::vector<double> upperlimitall;
};

#endif // CONSTRAINT_H
