/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "initialguess.h"

using namespace std;

initialguess::initialguess(int mode_nr, int collision_check_num){
    mode_number=mode_nr;
    collision_check=collision_check_num;
}
    
initialguess::~initialguess(){

}

void initialguess::setmode_nr(int number){
    mode_number=number;
}

void initialguess::setcollision_check(int collision_check_num){
    collision_check=collision_check_num;
}

void initialguess::setselect_bodyname(std::string bodyname){
    select_bodyname=bodyname;
}

int initialguess::getcollision_check(){
    return collision_check;
}

int initialguess::getmode_nr(){
    return mode_number;
}

std::string initialguess::getselect_bodyname(){
    return select_bodyname;
}

std::vector<std::vector<double>> initialguess::get_initialguessvalue(){
    return initialguessvalue;
}

std::vector<double> initialguess::get_initialguessvalueindex(int index){
    if(index<0){
        index=initialguessvalue.size()+index;
    }
    return initialguessvalue[index];
}

bool initialguess::check_have_collision(const std::vector<double>& node, Parm* parm, int muscle_num, int node_num){
    std::vector<body*> allbody=parm->getallbody();
    for(int k=1; k<allbody.size(); k++){
        double phi_value = allbody[k]->phi_shape_current(node);
        if(phi_value<-1e-5){
            if(g_enable_print){std::cout<<"node has penetration for initial guess local parametrization -> muscle name: "<<parm->getallmuscle()[muscle_num]->getname()<<" node number: "<< node_num << " body name: " << allbody[k]->getname()<< " penetration: " <<phi_value<<std::endl;}
            return true;
        }
    }
    return false;
}

void initialguess::set_initialguessvalue(Parm* parm, int first_step){
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    initialguessvalue={};
    for(int i=0; i<allmuscle.size();i++){
        std::vector<double> x0={};
        std::vector<double> eta_last_muscle = allmuscle[i]->geteta_step(-1);
        //if gamma value is given, we will directly use it as initial guess, not make any corresponding rotation for the first step for mode num = 0
        if (mode_number==0  &&  first_step) {
            if(allmuscle[i]->get_read_muscle_value()>1){
                std::vector<double> gamma_value = allmuscle[i]->getgamma_step(-1);
                x0.insert(x0.end(), gamma_value.begin(), gamma_value.end());
            } else{
                std::vector<node*> all_nodes=allmuscle[i]->get_allnodes();
                std::vector<double> gamma_init_muscle={};
                std::vector<double> gamma_o = localtoglobal(all_nodes[0]->get_ref_body(0)->getbodybasic()->getposition(), all_nodes[0]->get_ref_body(0)->getbodybasic()->getaxis(), all_nodes[0]->get_rho());
                std::vector<double> gamma_i = localtoglobal(all_nodes.back()->get_ref_body(0)->getbodybasic()->getposition(), all_nodes.back()->get_ref_body(0)->getbodybasic()->getaxis(), all_nodes.back()->get_rho());
                std::vector<std::vector<double>> gamma=allmuscle[i]->interpolation(gamma_o, gamma_i, allmuscle[i]->getnodenum());
                for(int j=0; j<all_nodes.size(); j++){
                    gamma_init_muscle.push_back(gamma[j][0]);
                    gamma_init_muscle.push_back(gamma[j][1]);
                    gamma_init_muscle.push_back(gamma[j][2]);
                }
                x0.insert(x0.end(), gamma_init_muscle.begin(), gamma_init_muscle.end());
            }
        } else {
            std::vector<node*> all_nodes=allmuscle[i]->get_allnodes();
            for(int j=0; j<all_nodes.size(); j++){
                std::vector<double> node_initial_guess=all_nodes[j]->get_new_initial_guess(mode_number);
                if(collision_check && mode_number!=0 && mode_number!=-1){
                    if(check_have_collision(node_initial_guess, parm, i, j)){
                        std::vector<double> node_pos=all_nodes[j]->get_gamma_node(-1);
                        x0.insert(x0.end(), node_pos.begin(), node_pos.end());
                    }
                    else{
                        x0.insert(x0.end(), node_initial_guess.begin(), node_initial_guess.end());
                    }
                }
                else{
                    x0.insert(x0.end(), node_initial_guess.begin(), node_initial_guess.end());
                }
            }
        }
        x0.insert(x0.end(), eta_last_muscle.begin(), eta_last_muscle.end());
        initialguessvalue.push_back(x0);
    }

}

void initialguess::resetforrecalc(){
    initialguessvalue={};
}

void initialguess::print_partition(const std::vector<muscle*>& allmuscle){
    for(int i=0; i<allmuscle.size();i++){
        std::cout<<"check initial guess muscle partition: "<<allmuscle[i]->getname()<<" node number: "<<allmuscle[i]->getnodenum()<<std::endl;
        std::vector<node*> all_nodes=allmuscle[i]->get_allnodes();
        for(int j=0; j<all_nodes.size(); j++){
            std::cout<<all_nodes[j]->get_ref_init_body(-1)->getname()<<"\t";
        }
        std::cout<<"\n";
    }
}





