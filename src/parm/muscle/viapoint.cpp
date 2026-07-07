/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "viapoint.h"

using namespace std;

viapoint::viapoint(std::vector<double> gamma){
    gammaall_node.push_back(gamma);
}

viapoint::viapoint(std::vector<double> gamma, body* ref_body){
    gammaall_node.push_back(gamma);
    ref_body_list=ref_body;
    fixpoint=1;
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
    rho_initial = globaltolocal(position, axis, gamma);
}

viapoint::viapoint(std::vector<double> rho, int local, body* ref_body){
    ref_body_list=ref_body;
    rho_initial=rho;
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
    fixpoint=1;
}

viapoint::~viapoint(){
    // Destructor
}

int viapoint::get_fixpoint(){
    return fixpoint;
}

std::vector<std::vector<double>> viapoint::get_gammaall_node(){
    return gammaall_node;
}

std::vector<std::vector<double>> viapoint::get_etaall_node(){
    return etaall_node;
}

// get initial setting rho in local coordinate of the reference body
std::vector<double> viapoint::get_rho(){
    return rho_initial;
}

double viapoint::get_cutoff(){
    return cutoff;
}

double viapoint::get_alpha_value(){
    return alpha_value;
}

std::vector<double> viapoint::get_gamma_node(){
    return gammaall_node.back();
}

std::vector<double> viapoint::get_gamma_node(int node_step){
    if(node_step<0){
        node_step=gammaall_node.size()+node_step;
    } 
    return gammaall_node[node_step];
}

std::vector<double> viapoint::get_eta_node(){
    return etaall_node.back();
}

std::vector<double> viapoint::get_eta_node(int node_step){
    if(node_step<0){
        node_step=etaall_node.size()+node_step;
    } 
    return etaall_node[node_step];
}

int viapoint::get_node_index(){
    return node_index;
}

body* viapoint::get_ref_body(){
    return ref_body_list;
}

void viapoint::set_node_index(int index){
    node_index=index;
}

void viapoint::set_fixpoint(int value){
    fixpoint=value;
}

void viapoint::set_cutoff(double value){
    cutoff=value;
}

void viapoint::set_alpha_value(double value){
    alpha_value=value;
}

void viapoint::add_eta_node(const std::vector<double>& etanew){
    etaall_node.push_back(etanew);
}

void viapoint::add_eta_node(int node_step, const std::vector<double>& etanew){
    if(etaall_node.size()>node_step){
        etaall_node[node_step]=etanew;
    } else{
        etaall_node.push_back(etanew);
    }
}

void viapoint::add_global_gamma_node(const std::vector<double>& gammanew){
    gammaall_node.push_back(gammanew);
}

void viapoint::add_localtoglobal_gamma_from_rho(){
    std::vector<double> position=ref_body_list->getbodybasic()->getposition();
    std::vector<std::vector<double>> axis=ref_body_list->getbodybasic()->getaxis();
    std::vector<double> global_gamma=localtoglobal(position, axis, rho_initial);
    gammaall_node.push_back(global_gamma);
}

void viapoint::add_localtoglobal_gamma_node(const std::vector<double>& gammanew){
    std::vector<double> position=ref_body_list->getbodybasic()->getposition();
    std::vector<std::vector<double>> axis=ref_body_list->getbodybasic()->getaxis();
    std::vector<double> global_gamma=localtoglobal(position, axis, gammanew);
    gammaall_node.push_back(global_gamma);
}

void viapoint::resetforrecalc_via_point_node(){
    if (gammaall_node.size() > 1) {
        gammaall_node.erase(gammaall_node.begin() + 1, gammaall_node.end());
        etaall_node.erase(etaall_node.begin() + 1, etaall_node.end());
    }
}

void viapoint::delete_eta_all(){
    etaall_node.clear();
}