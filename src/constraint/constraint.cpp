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
    dict_constraint_all={};
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
    dict_constraint["inequality_constraint"]["constraint_phi"] = constraintsshapenoeta;
    constraint_noeq.insert(constraint_noeq.end(), constraintsshapenoeta.begin(), constraintsshapenoeta.end());

    std::vector<MX> constraintseta;
    for(int j=0; j<etaall.size();j++){
        for(int k=0; k<etaall[j].size();k++){
            constraintseta.push_back(etaall[j][k]);
        }
    }
    dict_constraint["inequality_constraint"]["constraint_eta"] = constraintseta;
    constraint_noeq.insert(constraint_noeq.end(), constraintseta.begin(), constraintseta.end());
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

    dict_constraint["equality_constraint"]["gEuler"] = gEuler;
    dict_constraint["equality_constraint"]["fix_two_side"] = fixtwoside;
    dict_constraint["equality_constraint"]["constraint_phi_eta"] = constraintshape;
    
    constraint_eq.insert(constraint_eq.end(), gEuler.begin(), gEuler.end());
    constraint_eq.insert(constraint_eq.end(), fixtwoside.begin(), fixtwoside.end());
    constraint_eq.insert(constraint_eq.end(), constraintshape.begin(), constraintshape.end());

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
    for (const auto& [innerKey, vec] : dict_constraint["equality_constraint"]) {
        constraint.insert(constraint.end(), dict_constraint["equality_constraint"][innerKey].begin(), dict_constraint["equality_constraint"][innerKey].end());
    }

    //noeq
    std::vector<MX> constraintnoeq=constraintsnoeq(parm, dataall, musclenum);
    for (const auto& [innerKey, vec] : dict_constraint["inequality_constraint"]) {
        constraint.insert(constraint.end(), dict_constraint["inequality_constraint"][innerKey].begin(), dict_constraint["inequality_constraint"][innerKey].end());
    }

    //store dict_constraint_all
    dict_constraint_all[musclenum] = dict_constraint;

    return constraint;
}

std::vector<MX> constraint::put_constraints_together(){
    std::map<std::string, std::map<std::string, std::vector<MX>>> dict_constraint_together {
        {"equality_constraint", {}},
        {"inequality_constraint", {}}
    };

    for (const auto& [musclenum, muscle_single_constraint] : dict_constraint_all) {
        for (const auto& [constraint_type, constraint_value] : dict_constraint_all[musclenum]["equality_constraint"]) {
            if(dict_constraint_together["equality_constraint"].count(constraint_type)){
                if(constraint_type=="constraint_phi_eta"){
                    if(phi_eta_plus){
                        dict_constraint_together["equality_constraint"][constraint_type][0]=dict_constraint_together["equality_constraint"][constraint_type][0]+constraint_value[0];
                    } else {
                        dict_constraint_together["equality_constraint"][constraint_type].insert(dict_constraint_together["equality_constraint"][constraint_type].end(), constraint_value.begin(), constraint_value.end());
                    }
                } else {
                    dict_constraint_together["equality_constraint"][constraint_type].insert(dict_constraint_together["equality_constraint"][constraint_type].end(), constraint_value.begin(), constraint_value.end());
                }
            } else {
                dict_constraint_together["equality_constraint"][constraint_type] = constraint_value;
            }
        }
        for (const auto& [constraint_type, constraint_value] : dict_constraint_all[musclenum]["inequality_constraint"]) {
            if(dict_constraint_together["inequality_constraint"].count(constraint_type)){
                dict_constraint_together["inequality_constraint"][constraint_type].insert(dict_constraint_together["inequality_constraint"][constraint_type].end(), constraint_value.begin(), constraint_value.end());
            } else {
                dict_constraint_together["inequality_constraint"][constraint_type] = constraint_value;
            }
        }
    }
    dict_constraint = dict_constraint_together;

    std::vector<MX> constraint;
    //eq
    for (const auto& [innerKey, vec] : dict_constraint["equality_constraint"]) {
        constraint.insert(constraint.end(), dict_constraint["equality_constraint"][innerKey].begin(), dict_constraint["equality_constraint"][innerKey].end());
    }
    //noeq
    for (const auto& [innerKey, vec] : dict_constraint["inequality_constraint"]) {
        constraint.insert(constraint.end(), dict_constraint["inequality_constraint"][innerKey].begin(), dict_constraint["inequality_constraint"][innerKey].end());
    }

    return constraint;
}

std::vector<std::vector<double>> constraint::set_lower_upper_limit(){
    std::vector<std::vector<double>> lower_upper_limit;
    //eq
    int constraint_eqnum=0;
    for (const auto& [innerKey, vec] : dict_constraint["equality_constraint"]) {
        if(g_enable_print){std::cout<<"innerKey: "<<innerKey<< " size: "<< dict_constraint["equality_constraint"][innerKey].size()<<std::endl;}
        constraint_eqnum = constraint_eqnum + dict_constraint["equality_constraint"][innerKey].size();
    }

    //noeq
    int constraint_ineqnum=0;
    for (const auto& [innerKey, vec] : dict_constraint["inequality_constraint"]) {
        if(g_enable_print){std::cout<<"innerKey: "<<innerKey<< " size: "<< dict_constraint["inequality_constraint"][innerKey].size()<<std::endl;}
        constraint_ineqnum = constraint_ineqnum + dict_constraint["inequality_constraint"][innerKey].size();
    }
    //set limit
    std::vector<double> eqlowerlimit(constraint_eqnum, 0.0);
    std::vector<double> equpperlimit(constraint_eqnum, 0.0);
    std::vector<double> noeqlowerlimit(constraint_ineqnum, 0.0);
    std::vector<double> noequpperlimit(constraint_ineqnum, inf);

    //set lower limit
    std::vector<double> constraint_lowerlimit;
    constraint_lowerlimit.insert(constraint_lowerlimit.end(), eqlowerlimit.begin(), eqlowerlimit.end());
    constraint_lowerlimit.insert(constraint_lowerlimit.end(), noeqlowerlimit.begin(), noeqlowerlimit.end());
    lower_upper_limit.push_back(constraint_lowerlimit);

    //set upper limit
    std::vector<double> constraint_upperlimit;
    constraint_upperlimit.insert(constraint_upperlimit.end(), equpperlimit.begin(), equpperlimit.end());
    constraint_upperlimit.insert(constraint_upperlimit.end(), noequpperlimit.begin(), noequpperlimit.end());
    lower_upper_limit.push_back(constraint_upperlimit);

    if(g_enable_print){
        std::cout<<"eqlowerlimit size: "<<eqlowerlimit.size()<<std::endl;
        std::cout<<"noeqlowerlimit size: "<<noeqlowerlimit.size()<<std::endl;
        std::cout<<"equpperlimit size: "<<equpperlimit.size()<<std::endl;
        std::cout<<"noequpperlimit size: "<<noequpperlimit.size()<<std::endl;
        std::cout<<"constrainteqnum: "<<constraint_eqnum<< " comment: (K-1)*B (for eta) + (K-1)*B (for phi each node each body)"<<std::endl;
        std::cout<<"constraintnoeqnum: "<<constraint_ineqnum<< " comment: (K-1)*3 (for geodesic) + 2*3 (for origin and insertion) + (K-1)*B (for eta)"<<std::endl;
        std::cout<<"lowerlimitall: "<<constraint_lowerlimit.size()<< " comment: (K+1)*3 (for gamma) + (K-1)*B (for eta)"<<std::endl;
        std::cout<<"upperlimitall: "<<constraint_upperlimit.size()<<std::endl;
    }
    return lower_upper_limit;
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

void constraint::set_dict_constraint_all(const std::map<int, std::map<std::string, std::map<std::string, std::vector<MX>>>>& dict){
    dict_constraint_all = dict;
}

constraintshape* constraint::getConstraintshape(){
    return Constraintshape;
}