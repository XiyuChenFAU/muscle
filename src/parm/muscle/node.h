/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>
#include "../body/body.h"
#include "../../math/math.h"

class node {
public:
    node(std::vector<double> gamma);
    node(std::vector<double> gamma, body* ref_body);
    node(std::vector<double> gamma, body* ref_body, int endpoint_setting);
    node(std::vector<double> rho, int local, body* ref_body);
    node(std::vector<double> rho, int local, body* ref_body, int endpoint_setting);
    ~node();
    int get_endpoint();
    int get_fixpoint();
    std::vector<std::vector<double>> get_gammaall_node();
    std::vector<std::vector<double>> get_etaall_node();
    std::vector<double> get_gamma_node();
    std::vector<double> get_gamma_node(int node_step);
    std::vector<double> get_eta_node();
    std::vector<double> get_eta_node(int node_step);
    std::vector<double> get_rho();
    int get_node_index();
    body* get_ref_body(int index);
    body* get_ref_init_body(int index);
    std::vector<body*> get_ref_body();
    std::vector<body*> get_ref_init_body();

    void set_node_index(int index);
    void set_endpoint(int value);
    void set_fixpoint(int value);
    void add_ref_body(body* ref_body);
    void add_ref_body_init(body* ref_body);
    
    void add_eta_node(const std::vector<double>& etanew);
    void add_eta_node(int node_step, const std::vector<double>& etanew);
    void add_global_gamma_node(const std::vector<double>& gammanew);
    void add_localtoglobal_gamma_node(const std::vector<double>& gammanew);
    void set_gamma_history(const std::vector<std::vector<double>>& gamma_history);
    void set_eta_history(const std::vector<std::vector<double>>& eta_history);

    std::vector<double> get_new_initial_guess(int mode_number);

    void set_single_node_partition(const std::vector<body*>& allbody, int constraint_local_mode_number, body* constraint_local_select_body, int init_mode_nr, body* init_select_body, int first_step_index);
    void resetforrecalc_node(int save_first_step_value);

    void delete_eta_all();

private:
    int endpoint=0;
    int fixpoint=0;
    int node_index=0;
    std::vector<body*> ref_body_list={};
    std::vector<body*> ref_body_init_list={};
    std::vector<std::vector<double>> gammaall_node;
    std::vector<std::vector<double>> etaall_node;
    std::vector<double> rho_initial={0.0, 0.0, 0.0};
};

#endif // NODE_H
