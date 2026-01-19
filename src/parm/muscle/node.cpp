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
    ref_body_init_list.push_back(ref_body);
    fixpoint=1;
}

node::node(std::vector<double> gamma, body* ref_body, int endpoint_setting){
    gammaall_node.push_back(gamma);
    ref_body_list.push_back(ref_body);
    ref_body_init_list.push_back(ref_body);
    endpoint=endpoint_setting;
    fixpoint=1;
}

node::node(std::vector<double> rho, int local, body* ref_body){
    ref_body_list.push_back(ref_body);
    ref_body_init_list.push_back(ref_body);
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

node::node(std::vector<double> rho, int local, body* ref_body, int endpoint_setting){
    ref_body_list.push_back(ref_body);
    ref_body_init_list.push_back(ref_body);
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
    fixpoint=1;
}

node::~node(){
    // Destructor
}

int node::get_endpoint(){
    return endpoint;
}

int node::get_fixpoint(){
    return fixpoint;
}

std::vector<std::vector<double>> node::get_gammaall_node(){
    return gammaall_node;
}

std::vector<std::vector<double>> node::get_etaall_node(){
    return etaall_node;
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

std::vector<double> node::get_gamma_node(int node_step){
    if(node_step<0){
        node_step=gammaall_node.size()+node_step;
    } 
    return gammaall_node[node_step];
}

std::vector<double> node::get_eta_node(){
    return etaall_node.back();
}

std::vector<double> node::get_eta_node(int node_step){
    if(node_step<0){
        node_step=etaall_node.size()+node_step;
    } 
    return etaall_node[node_step];
}

int node::get_node_index(){
    return node_index;
}

body* node::get_ref_body(int index){
    if(index<0){
        index=ref_body_list.size()+index;
    } 
    return ref_body_list[index];
}

body* node::get_ref_init_body(int index){
    if(index<0){
        index=ref_body_init_list.size()+index;
    } 
    return ref_body_init_list[index];
}

std::vector<body*> node::get_ref_body(){
    return ref_body_list;
}

std::vector<body*> node::get_ref_init_body(){
    return ref_body_init_list;
}

void node::set_node_index(int index){
    node_index=index;
}

void node::set_endpoint(int value){
    endpoint=value;
}

void node::set_fixpoint(int value){
    fixpoint=value;
}

void node::add_ref_body(body* ref_body){
    ref_body_list.push_back(ref_body);
}

void node::add_ref_body_init(body* ref_body){
    ref_body_init_list.push_back(ref_body);
}

void node::add_eta_node(const std::vector<double>& etanew){
    etaall_node.push_back(etanew);
}

void node::add_eta_node(int node_step, const std::vector<double>& etanew){
    if(etaall_node.size()>node_step){
        etaall_node[node_step]=etanew;
    } else{
        etaall_node.push_back(etanew);
    }
}

void node::add_global_gamma_node(const std::vector<double>& gammanew){
    gammaall_node.push_back(gammanew);
}

void node::add_localtoglobal_gamma_node(const std::vector<double>& gammanew){
    body* ref_body=ref_body_list.back();
    std::vector<double> position=ref_body->getbodybasic()->getposition();
    std::vector<std::vector<double>> axis=ref_body->getbodybasic()->getaxis();
    std::vector<double> global_gamma=localtoglobal(position, axis, gammanew);
    gammaall_node.push_back(global_gamma);
}

std::vector<double> node::get_new_initial_guess(int mode_number){   
    std::vector<double>old_q=ref_body_init_list.back()->getbodybasic()->getq_step(-2);
    std::vector<double> positionold;
    std::vector<std::vector<double>> axisold;
    for(int i=0; i<3; i++){
        positionold.push_back(old_q[i]);
        std::vector<double> axisold1;
        for (int j = 3+i*3; j < 6+i*3 ; j++) {
            axisold1.push_back(old_q[j]);
        }
        axisold.push_back(axisold1);
    }
    std::vector<double> vector_local_diff=globaltolocal(positionold, axisold, gammaall_node.back());
    if(mode_number==4){
        return vector_local_diff; //constraint local parameterization
    } else{
        vector_local_diff=localtoglobal(ref_body_init_list.back()->getbodybasic()->getposition(),ref_body_init_list.back()->getbodybasic()->getaxis(), vector_local_diff);
    }
    return vector_local_diff;
}

void node::set_single_node_partition(const std::vector<body*>& allbody, int constraint_local_mode_number, body* constraint_local_select_body, int init_mode_nr, body* init_select_body, int first_step_index){
    // Set reference body according to partition settings
    int calculate_auto_local=0;
    if(first_step_index){
        if(constraint_local_mode_number>1 || init_mode_nr>1){
            calculate_auto_local=1;
        }
    }
    else{
        if(constraint_local_mode_number>2 || init_mode_nr>2){
            calculate_auto_local=1;
        }
    }
    if(fixpoint){
        if(!first_step_index){
            ref_body_list.push_back(ref_body_list.back());
            ref_body_init_list.push_back(ref_body_init_list.back());
        }
    }
    else{
        int body_index=1;
        if(calculate_auto_local){
            double distance=allbody[1]->phi_shape_current(gammaall_node.back());
            for(int k=2; k<allbody.size(); k++){
                double distance1=allbody[k]->phi_shape_current(gammaall_node.back());
                if(distance1<distance){
                    distance=distance1;
                    body_index=k;
                }
            }
        }
        body* constraint_ref_body = nullptr;
        if(constraint_local_mode_number==0){
            constraint_ref_body = allbody[0];
        } else if(constraint_local_mode_number==1){
            constraint_ref_body = constraint_local_select_body;
        } else if(constraint_local_mode_number==2){
            if(first_step_index){
                constraint_ref_body = allbody[body_index];
            } else{
                constraint_ref_body = ref_body_list.back();
            }
        } else{
            constraint_ref_body = allbody[body_index];
        }

        body* init_ref_body = nullptr;
        if(init_mode_nr==0){
            init_ref_body = allbody[0];
        } else if(init_mode_nr==1){
            init_ref_body = init_select_body;
        } else if(init_mode_nr==2){
            if(first_step_index){
                init_ref_body = allbody[body_index];
            } else{
                init_ref_body = ref_body_init_list.back();
            }
        } else if(init_mode_nr==3){
            init_ref_body = allbody[body_index];
        } else{// constraint local parameterization
            init_ref_body = constraint_ref_body;
        }

        if(first_step_index){
            if(!fixpoint){
                ref_body_list.push_back(constraint_ref_body);
                ref_body_init_list.push_back(init_ref_body);
            }
        } else{
            ref_body_list.push_back(constraint_ref_body);
            ref_body_init_list.push_back(init_ref_body);
        }
    }
}

void node::resetforrecalc_node(int save_first_step_value){
    if (gammaall_node.size() > 1) {
        gammaall_node.erase(gammaall_node.begin() + 1, gammaall_node.end());
        if(fixpoint){
            ref_body_list.erase(ref_body_list.begin() + 1, ref_body_list.end());
            ref_body_init_list.erase(ref_body_init_list.begin() + 1, ref_body_init_list.end());
        } else{
            if(save_first_step_value){
                etaall_node.erase(etaall_node.begin() + 1, etaall_node.end());
            } else{
                etaall_node.clear();
            }
            ref_body_list.clear();
            ref_body_init_list.clear();
        }
    }
}

void node::delete_eta_all(){
    etaall_node.clear();
}