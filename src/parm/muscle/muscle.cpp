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

muscle::muscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, int global, const std::vector<double>& gammavalue, const std::vector<double>& etavalue){
    setmuscle(allbody, gamma_o, rhoo_bodynamevalue, gamma_i, rhoi_bodynamevalue, namevalue, nodenumvalue, global, gammavalue, etavalue);
}

muscle::muscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, const std::vector<double>& gammavalue, const std::vector<double>& etavalue){   
    setmuscle(allbody, rho_ovalue, rhoo_bodynamevalue, rho_ivalue, rhoi_bodynamevalue, namevalue, nodenumvalue, gammavalue, etavalue);
}

muscle::~muscle() {
    // Destructor
}

void muscle::PrintParameters() {
    
}

void muscle::setmuscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, int global){
    name=namevalue;
    nodenum=nodenumvalue;
    deleteallnodes();

    node* node_o=new node(gamma_o, findbody(allbody, rhoo_bodynamevalue), 1);
    node* node_i=new node(gamma_i, findbody(allbody, rhoi_bodynamevalue), 1);
    gamma=interpolation(gamma_o, gamma_i, nodenum);
    all_nodes.push_back(node_o);
    for(int i=1;i<nodenum-1;i++){
        node* node_new=new node(gamma[i]);
        all_nodes.push_back(node_new);
    }
    all_nodes.push_back(node_i);

    if(gammaall.empty()){
        gammaall.push_back(rearrangeto1D(gamma));
    }
    else{
        gammaall[0]=rearrangeto1D(gamma);
    }
}

void muscle::setmuscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue){   
    name=namevalue;
    nodenum=nodenumvalue;
    deleteallnodes();

    node* node_o=new node(rho_ovalue, 1, findbody(allbody, rhoo_bodynamevalue), 1);
    node* node_i=new node(rho_ivalue, 1, findbody(allbody, rhoi_bodynamevalue), 1);
    gamma=interpolation(node_o->get_gamma_node(), node_i->get_gamma_node(), nodenum);
    all_nodes.push_back(node_o);
    for(int i=1;i<nodenum-1;i++){
        node* node_new=new node(gamma[i]);
        all_nodes.push_back(node_new);
    }
    all_nodes.push_back(node_i);

    if(gammaall.empty()){
        gammaall.push_back(rearrangeto1D(gamma));
    }
    else{
        gammaall[0]=rearrangeto1D(gamma);
    }
    //printmuscleinfo();
    //print2Dvalue(gamma);
}

void muscle::setmuscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, int global, const std::vector<double>& gammavalue, const std::vector<double>& etavalue){
    name=namevalue;
    nodenum=nodenumvalue;
    deleteallnodes();

    node* node_o=new node(gamma_o, findbody(allbody, rhoo_bodynamevalue), 1);
    node* node_i=new node(gamma_i, findbody(allbody, rhoi_bodynamevalue), 1);
    
    all_nodes.push_back(node_o);
    if(gammavalue.empty()){
        gamma=interpolation(node_o->get_gamma_node(), node_i->get_gamma_node(), nodenum);
        for(int i=1;i<nodenum-1;i++){
            node* node_new=new node(gamma[i]);
            all_nodes.push_back(node_new);
        }
        if(gammaall.empty()){
            gammaall.push_back(rearrangeto1D(gamma));
        }
        else{
            gammaall[0]=rearrangeto1D(gamma);
        }
    } else {
        gamma=rearrangeto2D(gammavalue,nodenum);
        for(int i=1;i<nodenum-1;i++){
            node* node_new=new node(gamma[i]);
            all_nodes.push_back(node_new);
        }
        if(gammaall.empty()){
            gammaall.push_back(gammavalue);
        }
        else{
            gammaall[0]=gammavalue;
        }
    }
    all_nodes.push_back(node_i);
    
    if(!etavalue.empty()){
        eta=rearrangeto2D(etavalue,nodenum-2);
        if(etaall.empty()){
            etaall.push_back(etavalue);
        }
        else{
            etaall[0]=etavalue;
        }
        for(int i=1;i<nodenum-1;i++){
            all_nodes[i]->add_eta_node(eta[i-1]);
        }
    }

}

void muscle::setmuscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, const std::vector<double>& gammavalue, const std::vector<double>& etavalue){   
    name=namevalue;
    nodenum=nodenumvalue;
    deleteallnodes();
    node* node_o=new node(rho_ovalue, 1, findbody(allbody, rhoo_bodynamevalue), 1);
    node* node_i=new node(rho_ivalue, 1, findbody(allbody, rhoi_bodynamevalue), 1);

    all_nodes.push_back(node_o);
    if(gammavalue.empty()){
        gamma=interpolation(node_o->get_gamma_node(), node_i->get_gamma_node(), nodenum);
        for(int i=1;i<nodenum-1;i++){
            node* node_new=new node(gamma[i]);
            all_nodes.push_back(node_new);
        }
        if(gammaall.empty()){
            gammaall.push_back(rearrangeto1D(gamma));
        }
        else{
            gammaall[0]=rearrangeto1D(gamma);
        }
    } else {
        gamma=rearrangeto2D(gammavalue,nodenum);
        for(int i=1;i<nodenum-1;i++){
            node* node_new=new node(gamma[i]);
            all_nodes.push_back(node_new);
        }
        if(gammaall.empty()){
            gammaall.push_back(gammavalue);
        }
        else{
            gammaall[0]=gammavalue;
        }
    }
    all_nodes.push_back(node_i);

    if(!etavalue.empty()){
        eta=rearrangeto2D(etavalue,nodenum-2);
        if(etaall.empty()){
            etaall.push_back(etavalue);
        }
        else{
            etaall[0]=etavalue;
        }
        for(int i=1;i<nodenum-1;i++){
            all_nodes[i]->add_eta_node(eta[i-1]);
        }
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
    return all_nodes[0]->get_ref_body()[0]->getname();
}
    
std::vector<double> muscle::getrho_o(){
    return all_nodes[0]->get_rho();
}
    
std::string muscle::getrhoi_bodyname(){
    return all_nodes.back()->get_ref_body()[0]->getname();
}

std::vector<double> muscle::getrho_i(){
    return all_nodes.back()->get_rho();
}

body* muscle::getrhoo_body(){
    return all_nodes[0]->get_ref_body()[0];
}
body* muscle::getrhoi_body(){
    return all_nodes.back()->get_ref_body()[0];
}

void muscle::set_read_muscle_value(int value){
    read_muscle_value=value;
}

int muscle::get_read_muscle_value(){
    return read_muscle_value;
}

void muscle::setnodenum(int nodenumvalue){
    nodenum=nodenumvalue;
}

std::vector<double> muscle::getrho_o_position_initial_global(){
    std::vector<std::vector<double>> rhoo_q=rhoo_body->getbodybasic()->getq();
    std::vector<double> gamma_o = localtoglobal({rhoo_q[0][0],rhoo_q[0][1],rhoo_q[0][2]},{{rhoo_q[0][3],rhoo_q[0][4],rhoo_q[0][5]},{rhoo_q[0][6],rhoo_q[0][7],rhoo_q[0][8]},{rhoo_q[0][9],rhoo_q[0][10],rhoo_q[0][11]}}, rho_o);
    return gamma_o;
}

std::vector<double> muscle::getrho_i_position_initial_global(){
    std::vector<std::vector<double>> rhoi_q=rhoi_body->getbodybasic()->getq();
    std::vector<double> gamma_i = localtoglobal({rhoi_q[0][0],rhoi_q[0][1],rhoi_q[0][2]},{{rhoi_q[0][3],rhoi_q[0][4],rhoi_q[0][5]},{rhoi_q[0][6],rhoi_q[0][7],rhoi_q[0][8]},{rhoi_q[0][9],rhoi_q[0][10],rhoi_q[0][11]}}, rho_i);
    return gamma_i;
}

std::vector<double> muscle::getrho_o_position(){
    std::vector<double> gamma_o = localtoglobal(rhoo_body->getbodybasic()->getposition(), rhoo_body->getbodybasic()->getaxis(), rho_o);
    return gamma_o;
}

std::vector<double> muscle::getrho_i_position(){
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
    if(read_muscle_value==0 || read_muscle_value==2){
        std::vector<double> etavector((nodenum-2)*(allbody.size()-1), 0.0);
        eta=rearrangeto2D(etavector,nodenum-2);
        if(etaall.empty()){
            etaall.push_back(etavector);
        }
        else{
            etaall[0]=etavector;
        }
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
    
    // body already make movement here, this one is new initial guess (local parameterization for very first step) for solving
    if(read_muscle_value<2){
        std::vector<double> gamma_o = localtoglobal(rhoo_body->getbodybasic()->getposition(), rhoo_body->getbodybasic()->getaxis(), rho_o);
        std::vector<double> gamma_i = localtoglobal(rhoi_body->getbodybasic()->getposition(), rhoi_body->getbodybasic()->getaxis(), rho_i);
        gamma=interpolation(gamma_o, gamma_i, nodenum);
        std::vector<double> gamma1D=rearrangeto1D(gamma);
        gammaall.push_back(gamma1D);
    }
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
    std::cout<<"origin body name: "<<all_nodes[0]->get_ref_body()[0]->getname()<<std::endl;
    std::vector<double> position_o = all_nodes[0]->get_ref_body()[0]->getbodybasic()->getposition();
    std::cout<<"origin body position: "<<position_o[0]<<"\t"<<position_o[1]<<"\t"<<position_o[2]<<std::endl;
    std::vector<std::vector<double>> axis_o = all_nodes[0]->get_ref_body()[0]->getbodybasic()->getaxis();
    std::cout<<"origin body axis_x: "<<axis_o[0][0]<<"\t"<<axis_o[0][1]<<"\t"<<axis_o[0][2]<<std::endl;
    std::cout<<"origin body axis_y: "<<axis_o[1][0]<<"\t"<<axis_o[1][1]<<"\t"<<axis_o[1][2]<<std::endl;
    std::cout<<"origin body axis_z: "<<axis_o[2][0]<<"\t"<<axis_o[2][1]<<"\t"<<axis_o[2][2]<<std::endl;
    std::vector<double> rho_o = all_nodes[0]->get_rho();
    std::cout<<"origin relative: "<<rho_o[0]<<"\t"<<rho_o[1]<<"\t"<<rho_o[2]<<std::endl;
    std::vector<double> gamma_o = all_nodes[0]->get_gamma_node();
    std::cout<<"origin global: "<<gamma_o[0]<<"\t"<<gamma_o[1]<<"\t"<<gamma_o[2]<<std::endl;

    std::cout<<"insertion body name: "<<all_nodes.back()->get_ref_body()[0]->getname()<<std::endl;
    std::vector<double> position_i = all_nodes.back()->get_ref_body()[0]->getbodybasic()->getposition();
    std::cout<<"insertion body position: "<<position_i[0]<<"\t"<<position_i[1]<<"\t"<<position_i[2]<<std::endl;
    std::vector<std::vector<double>> axis_i = all_nodes.back()->get_ref_body()[0]->getbodybasic()->getaxis();
    std::cout<<"insertion body axis_x: "<<axis_i[0][0]<<"\t"<<axis_i[0][1]<<"\t"<<axis_i[0][2]<<std::endl;
    std::cout<<"insertion body axis_y: "<<axis_i[1][0]<<"\t"<<axis_i[1][1]<<"\t"<<axis_i[1][2]<<std::endl;
    std::cout<<"insertion body axis_z: "<<axis_i[2][0]<<"\t"<<axis_i[2][1]<<"\t"<<axis_i[2][2]<<std::endl;
    std::vector<double> rho_i = all_nodes.back()->get_rho();
    std::cout<<"insertion relative: "<<rho_i[0]<<"\t"<<rho_i[1]<<"\t"<<rho_i[2]<<std::endl;
    std::vector<double> gamma_i = all_nodes.back()->get_gamma_node();
    std::cout<<"insertion global: "<<gamma_i[0]<<"\t"<<gamma_i[1]<<"\t"<<gamma_i[2]<<std::endl;
}

void muscle::resetforrecalc(){
    if (gammaall.size() > 1) {
        gamma=rearrangeto2D(gammaall[0],nodenum);
        gammaall.erase(gammaall.begin() + 1, gammaall.end());
        etaall.erase(etaall.begin() + 1, etaall.end());
        muscleparm.erase(muscleparm.begin() + 1, muscleparm.end());
    }
    for(int i=0;i<all_nodes.size();i++){
        all_nodes[i]->resetforrecalc_node();
    }
}

int muscle::getvariablenum(int n_bodies){
    int vari=nodenum*3+(nodenum-2)*n_bodies;
    return vari;
}

void muscle::deleteallnodes(){
    for (node* ptr : all_nodes) {
        delete ptr;
    }
    all_nodes.clear();
}

