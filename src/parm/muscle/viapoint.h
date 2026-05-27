/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef VIAPoint_H
#define VIAPoint_H

#include <vector>
#include <iostream>
#include "../body/body.h"
#include "../../math/math.h"

class viapoint {
public:
    viapoint(std::vector<double> gamma);
    viapoint(std::vector<double> gamma, body* ref_body);
    viapoint(std::vector<double> rho, int local, body* ref_body);
    ~viapoint();
    int get_fixpoint();
    std::vector<std::vector<double>> get_gammaall_node();
    std::vector<std::vector<double>> get_etaall_node();
    std::vector<double> get_gamma_node();
    std::vector<double> get_gamma_node(int node_step);
    std::vector<double> get_eta_node();
    std::vector<double> get_eta_node(int node_step);
    std::vector<double> get_rho();
    double get_cutoff();
    double get_alpha_value();
    int get_node_index();
    body* get_ref_body();

    void set_node_index(int index);
    void set_fixpoint(int value);
    void set_cutoff(double value);
    void set_alpha_value(double value);

    void add_eta_node(const std::vector<double>& etanew);
    void add_eta_node(int node_step, const std::vector<double>& etanew);
    void add_global_gamma_node(const std::vector<double>& gammanew);
    void add_localtoglobal_gamma_from_rho();
    void add_localtoglobal_gamma_node(const std::vector<double>& gammanew);
    void resetforrecalc_via_point_node();

    void delete_eta_all();

private:
    int fixpoint=0;
    int node_index=0;
    double cutoff=0.05;
    double alpha_value=100.0;
    body* ref_body_list=nullptr;
    std::vector<std::vector<double>> gammaall_node;
    std::vector<std::vector<double>> etaall_node;
    std::vector<double> rho_initial;
};

#endif // VIAPoint_H
