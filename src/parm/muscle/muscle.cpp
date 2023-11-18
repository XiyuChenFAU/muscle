/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "muscle.h"

using namespace std;

muscle::muscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, int global){
    setmuscle(allbody, gamma_o, rhoo_bodynamevalue, gamma_i, rhoi_bodynamevalue, namevalue, nodenumvalue, global);
}

muscle::muscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue){   
    setmuscle(allbody, rho_ovalue, rhoo_bodynamevalue, rho_ivalue, rhoi_bodynamevalue, namevalue, nodenumvalue);
}

muscle::~muscle() {
    // Destructor
}

void muscle::PrintParameters() {
    
}

void muscle::setmuscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, int global){
    name=namevalue;
    nodenum=nodenumvalue;
    rhoo_bodyname=rhoo_bodynamevalue;
    rhoi_bodyname=rhoi_bodynamevalue;
    rhoo_body=findbody(allbody, rhoo_bodyname);
    std::vector<std::vector<double>> rhoo_q=rhoo_body->getbodybasic()->getq();
    rho_o=globaltolocal({rhoo_q[0][0],rhoo_q[0][1],rhoo_q[0][2]},{{rhoo_q[0][3],rhoo_q[0][4],rhoo_q[0][5]},{rhoo_q[0][6],rhoo_q[0][7],rhoo_q[0][8]},{rhoo_q[0][9],rhoo_q[0][10],rhoo_q[0][11]}}, rhoo_body->getbodybasic()->getposition());
    rhoi_body=findbody(allbody, rhoi_bodyname);
    std::vector<std::vector<double>> rhoi_q=rhoi_body->getbodybasic()->getq();
    rho_i=globaltolocal({rhoi_q[0][0],rhoi_q[0][1],rhoi_q[0][2]},{{rhoi_q[0][3],rhoi_q[0][4],rhoi_q[0][5]},{rhoi_q[0][6],rhoi_q[0][7],rhoi_q[0][8]},{rhoi_q[0][9],rhoi_q[0][10],rhoi_q[0][11]}}, rhoi_body->getbodybasic()->getposition());
    gamma=interpolation(gamma_o, gamma_i, nodenum);
    std::vector<double> gamma1D=rearrangeto1D(gamma);
    if(gammaall.empty()){
        gammaall.push_back(gamma1D);
    }
    else{
        gammaall[0]=gamma1D;
    }
}

void muscle::setmuscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue){   
    name=namevalue;
    nodenum=nodenumvalue;
    rho_o=rho_ovalue;
    rhoo_bodyname=rhoo_bodynamevalue;
    rho_i=rho_ivalue;
    rhoi_bodyname=rhoi_bodynamevalue;
    rhoo_body=findbody(allbody, rhoo_bodyname);
    std::vector<std::vector<double>> rhoo_q=rhoo_body->getbodybasic()->getq();
    std::vector<double> gamma_o = localtoglobal({rhoo_q[0][0],rhoo_q[0][1],rhoo_q[0][2]},{{rhoo_q[0][3],rhoo_q[0][4],rhoo_q[0][5]},{rhoo_q[0][6],rhoo_q[0][7],rhoo_q[0][8]},{rhoo_q[0][9],rhoo_q[0][10],rhoo_q[0][11]}}, rho_o);
    rhoi_body=findbody(allbody, rhoi_bodyname);
    std::vector<std::vector<double>> rhoi_q=rhoi_body->getbodybasic()->getq();
    std::vector<double> gamma_i = localtoglobal({rhoi_q[0][0],rhoi_q[0][1],rhoi_q[0][2]},{{rhoi_q[0][3],rhoi_q[0][4],rhoi_q[0][5]},{rhoi_q[0][6],rhoi_q[0][7],rhoi_q[0][8]},{rhoi_q[0][9],rhoi_q[0][10],rhoi_q[0][11]}}, rho_i);
    gamma=interpolation(gamma_o, gamma_i, nodenum);
    std::vector<double> gamma1D=rearrangeto1D(gamma);
    if(gammaall.empty()){
        gammaall.push_back(gamma1D);
    }
    else{
        gammaall[0]=gamma1D;
    }
    //printmuscleinfo();
    //print2Dvalue(gamma);
}

int muscle::getnodenum(){
    return nodenum;
}

std::string muscle::getname(){
    return name;
}
    
// std::vector<double> muscle::getrho_o(){
//     return rho_o;
// }

// std::vector<double> muscle::getrho_i(){
//     return rho_i;
// }

std::vector<std::vector<double>> muscle::getgammaall(){
    return gammaall;
}
    
std::vector<std::vector<double>> muscle::getetaall(){
    return etaall;
}

std::vector<std::vector<double>> muscle::getgamma(){
    return gamma;
}

std::vector<std::vector<double>> muscle::geteta(){
    return eta;
}

std::vector<std::vector<double>> muscle::getmuscleparm(){
    return muscleparm;
}

std::string muscle::getrhoo_bodyname(){
    return rhoo_bodyname;
}
    
std::vector<double> muscle::getrho_o(){
    return rho_o;
}
    
std::string muscle::getrhoi_bodyname(){
    return rhoi_bodyname;
}

std::vector<double> muscle::getrho_i(){
    return rho_i;
}

std::vector<double> muscle::getrho_o_position(){
    std::vector<std::vector<double>> rhoo_q=rhoo_body->getbodybasic()->getq();
    std::vector<double> gamma_o = localtoglobal({rhoo_q[0][0],rhoo_q[0][1],rhoo_q[0][2]},{{rhoo_q[0][3],rhoo_q[0][4],rhoo_q[0][5]},{rhoo_q[0][6],rhoo_q[0][7],rhoo_q[0][8]},{rhoo_q[0][9],rhoo_q[0][10],rhoo_q[0][11]}}, rho_o);
    return gamma_o;
}

std::vector<double> muscle::getrho_i_position(){
    std::vector<std::vector<double>> rhoi_q=rhoi_body->getbodybasic()->getq();
    std::vector<double> gamma_i = localtoglobal({rhoi_q[0][0],rhoi_q[0][1],rhoi_q[0][2]},{{rhoi_q[0][3],rhoi_q[0][4],rhoi_q[0][5]},{rhoi_q[0][6],rhoi_q[0][7],rhoi_q[0][8]},{rhoi_q[0][9],rhoi_q[0][10],rhoi_q[0][11]}}, rho_i);
    return gamma_i;
}

std::vector<double> muscle::getrho_o_position_initial_global(){
    std::vector<double> gamma_o = localtoglobal(rhoo_body->getbodybasic()->getposition(), rhoo_body->getbodybasic()->getaxis(), rho_o);
    return gamma_o;
}

std::vector<double> muscle::getrho_i_position_initial_global(){
    std::vector<double> gamma_i = localtoglobal(rhoi_body->getbodybasic()->getposition(), rhoi_body->getbodybasic()->getaxis(), rho_i);
    return gamma_i;
}

void muscle::addgamma(const std::vector<std::vector<double>>& gammanew){
    gamma=gammanew;
    std::vector<double> gammanew1D=rearrangeto1D(gamma);
    gammaall.push_back(gammanew1D);
    if(gammaall.size()==etaall.size()){
        std::vector<double> muscleparm1;
        muscleparm1.insert(muscleparm1.end(), gammaall[gammaall.size()-1].begin(), gammaall[gammaall.size()-1].end());
        muscleparm1.insert(muscleparm1.end(), etaall[etaall.size()-1].begin(), etaall[etaall.size()-1].end());
        muscleparm.push_back(muscleparm1);
    }
}

void muscle::addeta(const std::vector<std::vector<double>>& etanew){
    eta=etanew;
    std::vector<double> etanew1D=rearrangeto1D(eta);
    etaall.push_back(etanew1D);
    if(gammaall.size()==etaall.size()){
        std::vector<double> muscleparm1;
        muscleparm1.insert(muscleparm1.end(), gammaall[gammaall.size()-1].begin(), gammaall[gammaall.size()-1].end());
        muscleparm1.insert(muscleparm1.end(), etaall[etaall.size()-1].begin(), etaall[etaall.size()-1].end());
        muscleparm.push_back(muscleparm1);
    }
}

void muscle::addgammaall(const std::vector<double>& gammanew){
    gammaall.push_back(gammanew);
    gamma=rearrangeto2D(gammanew,nodenum);
    if(gammaall.size()==etaall.size()){
        std::vector<double> muscleparm1;
        muscleparm1.insert(muscleparm1.end(), gammaall[gammaall.size()-1].begin(), gammaall[gammaall.size()-1].end());
        muscleparm1.insert(muscleparm1.end(), etaall[etaall.size()-1].begin(), etaall[etaall.size()-1].end());
        muscleparm.push_back(muscleparm1);
    }
}

void muscle::addetaall(const std::vector<double>& etanew){
    etaall.push_back(etanew);
    eta=rearrangeto2D(etanew,nodenum-2);
    if(gammaall.size()==etaall.size()){
        std::vector<double> muscleparm1;
        muscleparm1.insert(muscleparm1.end(), gammaall[gammaall.size()-1].begin(), gammaall[gammaall.size()-1].end());
        muscleparm1.insert(muscleparm1.end(), etaall[etaall.size()-1].begin(), etaall[etaall.size()-1].end());
        muscleparm.push_back(muscleparm1);
    }
}

void muscle::addmuscleparm(const std::vector<double>& muscleparmnew){
    std::vector<double> gammanew1D;
    std::vector<double> etanew1D;
    for(int i=0;i<muscleparmnew.size();i++){
        if(i<nodenum*3){
            gammanew1D.push_back(muscleparmnew[i]);
        }
        else{
            etanew1D.push_back(muscleparmnew[i]);
        }
    }
    addgammaall(gammanew1D);
    addetaall(etanew1D);
    muscleparm.push_back(muscleparmnew);
}

std::vector<std::vector<double>> muscle::interpolation(const std::vector<double>& gamma_o, const std::vector<double>& gamma_i, int nodenum){
    std::vector<std::vector<double>> gamma1;
    gamma1.push_back(gamma_o);
    double interval=1.0/(nodenum-1);
    std::vector<double> gammainterval=vector3timeconstant(vector3minus(gamma_i, gamma_o), interval);
    std::vector<double> gammabegin=gamma_o;
    for(int i=0;i<nodenum-2;i++){
        gammabegin=vector3plus(gammabegin, gammainterval);
        gamma1.push_back(gammabegin);
    }
    gamma1.push_back(gamma_i);
    return gamma1;
}

std::vector<double> muscle::rearrangeto1D(const std::vector<std::vector<double>>& value){
    std::vector<double> value1D;
    for(int i=0; i<value.size();i++){
        value1D.insert(value1D.end(), value[i].begin(), value[i].end());
    }
    return value1D;
}

std::vector<std::vector<double>> muscle::rearrangeto2D(const std::vector<double>& value, int rownum){
    std::vector<std::vector<double>> value2D;
    int colnum = std::round(value.size()/rownum);
    for(int i=0; i<rownum;i++){
        std::vector<double> value1D;
        value2D.push_back(value1D);
        for(int j=0; j<colnum;j++){
            value2D[i].push_back(value[3*colnum+j]);
        }
    }
    return value2D;
}

body* muscle::findbody(const std::vector<body*>& allbody, const std::string& bodyname){
    for (body* Body : allbody) {
        if(Body->getname()==bodyname){return Body;}
    }
    std::cout<<"not find any body\n";
    return nullptr;
}

void muscle::setinitialeta_gamma(const std::vector<body*>& allbody){
    std::vector<double> etavector((nodenum-2)*(allbody.size()-1), 0.0);
    eta=rearrangeto2D(etavector,nodenum-2);
    if(etaall.empty()){
        etaall.push_back(etavector);
    }
    else{
        etaall[0]=etavector;
    }
    std::vector<double> muscleparm1;
    muscleparm1.insert(muscleparm1.end(), gammaall[gammaall.size()-1].begin(), gammaall[gammaall.size()-1].end());
    muscleparm1.insert(muscleparm1.end(), etaall[etaall.size()-1].begin(), etaall[etaall.size()-1].end());
    if(muscleparm.empty()){
        muscleparm.push_back(muscleparm1);
    }
    else{
        muscleparm[0]=muscleparm1;
    }
    
    std::vector<double> gamma_o = localtoglobal(rhoo_body->getbodybasic()->getposition(), rhoo_body->getbodybasic()->getaxis(), rho_o);
    std::vector<double> gamma_i = localtoglobal(rhoi_body->getbodybasic()->getposition(), rhoi_body->getbodybasic()->getaxis(), rho_i);
    gamma=interpolation(gamma_o, gamma_i, nodenum);
    std::vector<double> gamma1D=rearrangeto1D(gamma);
    gammaall.push_back(gamma1D);

}

void muscle::deletegammaalllast(){
    gammaall.pop_back();
}

void muscle::print2Dvalue(const std::vector<std::vector<double>>& value){
    for(int i=0;i<value.size();i++){
        std::cout<<"row "<<i<<": ";
        for(int j=0;j<value[i].size();j++){
            std::cout<<value[i][j]<<", ";
        }
        std::cout<<std::endl;
    }
}

void muscle::printmuscleinfo(){
    std::cout<<"muscle name: "<<name<<std::endl;
    std::cout<<"origin body name: "<<rhoo_bodyname<<std::endl;
    std::vector<double> position_o = rhoo_body->getbodybasic()->getposition();
    std::cout<<"origin body position: "<<position_o[0]<<"\t"<<position_o[1]<<"\t"<<position_o[2]<<std::endl;
    std::vector<std::vector<double>> axis_o = rhoo_body->getbodybasic()->getaxis();
    std::cout<<"origin body axis_x: "<<axis_o[0][0]<<"\t"<<axis_o[0][1]<<"\t"<<axis_o[0][2]<<std::endl;
    std::cout<<"origin body axis_y: "<<axis_o[1][0]<<"\t"<<axis_o[1][1]<<"\t"<<axis_o[1][2]<<std::endl;
    std::cout<<"origin body axis_z: "<<axis_o[2][0]<<"\t"<<axis_o[2][1]<<"\t"<<axis_o[2][2]<<std::endl;
    std::cout<<"origin relative: "<<rho_o[0]<<"\t"<<rho_o[1]<<"\t"<<rho_o[2]<<std::endl;
    std::vector<double> gamma_o = localtoglobal(rhoo_body->getbodybasic()->getposition(), rhoo_body->getbodybasic()->getaxis(), rho_o);
    std::cout<<"origin global: "<<gamma_o[0]<<"\t"<<gamma_o[1]<<"\t"<<gamma_o[2]<<std::endl;
    std::cout<<"origin interpolation global: "<<gamma[0][0]<<"\t"<<gamma[0][1]<<"\t"<<gamma[0][2]<<std::endl;

    std::cout<<"insertion body name: "<<rhoi_bodyname<<std::endl;
    std::vector<double> position_i = rhoi_body->getbodybasic()->getposition();
    std::cout<<"insertion body position: "<<position_i[0]<<"\t"<<position_i[1]<<"\t"<<position_i[2]<<std::endl;
    std::vector<std::vector<double>> axis_i = rhoi_body->getbodybasic()->getaxis();
    std::cout<<"insertion body axis_x: "<<axis_i[0][0]<<"\t"<<axis_i[0][1]<<"\t"<<axis_i[0][2]<<std::endl;
    std::cout<<"insertion body axis_y: "<<axis_i[1][0]<<"\t"<<axis_i[1][1]<<"\t"<<axis_i[1][2]<<std::endl;
    std::cout<<"insertion body axis_z: "<<axis_i[2][0]<<"\t"<<axis_i[2][1]<<"\t"<<axis_i[2][2]<<std::endl;
    std::cout<<"insertion relative: "<<rho_i[0]<<"\t"<<rho_i[1]<<"\t"<<rho_i[2]<<std::endl;
    std::vector<double> gamma_i = localtoglobal(rhoi_body->getbodybasic()->getposition(), rhoi_body->getbodybasic()->getaxis(), rho_i);
    std::cout<<"insertion global: "<<gamma_i[0]<<"\t"<<gamma_i[1]<<"\t"<<gamma_i[2]<<std::endl;
    std::cout<<"insertion interpolation global: "<<gamma[nodenum-1][0]<<"\t"<<gamma[nodenum-1][1]<<"\t"<<gamma[nodenum-1][2]<<std::endl;
}

void muscle::resetforrecalc(){
    if (gammaall.size() > 1) {
        gamma=rearrangeto2D(gammaall[0],nodenum);
        gammaall.erase(gammaall.begin() + 1, gammaall.end());
        etaall.erase(etaall.begin() + 1, etaall.end());
        muscleparm.erase(muscleparm.begin() + 1, muscleparm.end());
    }
}

