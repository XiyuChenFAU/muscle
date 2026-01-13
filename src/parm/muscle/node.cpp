/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "node.h"

using namespace std;

node::node(std::vector<double> gamma){
    gammaall_node.push_back(gamma);
}

node::node(std::vector<double> gamma, body* ref_body){
    gammaall_node.push_back(gamma);
    ref_body_list.push_back(ref_body);
    viapoint=1;
}

node::node(std::vector<double> gamma, body* ref_body, int endpoint_setting){
    gammaall_node.push_back(gamma);
    ref_body_list.push_back(ref_body);
    endpoint=endpoint_setting;
    viapoint=1;
}

node::node(std::vector<double> rho, int local, body* ref_body){
    ref_body_list.push_back(ref_body);
    std::vector<std::vector<double>> q_all=ref_body->getbodybasic()->getq();
    std::vector<double> q_current=q_all[0];
    std::vector<double> position;
    std::vector<std::vector<double>> axis;
    for(int i=0; i<3; i++){
        position.push_back(q_current[i]);
        std::vector<double> axis1;
        for (int j = 3+i*3; j < 6+i*3 ; j++) {
            axis1.push_back(q_current[j]);
        }
        axis.push_back(axis1);
    }
    std::vector<double> global_gamma=localtoglobal(position, axis, rho);
    gammaall_node.push_back(global_gamma);
    viapoint=1;
}

node::node(std::vector<double> rho, int local, body* ref_body, int endpoint_setting){
    ref_body_list.push_back(ref_body);
    std::vector<std::vector<double>> q_all=ref_body->getbodybasic()->getq();
    std::vector<double> q_current=q_all[0];
    std::vector<double> position;
    std::vector<std::vector<double>> axis;
    for(int i=0; i<3; i++){
        position.push_back(q_current[i]);
        std::vector<double> axis1;
        for (int j = 3+i*3; j < 6+i*3 ; j++) {
            axis1.push_back(q_current[j]);
        }
        axis.push_back(axis1);
    }
    std::vector<double> global_gamma=localtoglobal(position, axis, rho);
    gammaall_node.push_back(global_gamma);
    endpoint=endpoint_setting;
    viapoint=1;
}

node::~node(){
    // Destructor
}

int node::get_endpoint(){
    return endpoint;
}

int node::get_viapoint(){
    return viapoint;
}

std::vector<std::vector<double>> node::get_gammaall_node(){
    return gammaall_node;
}

std::vector<std::vector<double>> node::get_etaall_node(){
    return etaall_node;
}

std::vector<std::vector<double>> node::get_local_gammaall_node(){
    return local_gammaall_node;
}

std::vector<double> node::get_local_gamma_node(){
    return local_gammaall_node.back();
}

// get initial setting rho in local coordinate of the reference body
std::vector<double> node::get_rho(){
    body* ref_body=ref_body_list[0];
    std::vector<std::vector<double>> q_all=ref_body->getbodybasic()->getq();
    std::vector<double> q_current=q_all[0];
    std::vector<double> position;
    std::vector<std::vector<double>> axis;
    for(int i=0; i<3; i++){
        position.push_back(q_current[i]);
        std::vector<double> axis1;
        for (int j = 3+i*3; j < 6+i*3 ; j++) {
            axis1.push_back(q_current[j]);
        }
        axis.push_back(axis1);
    }
    return globaltolocal(position, axis, gammaall_node[0]);
}

std::vector<double> node::get_gamma_node(){
    return gammaall_node.back();
}

std::vector<double> node::get_eta_node(){
    return etaall_node.back();
}

void node::set_endpoint(int value){
    endpoint=value;
}

void node::set_viapoint(int value){
    viapoint=value;
}

void node::add_ref_body(body* ref_body){
    ref_body_list.push_back(ref_body);
}

void node::add_global_gamma_node(const std::vector<double>& gammanew){
    gammaall_node.push_back(gammanew);
}

void node::add_eta_node(const std::vector<double>& etanew){
    etaall_node.push_back(etanew);
}

void node::add_localtoglobal_gamma_node(const std::vector<double>& gammanew){
    local_gammaall_node.push_back(gammanew);
    body* ref_body=ref_body_list.back();
    std::vector<std::vector<double>> q_all=ref_body->getbodybasic()->getq();
    std::vector<double> q_current=q_all.back();
    std::vector<double> position;
    std::vector<std::vector<double>> axis;
    for(int i=0; i<3; i++){
        position.push_back(q_current[i]);
        std::vector<double> axis1;
        for (int j = 3+i*3; j < 6+i*3 ; j++) {
            axis1.push_back(q_current[j]);
        }
        axis.push_back(axis1);
    }
    std::vector<double> global_gamma=localtoglobal(position, axis, gammanew);
    gammaall_node.push_back(global_gamma);
}

void resetforrecalc_node(){
    if (gammaall_node.size() > 1) {
        gammaall_node.erase(gammaall_node.begin() + 1, gammaall_node.end());
        etaall_node.erase(etaall_node.begin() + 1, etaall_node.end());
        local_gammaall_node.erase(local_gammaall_node.begin() + 1, local_gammaall_node.end());
        ref_body_list.erase(ref_body_list.begin() + 1, ref_body_list.end());
    }
}