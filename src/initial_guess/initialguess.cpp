/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "initialguess.h"

using namespace std;

initialguess::initialguess(int mode_nr){
    mode_number=mode_nr;
}
    
initialguess::~initialguess(){

}

void initialguess::setmode_nr(int number){
    mode_number=number;
}

int initialguess::getmode_nr(){
    return mode_number;
}

void initialguess::setselect_bodyname(std::string bodyname){
    select_bodyname=bodyname;
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
            if(mode_number==2){
                std::vector<double> gamma_value=allmuscle[i]->getgammaall().back();
                std::vector<double> node_pos={gamma_value[(j+1)*3],gamma_value[(j+1)*3+1],gamma_value[(j+1)*3+2]};
                std::vector<double> dis_vector=vector3minus(node_pos, allbody[1]->getbodybasic()->getposition());
                double distance=vectortime1(dis_vector, dis_vector);
                int body_index=1;
                for(int k=2; k<allbody.size(); k++){
                    double distance1=vectortime1(vector3minus(node_pos, allbody[k]->getbodybasic()->getposition()), vector3minus(node_pos, allbody[k]->getbodybasic()->getposition()));
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
        if(mode_number==1 || mode_number==2){
            for(int j=0; j<allmuscle[i]->getnodenum(); j++){
                std::vector<std::vector<double>> gammaall_muscle = allmuscle[i]->getgammaall();
                std::vector<std::vector<double>> etaall_muscle = allmuscle[i]->getetaall();
                std::vector<double> gamma_value=gammaall_muscle.back();
                std::vector<double> node_pos={gamma_value[j*3],gamma_value[j*3+1],gamma_value[j*3+2]};
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
                x0.insert(x0.end(), node_initial_guess.begin(), node_initial_guess.end());
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



