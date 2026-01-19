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
    
    std::vector<MX> constraints_shape_noeta(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int use_p_variable, MX p_var);
    std::vector<MX> constraintsnoeq(Parm* parm, const std::vector<std::vector<std::vector<MX>>>& dataall, int musclenum, int use_p_variable, MX p_var);

    std::vector<MX> constraints_fix_muscle_two_side_point(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode);
    std::vector<MX> constraints_shape_eta(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, const std::vector<std::vector<MX>>& eta, int use_p_variable, MX p_var);
    std::vector<MX> constraints_Discrete_Euler_Lagrange_eachmuscle_eachnode(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int nodenum, const std::vector<std::vector<MX>>& eta, int use_p_variable, MX p_var);
    std::vector<MX> constraints_Discrete_Euler_Lagrange_eachmuscle(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, const std::vector<std::vector<MX>>& eta, int use_p_variable, MX p_var);
    std::vector<MX> constraintseq(Parm* parm, const std::vector<std::vector<std::vector<MX>>>& dataall, int musclenum, int use_p_variable, MX p_var);
    
    std::vector<std::vector<std::vector<MX>>> rearrange_gamma_eta(Parm* parm, MX x, int musclenum, int use_p_variable, MX p_var);
    std::vector<MX> constraints(Parm* parm, MX x, int musclenum, int use_p_variable, MX p_var);
    std::vector<MX> put_constraints_together();
    std::vector<std::vector<double>> set_lower_upper_limit();

    std::vector<MX> geodesic_function(const std::vector<MX>& q_nm1, const std::vector<MX>& q_n, const std::vector<MX>& q_np1, double K);
    std::vector<MX> Jacobian_time_eta(const std::vector<std::vector<MX>>& Jacobian, const std::vector<MX>& eta);
    std::vector<MX> constraintshape_time_eta(const std::vector<MX>& constraintshape, const std::vector<MX>& eta);
    void printvalue(const std::vector<std::vector<std::vector<MX>>>& gmmaall);
        
    void set_local_mode_number(int value);
    void set_local_select_bodyname(std::string value);
    void set_phi_eta_plus(int value);
    int get_local_mode_number();
    std::string get_local_select_bodyname();
    int get_phi_eta_plus();
    void set_dict_constraint_all(const std::map<int, std::map<std::string, std::map<std::string, std::vector<MX>>>>& dict);
    constraintshape* getConstraintshape();

private:
    constraintshape* Constraintshape;
    int phi_eta_plus = 0;
    std::map<std::string, std::map<std::string, std::vector<MX>>> dict_constraint;
    std::map<int, std::map<std::string, std::map<std::string, std::vector<MX>>>> dict_constraint_all;
    std::string local_select_bodyname="fix_space";
    int local_mode_number=0; //0 global, 1 local, 2 auto local initial, 3 auto local dynamic
};

#endif // CONSTRAINT_H
