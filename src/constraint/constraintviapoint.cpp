/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "constraintviapoint.h"

using namespace std;
using namespace casadi;

constraintviapoint::constraintviapoint() {
}

constraintviapoint::~constraintviapoint() {
}

std::vector<std::vector<MX>> constraintviapoint::Jacobianvia_auto(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int node_index){
    std::vector<viapoint*> via_points = Muscle->getvia_point_list();
    std::vector<std::vector<MX>> Jacobian_via_all;
    MX node_i = vertcat(gammaallnode[node_index]);

    for (int i = 0; i < via_points.size(); i++) {

        std::vector<double> gamma_viapoint = via_points[i]->get_gamma_node(-1);
        MX viapoint = DM(gamma_viapoint);

        // ===== soft-min over ALL nodes =====
        std::vector<MX> terms;

        std::vector<node*> allnodes = Muscle->get_allnodes();
        for(int j=0; j<allnodes.size(); j++){
            if(allnodes[j]->get_fixpoint()){
                continue;
            }
            MX node_j = vertcat(gammaallnode[j]);
            MX diff_j = node_j - viapoint;
            MX dist2_j = dot(diff_j, diff_j);

            terms.push_back(exp(-1 * via_points[i]->get_alpha_value() * dist2_j));
        }

        MX sum_exp = sum1(vertcat(terms));
        MX fx = via_points[i]->get_cutoff()*via_points[i]->get_cutoff()-1/via_points[i]->get_alpha_value()*log(sum_exp);

        // ===== 对 node_i 求导 =====
        MX grad = gradient(fx, node_i);

        std::vector<MX> grad_vec(3);
        for (int k = 0; k < 3; k++) {
            grad_vec[k] = grad(k);
        }

        Jacobian_via_all.push_back(grad_vec);
    }
    return Jacobian_via_all;
}

std::vector<std::vector<MX>> constraintviapoint::Jacobianvia(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int node_index){
    std::vector<viapoint*> via_points = Muscle->getvia_point_list();
    std::vector<std::vector<MX>> Jacobian_via_all;
    for(int i=0; i<via_points.size(); i++){
        std::vector<double> gamma_viapoint=via_points[i]->get_gamma_node(-1);
        MX all_distance_exp_sum=0;
        MX node_index_distance=1.0;
        std::vector<node*> allnodes = Muscle->get_allnodes();
        for(int j=0; j<allnodes.size(); j++){
            if(allnodes[j]->get_fixpoint()){
                continue;
            }
            std::vector<MX> gammaallnode_j=gammaallnode[j];
            MX distance=0;
            for(int k=0; k<gamma_viapoint.size(); k++){
                distance=distance+pow(gamma_viapoint[k]-gammaallnode_j[k], 2);
            }
            //distance=sqrt(distance);
            all_distance_exp_sum=all_distance_exp_sum+MX::exp(-1*via_points[i]->get_alpha_value()*distance);
            if(j==node_index){
                node_index_distance=distance;
            }
        }
        std::vector<MX> Jacobian_via_single_viapoint;
        for(int j=0; j<gamma_viapoint.size(); j++){
            Jacobian_via_single_viapoint.push_back(-2*MX::exp(-1*via_points[i]->get_alpha_value()*node_index_distance)*(gammaallnode[node_index][j]-gamma_viapoint[j])/all_distance_exp_sum);
        }
        Jacobian_via_all.push_back(Jacobian_via_single_viapoint);
    }
    return Jacobian_via_all;
}

std::vector<MX> constraintviapoint::Jacobianvia_time_eta(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int node_index, const std::vector<MX>& eta_via){
    std::vector<std::vector<MX>> Jacobian = Jacobianvia(Muscle, gammaallnode, node_index);
    std::vector<MX> Jacobianeta;
    for(int i=0; i<3; i++){
        MX Jacobian_via_time_eta_xi=0;
        for(int j=0; j<Jacobian.size(); j++){
            Jacobian_via_time_eta_xi = Jacobian_via_time_eta_xi + Jacobian[j][i] * eta_via[j];
        }
        Jacobianeta.push_back(Jacobian_via_time_eta_xi);
    }
    return Jacobianeta;
}

std::vector<MX> constraintviapoint::constraintvia_phi_noeta(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode){
    std::vector<viapoint*> via_points = Muscle->getvia_point_list();
    std::vector<MX> constraintvia_phi;
    for(int i=0; i<via_points.size(); i++){
        std::vector<double> gamma_viapoint=via_points[i]->get_gamma_node(-1);
        MX all_distance_exp_sum=0;
        std::vector<node*> allnodes = Muscle->get_allnodes();
        for(int j=0; j<allnodes.size(); j++){
            if(allnodes[j]->get_fixpoint()){
                continue;
            }
            std::vector<MX> gammaallnode_j=gammaallnode[j];
            MX distance=0;
            for(int k=0; k<gamma_viapoint.size(); k++){
                distance=distance+pow(gamma_viapoint[k]-gammaallnode_j[k], 2);
            }
            //distance=sqrt(distance);
            all_distance_exp_sum=all_distance_exp_sum+MX::exp(-1*via_points[i]->get_alpha_value()*distance);
        }
        MX D_v=(-1/via_points[i]->get_alpha_value())*MX::log(all_distance_exp_sum);
        constraintvia_phi.push_back(via_points[i]->get_cutoff()*via_points[i]->get_cutoff()-D_v);
    }
    return constraintvia_phi;
}

std::vector<MX> constraintviapoint::constraintvia_phi_eta(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, const std::vector<MX>& eta_via, int phi_eta_plus){
    std::vector<MX> constraintvia_phi_no_eta = constraintvia_phi_noeta(Muscle, gammaallnode);
    std::vector<MX> constraintvia_phi_eta;
    if(phi_eta_plus){
        MX plus_value=0.0;
        for(int i=0; i<constraintvia_phi_no_eta.size(); i++){
            plus_value=plus_value+constraintvia_phi_no_eta[i]*eta_via[i];
        }
        constraintvia_phi_eta.push_back(plus_value);

    }else{
        for(int i=0; i<constraintvia_phi_no_eta.size(); i++){
            constraintvia_phi_eta.push_back(constraintvia_phi_no_eta[i]*eta_via[i]);
        }
    }
    
    return constraintvia_phi_eta;
}