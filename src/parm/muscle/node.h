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
    int get_viapoint();
    std::vector<std::vector<double>> get_gammaall_node();
    std::vector<std::vector<double>> get_etaall_node();
    std::vector<std::vector<double>> get_local_gammaall_node();
    std::vector<double> get_gamma_node();
    std::vector<double> get_eta_node();
    std::vector<double> get_local_gamma_node();
    std::vector<double> get_rho();
    void set_endpoint(int value);
    void set_viapoint(int value);
    void add_ref_body(body* ref_body);
    void add_global_gamma_node(const std::vector<double>& gammanew);
    void add_eta_node(const std::vector<double>& etanew);
    void add_localtoglobal_gamma_node(const std::vector<double>& gammanew);
    void resetforrecalc_node();

private:
    int endpoint=0;
    int viapoint=0;
    std::vector<body*> ref_body_list={};
    std::vector<std::vector<double>> gammaall_node;
    std::vector<std::vector<double>> etaall_node;
    std::vector<std::vector<double>> local_gammaall_node;
};

#endif // NODE_H
