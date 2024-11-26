/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "joint.h"

using namespace std;

std::vector<std::string> joint::alljoint_type = {"revolute joint", "spherical joint", "translate joint"};

joint::joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, double initialanglevalue, double anglevalue){
    setjoint(namevalue, bodynamevalue, currentbodyvalue, joint_typevalue, relative_posvalue, axisvectorvalue, initialanglevalue, anglevalue);
}

joint::joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, double initialangle1value, double initialangle2value, double initialangle3value, double angle1value, double angle2value, double angle3value){
    setjoint(namevalue, bodynamevalue, currentbodyvalue, joint_typevalue, relative_posvalue, initialangle1value, initialangle2value, initialangle3value, angle1value, angle2value, angle3value);
}

joint::joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<double>& initialrotationanglevalue, const std::vector<double>& rotationanglevalue, const std::vector<double>& initialtranslationvalue, const std::vector<double>& translationvalue){
    setjoint(namevalue, bodynamevalue, currentbodyvalue, joint_typevalue, relative_posvalue, axisvectorvalue, initialrotationanglevalue, rotationanglevalue, initialtranslationvalue, translationvalue);
}

joint::joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, double initialtranslationx, double initialtranslationy, double initialtranslationz, double translationx, double translationy, double translationz){
    setjoint(namevalue, bodynamevalue, currentbodyvalue, joint_typevalue, initialtranslationx, initialtranslationy, initialtranslationz, translationx, translationy, translationz);
}
    
joint::~joint(){

}

void joint::setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, double initialanglevalue, double anglevalue){
    name=namevalue;
    bodyname=bodynamevalue;
    currentbody=currentbodyvalue;
    joint_type=joint_typevalue;
    relative_pos=relative_posvalue;
    absolute_pos = localtoglobal(currentbody->getbodybasic()->getposition(), currentbody->getbodybasic()->getaxis(), relative_pos);
    initialrotationangle={initialanglevalue,0.0,0.0};
    rotationangle={anglevalue, 0.0, 0.0};
    axisvector=vector_unit(axisvectorvalue);
    rotation_axis_module=vector_module(vector3minus(rotationangle, initialrotationangle));
    absolute_axisvector=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), axisvector);
    currentstep={};
    initialtranslation={0.0, 0.0, 0.0};
    translation={0.0, 0.0, 0.0};
    absolute_translation=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), vector3minus(translation,initialtranslation));
    setwritemomentarm();
}

void joint::setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, double initialangle1value, double initialangle2value, double initialangle3value, double angle1value, double angle2value, double angle3value){
    name=namevalue;
    bodyname=bodynamevalue;
    currentbody=currentbodyvalue;
    joint_type=joint_typevalue;
    relative_pos=relative_posvalue;
    absolute_pos = localtoglobal(currentbody->getbodybasic()->getposition(), currentbody->getbodybasic()->getaxis(), relative_pos);
    initialrotationangle={initialangle1value,initialangle2value,initialangle3value};
    rotationangle={angle1value, angle2value, angle3value};
    axisvector=vector_unit(vector3minus(rotationangle, initialrotationangle));
    rotation_axis_module=vector_module(vector3minus(rotationangle, initialrotationangle));
    absolute_axisvector=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), axisvector);
    currentstep={};
    initialtranslation={0.0, 0.0, 0.0};
    translation={0.0, 0.0, 0.0};
    absolute_translation=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), vector3minus(translation,initialtranslation));
    setwritemomentarm();
}

void joint::setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<double>& initialrotationanglevalue, const std::vector<double>& rotationanglevalue, const std::vector<double>& initialtranslationvalue, const std::vector<double>& translationvalue){
    name=namevalue;
    bodyname=bodynamevalue;
    currentbody=currentbodyvalue;
    joint_type=joint_typevalue;
    relative_pos=relative_posvalue;
    absolute_pos = localtoglobal(currentbody->getbodybasic()->getposition(), currentbody->getbodybasic()->getaxis(), relative_pos);
    absolute_axisvector=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), axisvectorvalue);
    axisvector=vector_unit(axisvectorvalue);
    initialrotationangle=initialrotationanglevalue;
    rotationangle=rotationanglevalue;
    rotation_axis_module=vector_module(vector3minus(rotationangle, initialrotationangle));
    currentstep={};
    initialtranslation=initialtranslationvalue;
    translation=translationvalue;
    absolute_translation=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), vector3minus(translation,initialtranslation));
    setwritemomentarm();
}

void joint::setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, double initialtranslationx, double initialtranslationy, double initialtranslationz, double translationx, double translationy, double translationz){
    name=namevalue;
    bodyname=bodynamevalue;
    currentbody=currentbodyvalue;
    joint_type=joint_typevalue;
    relative_pos={0.0, 0.0, 0.0};
    absolute_pos = localtoglobal(currentbody->getbodybasic()->getposition(), currentbody->getbodybasic()->getaxis(), relative_pos);
    initialrotationangle={0.0, 0.0, 0.0};
    rotationangle={0.0, 0.0, 0.0};
    axisvector={0.0, 0.0, 0.0};
    rotation_axis_module=0;
    absolute_axisvector=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), axisvector);
    currentstep={};
    initialtranslation={initialtranslationx, initialtranslationy, initialtranslationz};
    translation={translationx, translationy, translationz};
    absolute_translation=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), vector3minus(translation,initialtranslation));
    setwritemomentarm();
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
    
std::vector<double> joint::getabsolute_pos(){
    return absolute_pos;
}

std::vector<double> joint::getaxisvector(){
    return axisvector;
}

std::vector<double> joint::getabsolute_axisvector(){
    return absolute_axisvector;
}

std::vector<double> joint::getrotationangle(){
    return rotationangle;
}

std::vector<double> joint::getinitialtranslation(){
    return initialtranslation;
}

std::vector<double> joint::gettranslation(){
    return translation;
}

std::vector<double> joint::getabsolute_translation(){
    return absolute_translation;
}

std::vector<int> joint::getcurrentstep(){
    return currentstep;
}

std::vector<double> joint::getinitialrotationangle(){
    return initialrotationangle;
}

int joint::getwritemomentarm(){
    return writemomentarm;
}

void joint::setwritemomentarm(){
    if(joint_type=="revolute joint" || joint_type=="spherical joint"){
        writemomentarm=1;
    }
}

void joint::absolute_pos_axis_update(){
    absolute_pos = localtoglobal(currentbody->getbodybasic()->getposition(), currentbody->getbodybasic()->getaxis(), relative_pos);
    absolute_axisvector=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), axisvector);
    absolute_translation=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), vector3minus(translation,initialtranslation));
}

std::vector<std::vector<double>> joint::rotation_matrix_update(double rotation_angle){
    absolute_pos_axis_update();
    if(currentstep.empty()){
        currentstep.push_back(0);
    }
    else{
        currentstep.push_back(currentstep.back()+1);
    }
    std::vector<std::vector<double>> R;
    if(currentstep.size()==1){
        rotation_angle=initialrotationangle[0];
    }
        
    if(rotation_angle<3){
        R=CayleyMap(vector3timeconstant(absolute_axisvector, rotation_angle/180.0*M_PI));
    }
    else{
        R=RodriguesMap(vector3timeconstant(absolute_axisvector, rotation_angle/180.0*M_PI));
    }
    return R;
}

void joint::revolute_update(int stepnum,body* Body, std::vector<std::vector<double>> R){
    
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

    std::vector<double> rhobody=vector3minus(oldposition,absolute_pos);

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
            revolute_update(stepnum,allchild[i],R);
        }
    }
}

void joint::spherical_update(int stepnum,body* Body, std::vector<std::vector<double>> R, int initial_change){

    if(currentstep.size()==1 && initial_change){
        if(initialrotationangle[0]<3 && initialrotationangle[1]<3 && initialrotationangle[2]<3){
            R=CayleyMap(vector3timeconstant(initialrotationangle, 1.0/180.0*M_PI));
        }
        else{
            R=RodriguesMap(vector3timeconstant(initialrotationangle, 1.0/180.0*M_PI));
        }
    }

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

    std::vector<double> rhobody=vector3minus(oldposition,absolute_pos);

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
            spherical_update(stepnum,allchild[i], R, 0);
        }
    }
}

void joint::translational_update(body* Body, std::vector<double> translation_per_step){
    if(currentstep.empty()){
        currentstep.push_back(0);
    }
    else{
        currentstep.push_back(currentstep.back()+1);
    }
    if(currentstep.size()==1){
        translation_per_step=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), initialtranslation);;
    }
    if(Body->getchild().size()>0){
        std::vector<body*> allchild=Body->getchild();
        for(int i=0;i<Body->getchild().size();i++){

            std::vector<double> body_update = allchild[i]->getbodybasic()->getbody_temporary_update();
            if (body_update.empty()){
                body_update = allchild[i]->getbodybasic()->getq().back();
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
            translational_update(allchild[i], translation_per_step);
        }
    }
}

void joint::updateall(int stepnum,int currentstepnum){
    if(joint_type=="revolute joint"){
        double angle_each_step=(rotationangle[0]-initialrotationangle[0])/stepnum;
        std::vector<std::vector<double>> R = rotation_matrix_update(angle_each_step);
        revolute_update(stepnum,currentbody, R);
    }
    if(joint_type=="spherical joint"){
        double angle_each_step=rotation_axis_module/stepnum;
        std::vector<std::vector<double>> R = rotation_matrix_update(angle_each_step);
        spherical_update(stepnum,currentbody,R, 1);
    }
    if(joint_type=="translate joint"){
        absolute_pos_axis_update();
        std::vector<double> translation_per_step=vector3timeconstant(absolute_translation,1.0/stepnum);
        body* findroot = currentbody;
        while(findroot->getparent()!=nullptr){
            findroot=findroot->getparent();
        }
        translational_update(findroot, translation_per_step);
    }
}

void joint::resetforrecalc(){
    currentstep.erase(currentstep.begin(), currentstep.end());
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

