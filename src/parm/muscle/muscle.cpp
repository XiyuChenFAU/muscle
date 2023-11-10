/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "muscle.h"

using namespace std;

muscle::muscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, const std::string& global){
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

void muscle::setmuscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, const std::string& global){
    name=namevalue;
    nodenum=nodenumvalue;
    rhoo_bodyname=rhoo_bodynamevalue;
    rhoi_bodyname=rhoi_bodynamevalue;
    body* rhoo_body=findbody(allbody, rhoo_bodyname);
    rho_o=matrix33time31tog(rhoo_body->getbodybasic()->getaxis(),vector3minus(gamma_o, rhoo_body->getbodybasic()->getposition()));
    body* rhoi_body=findbody(allbody, rhoi_bodyname);
    rho_i=matrix33time31tog(rhoi_body->getbodybasic()->getaxis(),vector3minus(gamma_i, rhoi_body->getbodybasic()->getposition()));
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
    body* rhoo_body=findbody(allbody, rhoo_bodyname);
    std::vector<double> gamma_o = localtoglobal(rhoo_body->getbodybasic()->getposition(), rhoo_body->getbodybasic()->getaxis(), rho_o);
    body* rhoi_body=findbody(allbody, rhoi_bodyname);
    std::vector<double> gamma_i = localtoglobal(rhoi_body->getbodybasic()->getposition(), rhoi_body->getbodybasic()->getaxis(), rho_i);
    gamma=interpolation(gamma_o, gamma_i, nodenum);
    std::vector<double> gamma1D=rearrangeto1D(gamma);
    if(gammaall.empty()){
        gammaall.push_back(gamma1D);
    }
    else{
        gammaall[0]=gamma1D;
    }
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

std::vector<double> muscle::getrho_o_position(const std::vector<body*>& allbody){
    body* rhoo_body=findbody(allbody, rhoo_bodyname);
    std::vector<double> gamma_o = localtoglobal(rhoo_body->getbodybasic()->getposition(), rhoo_body->getbodybasic()->getaxis(), rho_o);
    return gamma_o;
}

std::vector<double> muscle::getrho_i_position(const std::vector<body*>& allbody){
    body* rhoi_body=findbody(allbody, rhoi_bodyname);
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

void muscle::setinitialeta(int bodynum){
    std::vector<double> etavector((nodenum-2)*bodynum, 0.0);
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

void muscle::resetforrecalc(){
    if (gammaall.size() > 1) {
        gamma=rearrangeto2D(gammaall[0],nodenum);
        gammaall.erase(gammaall.begin() + 1, gammaall.end());
        etaall.erase(etaall.begin() + 1, etaall.end());
        muscleparm.erase(muscleparm.begin() + 1, muscleparm.end());
    }
}

