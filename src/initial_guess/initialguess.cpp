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

int initialguess::getcollision_check(){
    return collision_check;
}

int initialguess::getmode_nr(){
    return mode_number;
}

void initialguess::setselect_bodyname(std::string bodyname){
    select_bodyname=bodyname;
}

bool initialguess::check_have_collision(const std::vector<double>& node, Parm* parm){
    std::vector<body*> allbody=parm->getallbody();
    for(int k=1; k<allbody.size(); k++){
        if(allbody[k]->phi_shape_current(node)<0){
            return true;
        }
    }
    return false;
}
    
std::string initialguess::getselect_bodyname(){
    return select_bodyname;
}

void initialguess::setpartition(Parm* parm){
    std::vector<body*> allbody=parm->getallbody();
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    body_partition={};
    for(int i=0; i<allmuscle.size();i++){
        std::vector<body*> singlemusclepartition={allmuscle[i]->getrhoo_body()};
        for(int j=0; j<allmuscle[i]->getnodenum()-2; j++){
            if(mode_number==0){
                singlemusclepartition.push_back(allbody[0]);
            }
            if(mode_number==1){
                singlemusclepartition.push_back(parm->findbody(select_bodyname));
            }
            if(mode_number==2 || mode_number==3){
                std::vector<double> gamma_value=allmuscle[i]->getgammaall().back();
                std::vector<double> node_pos={gamma_value[(j+1)*3],gamma_value[(j+1)*3+1],gamma_value[(j+1)*3+2]};
                double distance=allbody[1]->phi_shape_current(node_pos);
                int body_index=1;
                for(int k=2; k<allbody.size(); k++){
                    double distance1=allbody[k]->phi_shape_current(node_pos);
                    if(distance1<distance){
                        distance=distance1;
                        body_index=k;
                    }
                }
                singlemusclepartition.push_back(allbody[body_index]);
            }
        }
        singlemusclepartition.push_back(allmuscle[i]->getrhoi_body());
        body_partition.push_back(singlemusclepartition);
    }
    //print
    //print_partition(allmuscle);
}

void initialguess::setpartition_dynamic(Parm* parm){
    if(mode_number==3){
        std::vector<body*> allbody=parm->getallbody();
        std::vector<muscle*> allmuscle=parm->getallmuscle();
        body_partition={};
        for(int i=0; i<allmuscle.size();i++){
            std::vector<body*> singlemusclepartition={allmuscle[i]->getrhoo_body()};
            for(int j=0; j<allmuscle[i]->getnodenum()-2; j++){
                std::vector<double> gamma_value=allmuscle[i]->getgammaall().back();
                std::vector<double> node_pos={gamma_value[(j+1)*3],gamma_value[(j+1)*3+1],gamma_value[(j+1)*3+2]};
                double distance=allbody[1]->phi_shape_current(node_pos);
                int body_index=1;
                for(int k=2; k<allbody.size(); k++){
                    double distance1=allbody[k]->phi_shape_current(node_pos);
                    if(distance1<distance){
                        distance=distance1;
                        body_index=k;
                    }
                }
                singlemusclepartition.push_back(allbody[body_index]);                
            }
            singlemusclepartition.push_back(allmuscle[i]->getrhoi_body());
            body_partition.push_back(singlemusclepartition);
        }
        //print
        //print_partition(allmuscle);
    }
}

void initialguess::set_initialguessvalue(Parm* parm){
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    initialguessvalue={};
    for(int i=0; i<allmuscle.size();i++){
        std::vector<double> x0={};
        std::vector<std::vector<double>> etaall_muscle = allmuscle[i]->getetaall();
        if(mode_number==0){
            std::vector<std::vector<double>> gammaall_muscle = allmuscle[i]->getgammaall();
            x0.insert(x0.end(), gammaall_muscle.back().begin(), gammaall_muscle.back().end());
        }
        if(mode_number==1 || mode_number==2 || mode_number==3){
            for(int j=0; j<allmuscle[i]->getnodenum(); j++){
                std::vector<std::vector<double>> gammaall_muscle = allmuscle[i]->getgammaall();
                std::vector<std::vector<double>> etaall_muscle = allmuscle[i]->getetaall();
                std::vector<double> gamma_value=gammaall_muscle.back();
                std::vector<double> node_pos={gamma_value[j*3],gamma_value[j*3+1],gamma_value[j*3+2]};
                if(j==0 || j==allmuscle[i]->getnodenum()-1){
                    x0.insert(x0.end(), node_pos.begin(), node_pos.end());
                } else{
                    std::vector<std::vector<double>> all_q=body_partition[i][j]->getbodybasic()->getq();
                    std::vector<double>old_q=all_q[all_q.size()-2];

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
                    std::vector<double> vector_local_diff=globaltolocal(positionold, axisold, node_pos);
                    std::vector<double> node_initial_guess=localtoglobal(body_partition[i][j]->getbodybasic()->getposition(),body_partition[i][j]->getbodybasic()->getaxis(), vector_local_diff);
                    if(collision_check){
                        if(check_have_collision(node_initial_guess, parm)){
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
        }
        x0.insert(x0.end(), etaall_muscle.back().begin(), etaall_muscle.back().end());
        initialguessvalue.push_back(x0);
    }

}

std::vector<std::vector<double>> initialguess::get_initialguessvalue(){
    return initialguessvalue;
}

std::vector<double> initialguess::get_initialguessvalueindex(int index){
    return initialguessvalue[index];
}

void initialguess::resetforrecalc(){
    initialguessvalue={};
    body_partition={};
}

void initialguess::print_partition(const std::vector<muscle*>& allmuscle){
    for(int i=0; i<allmuscle.size();i++){
        std::cout<<"new muscle"<<std::endl;
        std::cout<<allmuscle[i]->getname()<<std::endl;
        for(int j=0; j<allmuscle[i]->getnodenum(); j++){
            std::cout<<body_partition[i][j]->getname()<<"\t";
        }
        std::cout<<"\n";
    }
}



