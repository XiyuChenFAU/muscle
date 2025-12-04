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

std::vector<MX> constraint::constraints_shape_noeta(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode){
    std::vector<MX> constraintshape;
    for(int i=0; i<Muscle->getnodenum()-2;i++){
        std::vector<MX> constraintshape1=Constraintshape->constraint_shape(gammaallnode[i+1], parm);
        constraintshape.insert(constraintshape.end(), constraintshape1.begin(), constraintshape1.end());
    }
    return constraintshape;
}

std::vector<MX> constraint::constraintsnoeq(Parm* parm, const std::vector<std::vector<std::vector<MX>>>& dataall, int musclenum){
    std::vector<std::vector<MX>> gammaallnode=dataall[0];
    std::vector<std::vector<MX>> etaall=dataall[1];
    muscle* Muscle=parm->getmuscleindex(musclenum);
    std::vector<MX> constraint_noeq;
    std::vector<MX> constraintsshapenoeta=constraints_shape_noeta(parm, Muscle, gammaallnode);
    constraint_noeq.insert(constraint_noeq.end(), constraintsshapenoeta.begin(), constraintsshapenoeta.end());

    for(int j=0; j<etaall.size();j++){
        for(int k=0; k<etaall[j].size();k++){
            constraint_noeq.push_back(etaall[j][k]);
        }
    }
    return constraint_noeq;
}

std::vector<MX> constraint::constraints_fix_muscle_two_side_point(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode) {
    std::vector<MX> musclefix;
    std::vector<double> gammarhoo=Muscle->getrho_o_position();
    std::vector<double> gammarhoi=Muscle->getrho_i_position();
    for(int j=0;j<3;j++){
        MX muscleo=gammarhoo[j] - gammaallnode[0][j];
        musclefix.push_back(muscleo);
    }
    for(int j=0;j<3;j++){
        MX musclei=gammarhoi[j] - gammaallnode[gammaallnode.size()-1][j];
        musclefix.push_back(musclei);
    }
    return musclefix;
}

std::vector<MX> constraint::constraints_shape_eta(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, const std::vector<std::vector<MX>>& eta){
    std::vector<MX> constraintshapeeta;
    MX plus_value=0.0;
    for(int i=0; i<Muscle->getnodenum()-2;i++){
        std::vector<MX> constraintshapeeta1=constraintshape_time_eta(Constraintshape->constraint_shape(gammaallnode[i+1], parm), eta[i]);
        if(phi_eta_plus){
            for(int j=0;j<constraintshapeeta1.size();j++){
                plus_value=plus_value+constraintshapeeta1[j];
            }
        } else{
            constraintshapeeta.insert(constraintshapeeta.end(), constraintshapeeta1.begin(), constraintshapeeta1.end());
        }
    }
    if(phi_eta_plus){
        constraintshapeeta.push_back(plus_value);
    }
    return constraintshapeeta;
}

std::vector<MX> constraint::constraints_Discrete_Euler_Lagrange_eachmuscle_eachnode(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int nodenum, const std::vector<std::vector<MX>>& eta) {
    std::vector<std::vector<MX>> Jacobianshape=Constraintshape->Jacobianshape(gammaallnode[nodenum], parm);
    std::vector<MX> Jacobian = Jacobian_time_eta(Jacobianshape, eta[nodenum-1]);
    std::vector<MX> geodesic = geodesic_function(gammaallnode[nodenum-1], gammaallnode[nodenum], gammaallnode[nodenum+1], (Muscle->getnodenum()-1.0)/1.0);
    std::vector<MX> ELeachnode;
    for(int j=0;j<3;j++){
        MX ELeachnode1=geodesic[j]-1.0/(Muscle->getnodenum()-1.0)*Jacobian[j];
        ELeachnode.push_back(ELeachnode1);
    }
    return ELeachnode;
}

std::vector<MX> constraint::constraints_Discrete_Euler_Lagrange_eachmuscle(Parm* parm, muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, const std::vector<std::vector<MX>>& eta){

    std::vector<MX> ELeachmuscle;
    for(int i=0; i<Muscle->getnodenum()-2;i++){
        std::vector<MX> ELeachmuscle1=constraints_Discrete_Euler_Lagrange_eachmuscle_eachnode(parm, Muscle, gammaallnode, i+1, eta);
        ELeachmuscle.insert(ELeachmuscle.end(), ELeachmuscle1.begin(), ELeachmuscle1.end());
    }
    return ELeachmuscle;
}

std::vector<MX> constraint::constraintseq(Parm* parm, const std::vector<std::vector<std::vector<MX>>>& dataall, int musclenum){
    std::vector<std::vector<MX>> gammaallnode=dataall[0];
    std::vector<std::vector<MX>> etaall=dataall[1];
    muscle* Muscle=parm->getmuscleindex(musclenum);
    std::vector<MX> constraint_eq;

    std::vector<MX> gEuler = constraints_Discrete_Euler_Lagrange_eachmuscle(parm, Muscle, gammaallnode, etaall);
    std::vector<MX> constraintshape=constraints_shape_eta(parm, Muscle, gammaallnode, etaall);
    std::vector<MX> fixtwoside=constraints_fix_muscle_two_side_point(Muscle, gammaallnode);

    
    constraint_eq.insert(constraint_eq.end(), gEuler.begin(), gEuler.end());
    constraint_eq.insert(constraint_eq.end(), constraintshape.begin(), constraintshape.end());
    constraint_eq.insert(constraint_eq.end(), fixtwoside.begin(), fixtwoside.end());

    return constraint_eq;
}

std::vector<std::vector<std::vector<MX>>> constraint::rearrange_gamma_eta(Parm* parm, MX x, int musclenum){
    std::vector<std::vector<std::vector<MX>>> dataall;
    std::vector<std::vector<MX>> gammaallnodemuscle;
    std::vector<std::vector<MX>> etamuscle;

    muscle* Muscle=parm->getmuscleindex(musclenum);
    for(int j=0;j<Muscle->getnodenum();j++){
        std::vector<MX> gammaallnodemuscle1;            
        for(int k=0;k<3;k++){
            gammaallnodemuscle1.push_back(x(j*3+k));
        }
        gammaallnodemuscle.push_back(gammaallnodemuscle1);  
        if(j>0 && j<Muscle->getnodenum()-1){
            std::vector<MX> etamuscle1;
            for(int k=0;k<parm->getn_bodies();k++){
                etamuscle1.push_back(x(Muscle->getnodenum()*3+(j-1)*parm->getn_bodies()+k));
            }
            etamuscle.push_back(etamuscle1);
        }            
    }

    dataall.push_back(gammaallnodemuscle);
    dataall.push_back(etamuscle);

    return dataall;
}

std::vector<MX> constraint::constraints(Parm* parm, MX x, int musclenum){
    std::vector<std::vector<std::vector<MX>>> dataall= constraint::rearrange_gamma_eta(parm, x, musclenum);
    std::vector<MX> constraint;

    //eq
    std::vector<MX> constrainteq=constraintseq(parm, dataall, musclenum);
    constrainteqnum=constrainteq.size();
    constraint.insert(constraint.end(), constrainteq.begin(), constrainteq.end());

    //noeq
    std::vector<MX> constraintnoeq=constraintsnoeq(parm, dataall, musclenum);
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
    if(g_enable_print){
        std::cout<<"bodynum: "<<parm->getn_bodies()<<std::endl;
        std::cout<<"musclenum: "<<parm->getn_muscles()<<std::endl;
        std::cout<<"constrainteqnum: "<<constrainteqnum<< " comment: (K-1)*B (for eta) + (K-1)*B (for phi each node each body)"<<std::endl;
        std::cout<<"constraintnoeqnum: "<<constraintnoeqnum<< " comment: (K-1)*3 (for geodesic) + 2*3 (for origin and insertion) + (K-1)*B (for eta)"<<std::endl;
        std::cout<<"lowerlimitall: "<<lowerlimitall.size()<< " comment: (K+1)*3 (for gamma) + (K-1)*B (for eta)"<<std::endl;
        std::cout<<"upperlimitall: "<<upperlimitall.size()<<std::endl;
    }
    return constraint;
}

std::vector<MX> constraint::geodesic_function(const std::vector<MX>& gamma_nm1, const std::vector<MX>& gamma_n, const std::vector<MX>& gamma_np1, double K){
    std::vector<MX> geodesic(3);
    for(int i=0;i<3;i++){
        geodesic[i] = ( 2*gamma_n[i] - gamma_np1[i] - gamma_nm1[i])*K;
    }
    return geodesic;
}

std::vector<MX> constraint::Jacobian_time_eta(const std::vector<std::vector<MX>>& Jacobian, const std::vector<MX>& eta){
    std::vector<MX> Jacobianeta;
    for(int i=0;i<3;i++){
        MX Jacobianetaj=Jacobian[0][i]*eta[0];
        for(int j=1; j<Jacobian.size(); j++){
            Jacobianetaj+=Jacobian[j][i]*eta[j];
        }
        Jacobianeta.push_back(Jacobianetaj);
    }
    return Jacobianeta;
}

std::vector<MX> constraint::constraintshape_time_eta(const std::vector<MX>& constraintshape, const std::vector<MX>& eta){
    std::vector<MX> constraintshapeeta;
    for(int i=0;i<constraintshape.size();i++){
        MX constraintshapeetaj=constraintshape[i]*eta[i];
        constraintshapeeta.push_back(constraintshapeetaj);
    }
    return constraintshapeeta;
}

void constraint::printvalue(const std::vector<std::vector<std::vector<MX>>>& gmmaall){
    for(int i=0;i<gmmaall.size();i++){
        for(int j=0;j<gmmaall[i].size();j++){
            for(int k=0;k<gmmaall[i][j].size();k++){
                std::cout<<"x:"<<gmmaall[i][j][k]<<std::endl;
            }
        }
    }
}

void constraint::set_phi_eta_plus(int value){
    phi_eta_plus = value;
}
    
int constraint::get_phi_eta_plus(){
    return phi_eta_plus;
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

std::vector<MX> constraint::getconstraintall(){
    return constraintall;
}

std::vector<double> constraint::getlowerlimitall(){
    return lowerlimitall;
}
    
std::vector<double> constraint::getupperlimitall(){
    return upperlimitall;
}
