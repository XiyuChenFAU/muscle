/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "constraint.h"

using namespace std;
using namespace casadi;

constraint::constraint(){
    Constraintshape=new constraintshape();
    constrainteqnum=0;
    constraintnoeqnum=0;
}

constraint::~constraint(){
    delete Constraintshape;
}

std::vector<SX> constraint::constraints_fix_muscle_two_side_point(Parm* parm, muscle* Muscle, const std::vector<std::vector<SX>>& gammaallnode) {
    std::vector<SX> musclefix;
    std::vector<double> gammarhoo=Muscle->getrho_o_position();
    std::vector<double> gammarhoi=Muscle->getrho_i_position();
    for(int j=0;j<3;j++){
        SX muscleo=gammarhoo[j] - gammaallnode[0][j];
        musclefix.push_back(muscleo);
    }
    for(int j=0;j<3;j++){
        SX musclei=gammarhoi[j] - gammaallnode[gammaallnode.size()-1][j];
        musclefix.push_back(musclei);
    }
    return musclefix;
}

std::vector<SX> constraint::constraints_shape_noeta(Parm* parm, const std::vector<std::vector<std::vector<SX>>>& gammaallnode){
    std::vector<muscle*> Muscle=parm->getallmuscle();
    std::vector<SX> constraintshape;
    for(int k=0; k<parm->getn_muscles();k++){
        for(int i=0; i<Muscle[k]->getnodenum()-2;i++){
            std::vector<SX> constraintshape1=Constraintshape->constraint_shape(gammaallnode[k][i+1], parm);
            for(int j=0;j<constraintshape1.size();j++){
                constraintshape.push_back(constraintshape1[j]);
            }
        }
    }
    return constraintshape;
}

std::vector<SX> constraint::constraintsnoeq(Parm* parm, const std::vector<std::vector<std::vector<std::vector<SX>>>>& dataall){
    std::vector<std::vector<std::vector<SX>>> gammaallnode=dataall[0];
    std::vector<std::vector<std::vector<SX>>> etaall=dataall[1];
    std::vector<SX> constraint_noeq;
    std::vector<SX> constraintsshapenoeta=constraints_shape_noeta(parm, gammaallnode);
    constraint_noeq.insert(constraint_noeq.end(), constraintsshapenoeta.begin(), constraintsshapenoeta.end());
    for(int i=0; i<etaall.size();i++){
        for(int j=0; j<etaall[i].size();j++){
            for(int k=0; k<etaall[i][j].size();k++){
                constraint_noeq.push_back(etaall[i][j][k]);
            }
        }
    }
    return constraint_noeq;
    
}

std::vector<SX> constraint::constraints_shape_eta(Parm* parm, const std::vector<std::vector<std::vector<SX>>>& gammaallnode, const std::vector<std::vector<std::vector<SX>>>& eta){
    std::vector<muscle*> Muscle=parm->getallmuscle();
    std::vector<SX> constraintshapeeta;
    for(int k=0; k<parm->getn_muscles();k++){
        for(int i=0; i<Muscle[k]->getnodenum()-2;i++){
            std::vector<SX> constraintshapeeta1=constraintshape_time_eta(Constraintshape->constraint_shape(gammaallnode[k][i+1], parm), eta[k][i]);
            for(int j=0;j<constraintshapeeta1.size();j++){
                constraintshapeeta.push_back(constraintshapeeta1[j]);
            }
        }
    }
    return constraintshapeeta;
}

std::vector<SX> constraint::constraints_Discrete_Euler_Lagrange_eachmuscle_eachnode(Parm* parm, int musclenum, const std::vector<std::vector<SX>>& gammaallnode, int nodenum, const std::vector<std::vector<SX>>& eta) {
    std::vector<muscle*> Muscle=parm->getallmuscle();
    std::vector<std::vector<SX>> Jacobianshape=Constraintshape->Jacobianshape(gammaallnode[nodenum], parm);
    std::vector<SX> Jacobian = Jacobian_time_eta(Jacobianshape, eta[nodenum-1]);
    std::vector<SX> geodesic = geodesic_function(gammaallnode[nodenum-1], gammaallnode[nodenum], gammaallnode[nodenum+1], (Muscle[musclenum]->getnodenum()-1.0)/1.0);
    std::vector<SX> ELeachnode;
    for(int j=0;j<3;j++){
        SX ELeachnode1=geodesic[j]-1.0/(Muscle[musclenum]->getnodenum()-1.0)*Jacobian[j];
        ELeachnode.push_back(ELeachnode1);
    }
    return ELeachnode;
}

std::vector<SX> constraint::constraints_Discrete_Euler_Lagrange_eachmuscle(Parm* parm, int musclenum, const std::vector<std::vector<SX>>& gammaallnode, const std::vector<std::vector<SX>>& eta){
    std::vector<muscle*> Muscle=parm->getallmuscle();
    std::vector<SX> ELeachmuscle;
    for(int i=0; i<Muscle[musclenum]->getnodenum()-2;i++){
        std::vector<SX> ELeachmuscle1=constraints_Discrete_Euler_Lagrange_eachmuscle_eachnode(parm, musclenum, gammaallnode, i+1, eta);
        for(int j=0;j<ELeachmuscle1.size();j++){
            ELeachmuscle.push_back(ELeachmuscle1[j]);
        }
    }
    return ELeachmuscle;
}

std::vector<SX> constraint::constraints_Discrete_Euler_Lagrange(Parm* parm, const std::vector<std::vector<std::vector<SX>>>& gammaallnode, const std::vector<std::vector<std::vector<SX>>>& eta){
    std::vector<SX> EL;
    for(int i=0; i<parm->getn_muscles();i++){
        std::vector<SX> EL1=constraints_Discrete_Euler_Lagrange_eachmuscle(parm, i, gammaallnode[i], eta[i]);
        for(int j=0;j<EL1.size();j++){
            EL.push_back(EL1[j]);
        }
    }
    return EL;
}

std::vector<std::vector<std::vector<std::vector<SX>>>> constraint::rearrange_gamma_eta(Parm* parm, SX x){
    std::vector<std::vector<std::vector<std::vector<SX>>>> dataall;
    std::vector<std::vector<std::vector<SX>>> gammaallnode;
    std::vector<std::vector<std::vector<SX>>> etaall;
    std::vector<muscle*> Muscle=parm->getallmuscle();
    int constraintnum_muscle=0;
    for(int i=0;i<parm->getn_muscles();i++){
        std::vector<std::vector<SX>> gammaallnodemuscle;
        std::vector<std::vector<SX>> etamuscle;

        for(int j=0;j<Muscle[i]->getnodenum();j++){
            std::vector<SX> gammaallnodemuscle1;            
            for(int k=0;k<3;k++){
                gammaallnodemuscle1.push_back(x(constraintnum_muscle+j*3+k));
            }
            gammaallnodemuscle.push_back(gammaallnodemuscle1);  
            if(j>0 && j<Muscle[i]->getnodenum()-1){
                std::vector<SX> etamuscle1;
                for(int k=0;k<parm->getn_bodies();k++){
                    etamuscle1.push_back(x(constraintnum_muscle+Muscle[i]->getnodenum()*3+(j-1)*parm->getn_bodies()+k));
                }
                etamuscle.push_back(etamuscle1);
            }            
        }

        constraintnum_muscle=constraintnum_muscle+Muscle[i]->getnodenum()*3+(Muscle[i]->getnodenum()-2)*parm->getn_bodies();
        gammaallnode.push_back(gammaallnodemuscle);
        etaall.push_back(etamuscle);
    }

    dataall.push_back(gammaallnode);
    dataall.push_back(etaall);

    return dataall;
}

std::vector<SX> constraint::constraintseq(Parm* parm, const std::vector<std::vector<std::vector<std::vector<SX>>>>& dataall){
    std::vector<std::vector<std::vector<SX>>> gammaallnode=dataall[0];
    std::vector<std::vector<std::vector<SX>>> etaall=dataall[1];
    std::vector<muscle*> Muscle=parm->getallmuscle();

    std::vector<SX> constraint_eq;
    std::vector<SX> gEuler = constraints_Discrete_Euler_Lagrange(parm, gammaallnode, etaall);
    std::vector<SX> constraintshape=constraints_shape_eta(parm, gammaallnode, etaall);
    constraint_eq.insert(constraint_eq.end(), gEuler.begin(), gEuler.end());
    for(int i=0;i<parm->getn_muscles();i++){
        std::vector<SX> fixtwoside=constraints_fix_muscle_two_side_point(parm,Muscle[i], gammaallnode[i]);
        constraint_eq.insert(constraint_eq.end(), fixtwoside.begin(), fixtwoside.end());
    }
    constraint_eq.insert(constraint_eq.end(), constraintshape.begin(), constraintshape.end());

    return constraint_eq;
}

std::vector<SX> constraint::constraints(Parm* parm, SX x){
    std::vector<std::vector<std::vector<std::vector<SX>>>> dataall= constraint::rearrange_gamma_eta(parm, x);
    std::vector<SX> constraint;

    //eq
    std::vector<SX> constrainteq=constraintseq(parm, dataall);
    constrainteqnum=constrainteq.size();
    constraint.insert(constraint.end(), constrainteq.begin(), constrainteq.end());

    //noeq
    std::vector<SX> constraintnoeq=constraintsnoeq(parm, dataall);
    constraintnoeqnum=constraintnoeq.size();
    constraint.insert(constraint.end(), constraintnoeq.begin(), constraintnoeq.end());
    constraintall=constraint;

    //set limit
    std::vector<double> eqlowerlimit(constrainteqnum, 0.0);
    std::vector<double> equpperlimit(constrainteqnum, 0.0);
    std::vector<double> noeqlowerlimit(constraintnoeqnum, 0.0);
    std::vector<double> noequpperlimit(constraintnoeqnum, inf);

    //set lower limit
    std::vector<double> constraintlowerlimit;
    constraintlowerlimit.insert(constraintlowerlimit.end(), eqlowerlimit.begin(), eqlowerlimit.end());
    constraintlowerlimit.insert(constraintlowerlimit.end(), noeqlowerlimit.begin(), noeqlowerlimit.end());
    lowerlimitall=constraintlowerlimit;

    //set lower limit
    std::vector<double> constraintupperlimit;
    constraintupperlimit.insert(constraintupperlimit.end(), equpperlimit.begin(), equpperlimit.end());
    constraintupperlimit.insert(constraintupperlimit.end(), noequpperlimit.begin(), noequpperlimit.end());
    upperlimitall=constraintupperlimit;
    return constraint;
}

std::vector<SX> constraint::geodesic_function(const std::vector<SX>& gamma_nm1, const std::vector<SX>& gamma_n, const std::vector<SX>& gamma_np1, double K){
    std::vector<SX> geodesic(3);
    for(int i=0;i<3;i++){
        geodesic[i] = ( 2*gamma_n[i] - gamma_np1[i] - gamma_nm1[i])*K;
    }
    return geodesic;
}

std::vector<SX> constraint::Jacobian_time_eta(const std::vector<std::vector<SX>>& Jacobian, const std::vector<SX>& eta){
    std::vector<SX> Jacobianeta;
    for(int i=0;i<3;i++){
        SX Jacobianetaj=Jacobian[0][i]*eta[0];
        for(int j=1; j<Jacobian.size(); j++){
            Jacobianetaj+=Jacobian[j][i]*eta[j];
        }
        Jacobianeta.push_back(Jacobianetaj);
    }
    return Jacobianeta;
}

std::vector<SX> constraint::constraintshape_time_eta(const std::vector<SX>& constraintshape, const std::vector<SX>& eta){
    std::vector<SX> constraintshapeeta;
    for(int i=0;i<constraintshape.size();i++){
        SX constraintshapeetaj=constraintshape[i]*eta[i];
        constraintshapeeta.push_back(constraintshapeetaj);
    }
    return constraintshapeeta;
}

void constraint::printvalue(const std::vector<std::vector<std::vector<SX>>>& gmmaall){
    for(int i=0;i<gmmaall.size();i++){
        for(int j=0;j<gmmaall[i].size();j++){
            for(int k=0;k<gmmaall[i][j].size();k++){
                std::cout<<"x:"<<gmmaall[i][j][k]<<std::endl;
            }
        }
    }
}   

constraintshape* constraint::getConstraintshape(){
    return Constraintshape;
}

int constraint::getconstrainteqnum(){
    return constrainteqnum;
}

int constraint::getconstraintnoeqnum(){
    return constraintnoeqnum;
}

std::vector<SX> constraint::getconstraintall(){
    return constraintall;
}

std::vector<double> constraint::getlowerlimitall(){
    return lowerlimitall;
}
    
std::vector<double> constraint::getupperlimitall(){
    return upperlimitall;
}
