/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "joint.h"

using namespace std;

std::vector<std::string> joint::alljoint_type = {"revolute joint", "spherical joint", "translate joint"};

joint::joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<std::vector<std::vector<double>>>& move_setting_value, const std::vector<std::vector<double>>& movement_value){
    setjoint(namevalue, bodynamevalue, currentbodyvalue, joint_typevalue, relative_posvalue, axisvectorvalue, move_setting_value, movement_value);
}
    
joint::~joint(){

}

void joint::setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<std::vector<std::vector<double>>>& move_setting_value, const std::vector<std::vector<double>>& movement_value){
    name=namevalue;
    bodyname=bodynamevalue;
    currentbody=currentbodyvalue;
    joint_type=joint_typevalue;
    relative_pos=relative_posvalue; // input translation {0,0,0}
    absolute_pos = {};
    if(vector_module(axisvectorvalue)){
        axisvector = vector_unit(axisvectorvalue); // input translation {0,0,0} spherical {0,0,0}
    } else{
        axisvector = {0,0,0}; // input translation {0,0,0} spherical {0,0,0}
    }
        
    absolute_axisvector = {};
    move_setting=move_setting_value;
    if(movement_value.empty()){
        set_movement_from_move_setting();
    }
    else{
        movement=movement_value;
    }
    currentstep={};
    setwritemomentarm();
    set_movement_per_step();
    joint_stepnum=movement_per_step[0].size()-1;
}

std::string joint::getname(){
    return name;
}
    
std::string joint::getbodyname(){
    return bodyname;
}
    
body* joint::getcurrentbody(){
    return currentbody;
}
    
std::string joint::getjoint_type(){
    return joint_type;
}
    
std::vector<double> joint::getrelative_pos(){
    return relative_pos;
}
    
std::vector<std::vector<double>> joint::getabsolute_pos(){
    return absolute_pos;
}

std::vector<double> joint::getaxisvector(){
    return axisvector;
}

std::vector<std::vector<double>> joint::getabsolute_axisvector(){
    return absolute_axisvector;
}

std::vector<int> joint::getcurrentstep(){
    return currentstep;
}

std::vector<std::vector<std::vector<double>>> joint::get_move_setting(){
    return move_setting;
}

std::vector<std::vector<double>> joint::get_movement(){
    return movement;
}

std::vector<std::vector<double>> joint::get_movement_per_step(){
    return movement_per_step;
}

int joint::getwritemomentarm(){
    return writemomentarm;
}

int joint::getjoint_stepnum(){
    return joint_stepnum;
}

int joint::getread_from_movement(){
    return read_from_movement;
}

void joint::setread_from_movement(int read_from_movement_value){
    read_from_movement=read_from_movement_value;
}
   
void joint::setjoint_stepnum(int joint_stepnum_value){
    joint_stepnum=joint_stepnum_value;
}

void joint::setwritemomentarm(){
    if(joint_type=="revolute joint" || joint_type=="spherical joint"){
        writemomentarm=1;
    }
}

void joint::absolute_pos_axis_update(int currentstepnum){
    if(absolute_pos.size()<currentstepnum+1){
        absolute_pos.push_back(localtoglobal(currentbody->getbodybasic()->getposition(), currentbody->getbodybasic()->getaxis(), relative_pos));
        absolute_axisvector.push_back(matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), axisvector));
        //absolute_translation=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), vector3minus(translation,initialtranslation));
    }else{
        absolute_pos[currentstepnum] = localtoglobal(currentbody->getbodybasic()->getposition(), currentbody->getbodybasic()->getaxis(), relative_pos);
        absolute_axisvector[currentstepnum] = matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), axisvector);
    }  
}

std::vector<std::vector<double>> joint::rotation_matrix_update(double rotation_angle, int currentstepnum){
    currentstep.push_back(currentstepnum);
    absolute_pos_axis_update(currentstepnum);
    std::vector<std::vector<double>> R;
        
    if(rotation_angle<10){
        R=CayleyMap(vector3timeconstant(absolute_axisvector[currentstepnum], rotation_angle/180.0*M_PI));
    }
    else{
        R=RodriguesMap(vector3timeconstant(absolute_axisvector[currentstepnum], rotation_angle/180.0*M_PI));
    }

    return R;
}

void joint::revolute_update(body* Body, std::vector<std::vector<double>> R){
    if(g_enable_print){Body->getbodybasic()->set_R_time(R);}
    
    std::vector<double> body_update = Body->getbodybasic()->getbody_temporary_update();
    if (body_update.empty()){
        body_update = Body->getbodybasic()->getq().back();
    }

    std::vector<double> oldposition;
    std::vector<std::vector<double>> oldaxis;
    for(int i=0; i<3; i++){
        oldposition.push_back(body_update[i]);
        std::vector<double> oldaxis1;
        for (int j = 3+i*3; j < 6+i*3 ; j++) {
            oldaxis1.push_back(body_update[j]);
        }
        oldaxis.push_back(oldaxis1);
    }

    std::vector<double> rhobody=vector3minus(oldposition,absolute_pos.back());

    std::vector<double> position_new=vector3plus(vector3minus(matrix33time31tog(R, rhobody),rhobody),oldposition);
    std::vector<std::vector<double>> axis_new;
    for(int i=0; i<3; i++){
        axis_new.push_back(matrix33time31tog(R, oldaxis[i]));
    }

    Body->getbodybasic()->setbody_temporary_update(position_new,axis_new);
    Body->getbodybasic()->setrotatestatus(1);

    if(Body->getchild().size()>0){
        std::vector<body*> allchild=Body->getchild();
        for(int i=0;i<Body->getchild().size();i++){
            revolute_update(allchild[i],R);
        }
    }
}

double joint::spherical_axis_angle(double angle1value, double angle2value, double angle3value){
    std::vector<double> rotationanglevalue={angle1value, angle2value, angle3value};
    double rotation_angle=vector_module(rotationanglevalue);
    if(rotation_angle==0){
        axisvector={1,0,0};
    } else {
        axisvector=vector_unit(rotationanglevalue);
    }
    return rotation_angle;
}


void joint::spherical_update(body* Body, std::vector<std::vector<double>> R){
    if(g_enable_print){Body->getbodybasic()->set_R_time(R);}

    std::vector<double> body_update = Body->getbodybasic()->getbody_temporary_update();
    if (body_update.empty()){
        body_update = Body->getbodybasic()->getq().back();
    }

    std::vector<double> oldposition;
    std::vector<std::vector<double>> oldaxis;
    for(int i=0; i<3; i++){
        oldposition.push_back(body_update[i]);
        std::vector<double> oldaxis1;
        for (int j = 3+i*3; j < 6+i*3 ; j++) {
            oldaxis1.push_back(body_update[j]);
        }
        oldaxis.push_back(oldaxis1);
    }

    std::vector<double> rhobody=vector3minus(oldposition,absolute_pos.back());

    std::vector<double> position_new=vector3plus(vector3minus(matrix33time31tog(R, rhobody),rhobody),oldposition);
    std::vector<std::vector<double>> axis_new;
    for(int i=0; i<3; i++){
        axis_new.push_back(matrix33time31tog(R, oldaxis[i]));
    }

    Body->getbodybasic()->setbody_temporary_update(position_new,axis_new);
    Body->getbodybasic()->setrotatestatus(1);

    if(Body->getchild().size()>0){
        std::vector<body*> allchild=Body->getchild();
        for(int i=0;i<Body->getchild().size();i++){
            spherical_update(allchild[i], R);
        }
    }
}

void joint::translational_update(body* Body, std::vector<double> translation_per_step){

    if(Body->getname()!="fix_space"){
        std::vector<double> body_update = Body->getbodybasic()->getbody_temporary_update();
        if (body_update.empty()){
            body_update = Body->getbodybasic()->getq().back();
        }

        std::vector<double> oldposition;
        std::vector<std::vector<double>> oldaxis;
        for(int i=0; i<3; i++){
            oldposition.push_back(body_update[i]);
            std::vector<double> oldaxis1;
            for (int j = 3+i*3; j < 6+i*3 ; j++) {
                oldaxis1.push_back(body_update[j]);
            }
            oldaxis.push_back(oldaxis1);
        }

        std::vector<double> position_new=vector3plus(translation_per_step,oldposition);
        Body->getbodybasic()->setbody_temporary_update(position_new,oldaxis);
        Body->getbodybasic()->setrotatestatus(1);
    }

    if(Body->getchild().size()>0){
        std::vector<body*> allchild=Body->getchild();
        for(int i=0;i<Body->getchild().size();i++){
            translational_update(allchild[i], translation_per_step);
        }
    }
}

void joint::updateall(int currentstepnum){
    if(currentstepnum<movement_per_step[0].size()){
        if(joint_type=="revolute joint"){
            std::vector<std::vector<double>> R = rotation_matrix_update(movement_per_step[0][currentstepnum], currentstepnum);
            revolute_update(currentbody, R);
        }
        if(joint_type=="spherical joint"){
            double current_rotation_angle=spherical_axis_angle(movement_per_step[0][currentstepnum], movement_per_step[1][currentstepnum], movement_per_step[2][currentstepnum]);
            std::vector<std::vector<double>> R = rotation_matrix_update(current_rotation_angle, currentstepnum);
            spherical_update(currentbody,R);
        }
        if(joint_type=="translate joint"){
            currentstep.push_back(currentstepnum);
            absolute_pos_axis_update(currentstepnum);
            std::vector<double> translation_per_step={movement_per_step[0][currentstepnum], movement_per_step[1][currentstepnum], movement_per_step[2][currentstepnum]};
            /*
            body* findroot = currentbody;
            while(findroot->getparent()!=nullptr){
                findroot=findroot->getparent();
            }
            */
            translational_update(currentbody, translation_per_step);
        }
    }
}

std::vector<double> joint::set_movement_single_axis_single_interval(std::vector<double> move_setting_axis_interval, std::vector<double> move_setting_axis_line){
    //move_setting_axis_interval={initial,final,step,acceleration}
    double velocity= (move_setting_axis_interval[1]-move_setting_axis_interval[0]-move_setting_axis_interval[2]*(move_setting_axis_interval[2]-1)/2.0*move_setting_axis_interval[3])/move_setting_axis_interval[2];
    for(int i=0;i<move_setting_axis_interval[2];i++){
        move_setting_axis_line.push_back(move_setting_axis_line.back()+velocity+i*move_setting_axis_interval[3]);
    }
    return move_setting_axis_line;
}

std::vector<double> joint::set_movement_from_move_setting_single_axis(std::vector<std::vector<double>> move_setting_axis){
    std::vector<double> move_setting_axis_all={move_setting_axis[0][0]};
    for(int i=0;i<move_setting_axis.size();i++){
        if(i>0 && move_setting_axis[i][0]!=move_setting_axis[i-1][1]){
            move_setting_axis_all.push_back(move_setting_axis[i][0]);
        }
        move_setting_axis_all=set_movement_single_axis_single_interval(move_setting_axis[i],move_setting_axis_all);
    }
    return move_setting_axis_all;
}

void joint::set_movement_from_move_setting(){
    std::vector<std::vector<double>> movementvalue={};
    for(int i=0;i<move_setting.size();i++){
        if(!move_setting[i].empty()){
            movementvalue.push_back(set_movement_from_move_setting_single_axis(move_setting[i]));
        }
    }
    movement=movementvalue;
}

void joint::set_movement_per_step(){
    std::vector<std::vector<double>> movement_perstep_value={};
    if(joint_type=="revolute joint"){
        movement_perstep_value={{movement[0][0]}};
        for(int i=0;i<movement[0].size()-1;i++){
            movement_perstep_value[0].push_back(movement[0][i+1]-movement[0][i]);
        }
    }
    if(joint_type=="spherical joint"){
        movement_perstep_value={{movement[0][0]}, {movement[1][0]}, {movement[2][0]}};
        for(int j=0;j<3;j++){
            for(int i=0;i<movement[0].size()-1;i++){
                movement_perstep_value[j].push_back(movement[j][i+1]-movement[j][i]);
            }
        }
    }
    if(joint_type=="translate joint"){
        std::vector<std::vector<double>> absolute_trans= matrix33time3Nsepcol(currentbody->getbodybasic()->getaxis(), movement);
        movement_perstep_value={{absolute_trans[0][0]}, {absolute_trans[1][0]}, {absolute_trans[2][0]}};
        for(int j=0;j<3;j++){
            for(int i=0;i<absolute_trans[0].size()-1;i++){
                movement_perstep_value[j].push_back(absolute_trans[j][i+1]-absolute_trans[j][i]);
            }
        }
    }
    movement_per_step=movement_perstep_value;
}

void joint::resetforrecalc(){
    currentstep.erase(currentstep.begin(), currentstep.end());
    absolute_pos={};
    absolute_axisvector={};
}

int joint::getjointtypeindex(std::string joint_typename){
    int index=-1;
    for(int i=0;i<alljoint_type.size();i++){
        if(alljoint_type[i]==joint_typename){
            index=i;
            break;
        }
    }
    return index;
}

