/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "muscle.h"

using namespace std;

muscle::muscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, int global, const std::vector<double>& gammavalue, const std::vector<double>& etavalue, const std::vector<std::string>& consider_body_list_value){
    setmuscle(allbody, gamma_o, rhoo_bodynamevalue, gamma_i, rhoi_bodynamevalue, namevalue, nodenumvalue, global, gammavalue, etavalue, consider_body_list_value);
}

muscle::muscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, const std::vector<double>& gammavalue, const std::vector<double>& etavalue, const std::vector<std::string>& consider_body_list_value){   
    setmuscle(allbody, rho_ovalue, rhoo_bodynamevalue, rho_ivalue, rhoi_bodynamevalue, namevalue, nodenumvalue, gammavalue, etavalue, consider_body_list_value);
}

muscle::~muscle() {
    deleteallnodes();
    deletevia_point_list();
}

void muscle::PrintParameters() {
    
}

void muscle::setmuscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, int global, const std::vector<double>& gammavalue, const std::vector<double>& etavalue, const std::vector<std::string>& consider_body_list_value){
    name=namevalue;
    nodenum=nodenumvalue;
    consider_body_list=consider_body_list_value;
    deleteallnodes();

    node* node_o=new node(gamma_o, findbody(allbody, rhoo_bodynamevalue), 1);
    node* node_i=new node(gamma_i, findbody(allbody, rhoi_bodynamevalue), 1);
    
    all_nodes.push_back(node_o);
    if(gammavalue.empty()){
        std::vector<std::vector<double>> gamma_value=interpolation(node_o->get_gamma_node(), node_i->get_gamma_node(), nodenum);
        for(int i=1;i<nodenum-1;i++){
            node* node_new=new node(gamma_value[i]);
            all_nodes.push_back(node_new);
        }
        all_nodes.push_back(node_i);
    } else {
        for(int i=1;i<gammavalue.size()/3-1;i++){
            node* node_new=new node({gammavalue[3*i],gammavalue[3*i+1],gammavalue[3*i+2]});
            all_nodes.push_back(node_new);
        }
        all_nodes.push_back(node_i);
    }
    
    
    if(!etavalue.empty()){
        int consider_body_num=allbody.size()-1; // exclude ground body
        if(consider_body_list_value.size()>0){
            consider_body_num=consider_body_list_value.size();
        }
        int rownum = std::round(etavalue.size()/consider_body_num);
        std::vector<std::vector<double>> eta_value=rearrangeto2D(etavalue,rownum);
        int count_fixpoint=0;
        for(int i=0;i<all_nodes.size();i++){
            if(all_nodes[i]->get_fixpoint()){
                count_fixpoint++;
            } else{
                all_nodes[i]->add_eta_node(0, eta_value[i-count_fixpoint]);
            }
        }
    }

}

void muscle::setmuscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, const std::vector<double>& gammavalue, const std::vector<double>& etavalue, const std::vector<std::string>& consider_body_list_value){   
    name=namevalue;
    nodenum=nodenumvalue;
    consider_body_list=consider_body_list_value;
    deleteallnodes();
    node* node_o=new node(rho_ovalue, 1, findbody(allbody, rhoo_bodynamevalue), 1);
    node* node_i=new node(rho_ivalue, 1, findbody(allbody, rhoi_bodynamevalue), 1);

    all_nodes.push_back(node_o);
    if(gammavalue.empty()){
        std::vector<std::vector<double>> gamma_value=interpolation(node_o->get_gamma_node(), node_i->get_gamma_node(), nodenum);
        for(int i=1;i<nodenum-1;i++){
            node* node_new=new node(gamma_value[i]);
            all_nodes.push_back(node_new);
        }
        all_nodes.push_back(node_i);
    } else {
        for(int i=1;i<gammavalue.size()/3-1;i++){
            node* node_new=new node({gammavalue[3*i],gammavalue[3*i+1],gammavalue[3*i+2]});
            all_nodes.push_back(node_new);
        }
        all_nodes.push_back(node_i);
    }
    
    
    if(!etavalue.empty()){
        int consider_body_num=allbody.size()-1; // exclude ground body
        if(consider_body_list_value.size()>0){
            consider_body_num=consider_body_list_value.size();
        }
        int rownum = std::round(etavalue.size()/consider_body_num);
        std::vector<std::vector<double>> eta_value=rearrangeto2D(etavalue,rownum);
        int count_fixpoint=0;
        for(int i=0;i<all_nodes.size();i++){
            if(all_nodes[i]->get_fixpoint()){
                count_fixpoint++;
            } else{
                all_nodes[i]->add_eta_node(0, eta_value[i-count_fixpoint]);
            }
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
    std::vector<std::vector<double>> gammaall=all_nodes[0]->get_gammaall_node();
    for(int i =1;i<all_nodes.size();i++){
        std::vector<std::vector<double>> gammaall_node = all_nodes[i]->get_gammaall_node();
        for(int j=0;j<gammaall.size();j++){
            gammaall[j].insert(gammaall[j].end(), gammaall_node[j].begin(), gammaall_node[j].end());
        }
    }
    return gammaall;
}
    
std::vector<std::vector<double>> muscle::getetaall(){
    std::vector<std::vector<double>> etaall= {};
    for(int i =1;i<all_nodes.size()-1;i++){
        if (!all_nodes[i]->get_fixpoint()){
            if(etaall.empty()){
                etaall = all_nodes[i]->get_etaall_node();
                continue;
            } else {
                std::vector<std::vector<double>> etaall_node = all_nodes[i]->get_etaall_node();
                for(int j=0;j<etaall.size();j++){
                    etaall[j].insert(etaall[j].end(), etaall_node[j].begin(), etaall_node[j].end());
                }
            }
        }
    }
    return etaall;
}

std::vector<double> muscle::getgamma_step(int stepnum){
    std::vector<double> gammaall={};
    for(int i =0;i<all_nodes.size();i++){
        std::vector<double> gammaall_node = all_nodes[i]->get_gamma_node(stepnum);
        gammaall.insert(gammaall.end(), gammaall_node.begin(), gammaall_node.end());
    }
    return gammaall;
}

std::vector<double> muscle::geteta_step(int stepnum){
    std::vector<double> etaall= {};
    for(int i =1;i<all_nodes.size()-1;i++){
        if (!all_nodes[i]->get_fixpoint()){
            std::vector<double> etaall_node = all_nodes[i]->get_eta_node(stepnum);
            etaall.insert(etaall.end(), etaall_node.begin(), etaall_node.end());
        }
    }
    return etaall;
}

std::string muscle::getrhoo_bodyname(){
    return all_nodes[0]->get_ref_body(0)->getname();
}
    
std::vector<double> muscle::getrho_o(){
    return all_nodes[0]->get_rho();
}
    
std::string muscle::getrhoi_bodyname(){
    return all_nodes.back()->get_ref_body(0)->getname();
}

std::vector<double> muscle::getrho_i(){
    return all_nodes.back()->get_rho();
}

body* muscle::getrhoo_body(){
    return all_nodes[0]->get_ref_body(0);
}
body* muscle::getrhoi_body(){
    return all_nodes.back()->get_ref_body(0);
}

std::vector<node*> muscle::get_allnodes(){
    return all_nodes;
}

std::vector<std::string> muscle::get_consider_body_list(){
    return consider_body_list;
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

void muscle::set_node_partition(const std::vector<body*>& allbody, int constraint_local_mode_number, const std::string& constraint_local_select_bodyname, int init_mode_nr, const std::string& init_select_bodyname, int first_step_index){
    for(node* node_ptr : all_nodes){
        node_ptr->set_single_node_partition(allbody, constraint_local_mode_number, findbody(allbody, constraint_local_select_bodyname), init_mode_nr, findbody(allbody, init_select_bodyname), first_step_index);
    }
}

std::vector<double> muscle::getrho_o_position(){ //new position
    return localtoglobal(all_nodes[0]->get_ref_body(-1)->getbodybasic()->getposition(), all_nodes[0]->get_ref_body(-1)->getbodybasic()->getaxis(),all_nodes[0]->get_rho());
}

std::vector<double> muscle::getrho_i_position(){
    return localtoglobal(all_nodes.back()->get_ref_body(-1)->getbodybasic()->getposition(), all_nodes.back()->get_ref_body(-1)->getbodybasic()->getaxis(),all_nodes.back()->get_rho());
}

std::vector<node*> muscle::getvia_point_list(){
    return via_point_list;
}

void muscle::addvia_point_list(node* viapoint_node){
    via_point_list.push_back(viapoint_node);
}

void muscle::setvia_point_list(const std::vector<body*>& allbody, const std::vector<std::string>& rho_via_point_bodyname, const std::vector<std::vector<double>>& rho_via_value){
    for(int i=0;i<rho_via_point_bodyname.size();i++){
        body* via_body = findbody(allbody, rho_via_point_bodyname[i]);
        node* new_node = new node(rho_via_value[i], 1, via_body);
        via_point_list.push_back(new_node);
    }
}

void muscle::deletevia_point_list(){
    for(node* viapoint_node : via_point_list){
        delete viapoint_node;
    }
    via_point_list.clear();
}

void muscle::delete_eta_node(){
    for(int i=0;i<all_nodes.size();i++){
        if (!all_nodes[i]->get_fixpoint()){
            all_nodes[i]->delete_eta_all();
        }
    }
    if(read_muscle_value==1 || read_muscle_value==3){
        delete_eta =1;
    }
}

void muscle::addmuscleparm(const std::vector<double>& muscleparmnew, int local_mode_number){
    int eta_node_num=0;
    for(int i=0;i<all_nodes.size();i++){
        if(!all_nodes[i]->get_fixpoint()){
            eta_node_num=eta_node_num+1;
        }   
    }
    int gamma_value_all_num = 3*all_nodes.size();
    int eta_value_all_num = muscleparmnew.size()-gamma_value_all_num;
    int consider_body_num=std::round(eta_value_all_num/eta_node_num);

    if(local_mode_number>0){
        int count_fix_point=0;
        for(int i=0;i<all_nodes.size();i++){
            std::vector<double> gamma_single_node={muscleparmnew[3*i], muscleparmnew[3*i+1], muscleparmnew[3*i+2]};
            all_nodes[i]->add_localtoglobal_gamma_node(gamma_single_node);
            if(!all_nodes[i]->get_fixpoint()){
                std::vector<double> eta_single_node;
                for(int j=0;j<consider_body_num;j++){
                    eta_single_node.push_back(muscleparmnew[gamma_value_all_num + (i    - count_fix_point)*consider_body_num + j]); 
                }
                all_nodes[i]->add_eta_node(eta_single_node);
            } else{
                count_fix_point++;
            }
        }
    }
    else{
        int count_fix_point=0;
        for(int i=0;i<all_nodes.size();i++){
            std::vector<double> gamma_single_node={muscleparmnew[3*i], muscleparmnew[3*i+1], muscleparmnew[3*i+2]};
            all_nodes[i]->add_global_gamma_node(gamma_single_node);
            if(!all_nodes[i]->get_fixpoint()){
                std::vector<double> eta_single_node;
                for(int j=0;j<consider_body_num;j++){
                    eta_single_node.push_back(muscleparmnew[gamma_value_all_num + (i    - count_fix_point)*consider_body_num + j]); 
                }
                all_nodes[i]->add_eta_node(eta_single_node);
            } else{
                count_fix_point++;
            }
        }
    }
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
            value2D[i].push_back(value[i*colnum+j]);
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

std::vector<body*> muscle::get_constraint_consider_body(const std::vector<body*>& allbody){
    std::vector<body*> constraint_consider_body={};
    if(consider_body_list.size()){
        for(int i=0;i<consider_body_list.size();i++){
            constraint_consider_body.push_back(findbody(allbody, consider_body_list[i]));
        }
    } else {
        for(int i=0; i<allbody.size()-1;i++){
            constraint_consider_body.push_back(allbody[i+1]);
        }
    }
    return constraint_consider_body;
}

void muscle::setinitialeta_gamma(const std::vector<body*>& allbody){
    // 0 no read, 1 read eta only, 2 read gamma only, 3 read both
    if(read_muscle_value==0 || read_muscle_value==2 || delete_eta==1){
        int consider_body_list_num = allbody.size()-1;
        if(!consider_body_list.empty()){
            consider_body_list_num = consider_body_list.size();
        }
        for(int i=1;i<all_nodes.size()-1;i++){
            if(!all_nodes[i]->get_fixpoint()){
                std::vector<double> eta_single_node(consider_body_list_num, 0.0);
                all_nodes[i]->add_eta_node(0, eta_single_node);
            }
        }
        if(delete_eta==1){
            delete_eta=0;
        }
    }
}

void muscle::insert_point_node(node* Node_value){
    double shortest_distance = 1e10;
    int closest_index = -1;
    for (int i = 1; i < all_nodes.size() - 1; i++) {
        if (!all_nodes[i]->get_fixpoint()) {
            double distance = vectortime1(vector3minus(Node_value->get_gamma_node(0), all_nodes[i]->get_gamma_node(0)),vector3minus(Node_value->get_gamma_node(0), all_nodes[i]->get_gamma_node(0)));
            if (distance < shortest_distance) {
                shortest_distance = distance;
                closest_index = i;
            }
        }
    }
    double distance_before = vectortime1(vector3minus(Node_value->get_gamma_node(0), all_nodes[closest_index - 1]->get_gamma_node(0)),vector3minus(Node_value->get_gamma_node(0), all_nodes[closest_index - 1]->get_gamma_node(0)));
    double distance_after = vectortime1(vector3minus(Node_value->get_gamma_node(0), all_nodes[closest_index + 1]->get_gamma_node(0)),vector3minus(Node_value->get_gamma_node(0), all_nodes[closest_index + 1]->get_gamma_node(0)));
    if (distance_after < distance_before) {
        closest_index += 1;
    }
    all_nodes.insert(all_nodes.begin() + closest_index, Node_value);
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
    std::cout<<"origin body name: "<<all_nodes[0]->get_ref_body(0)->getname()<<std::endl;
    std::vector<double> position_o = all_nodes[0]->get_ref_body(0)->getbodybasic()->getposition();
    std::cout<<"origin body position: "<<position_o[0]<<"\t"<<position_o[1]<<"\t"<<position_o[2]<<std::endl;
    std::vector<std::vector<double>> axis_o = all_nodes[0]->get_ref_body(0)->getbodybasic()->getaxis();
    std::cout<<"origin body axis_x: "<<axis_o[0][0]<<"\t"<<axis_o[0][1]<<"\t"<<axis_o[0][2]<<std::endl;
    std::cout<<"origin body axis_y: "<<axis_o[1][0]<<"\t"<<axis_o[1][1]<<"\t"<<axis_o[1][2]<<std::endl;
    std::cout<<"origin body axis_z: "<<axis_o[2][0]<<"\t"<<axis_o[2][1]<<"\t"<<axis_o[2][2]<<std::endl;
    std::vector<double> rho_o = all_nodes[0]->get_rho();
    std::cout<<"origin relative: "<<rho_o[0]<<"\t"<<rho_o[1]<<"\t"<<rho_o[2]<<std::endl;
    std::vector<double> gamma_o = all_nodes[0]->get_gamma_node();
    std::cout<<"origin global: "<<gamma_o[0]<<"\t"<<gamma_o[1]<<"\t"<<gamma_o[2]<<std::endl;

    std::cout<<"insertion body name: "<<all_nodes.back()->get_ref_body(0)->getname()<<std::endl;
    std::vector<double> position_i = all_nodes.back()->get_ref_body(0)->getbodybasic()->getposition();
    std::cout<<"insertion body position: "<<position_i[0]<<"\t"<<position_i[1]<<"\t"<<position_i[2]<<std::endl;
    std::vector<std::vector<double>> axis_i = all_nodes.back()->get_ref_body(0)->getbodybasic()->getaxis();
    std::cout<<"insertion body axis_x: "<<axis_i[0][0]<<"\t"<<axis_i[0][1]<<"\t"<<axis_i[0][2]<<std::endl;
    std::cout<<"insertion body axis_y: "<<axis_i[1][0]<<"\t"<<axis_i[1][1]<<"\t"<<axis_i[1][2]<<std::endl;
    std::cout<<"insertion body axis_z: "<<axis_i[2][0]<<"\t"<<axis_i[2][1]<<"\t"<<axis_i[2][2]<<std::endl;
    std::vector<double> rho_i = all_nodes.back()->get_rho();
    std::cout<<"insertion relative: "<<rho_i[0]<<"\t"<<rho_i[1]<<"\t"<<rho_i[2]<<std::endl;
    std::vector<double> gamma_i = all_nodes.back()->get_gamma_node();
    std::cout<<"insertion global: "<<gamma_i[0]<<"\t"<<gamma_i[1]<<"\t"<<gamma_i[2]<<std::endl;
}

void muscle::resetforrecalc(){
    for(int i=0;i<all_nodes.size();i++){
        if(read_muscle_value==0 || read_muscle_value==2){
            all_nodes[i]->resetforrecalc_node(0);
        } else{
            all_nodes[i]->resetforrecalc_node(1);
        }
    }
}

int muscle::getvariablenum(int n_bodies){
    int vari=0;
    if(consider_body_list.size()>0){
        n_bodies=consider_body_list.size();
    }
    for(node* Node : all_nodes){
        vari=vari+3;
        if(!Node->get_fixpoint()){
            vari=vari+n_bodies;
        }
    }
    return vari;
}

int muscle::get_consider_bodynum(int n_bodies){
    if(consider_body_list.size()>0){
        n_bodies=consider_body_list.size();
    }
    return n_bodies;
}

void muscle::deleteallnodes(){
    for (node* ptr : all_nodes) {
        delete ptr;
    }
    all_nodes.clear();
}

