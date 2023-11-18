/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "joint.h"

using namespace std;

std::vector<std::string> joint::alljoint_type = {"revolute joint", "spherical joint"};

joint::joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, double initialanglevalue, double anglevalue){
    setjoint(namevalue, bodynamevalue, currentbodyvalue, joint_typevalue, relative_posvalue, axisvectorvalue, initialanglevalue, anglevalue);
}

joint::joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, double initialangle1value, double initialangle2value, double initialangle3value, double angle1value, double angle2value, double angle3value){
    setjoint(namevalue, bodynamevalue, currentbodyvalue, joint_typevalue, relative_posvalue, initialangle1value, initialangle2value, initialangle3value, angle1value, angle2value, angle3value);
}

joint::joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<double>& initialrotationanglevalue, const std::vector<double>& rotationanglevalue){
    setjoint(namevalue, bodynamevalue, currentbodyvalue, joint_typevalue, relative_posvalue, axisvectorvalue, initialrotationanglevalue, rotationanglevalue);
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
    absolute_axisvector=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), axisvectorvalue);
    axisvector=axisvectorvalue;
    initialrotationangle={initialanglevalue,0.0,0.0};
    rotationangle={anglevalue, 0.0, 0.0};
    allrotationangle={};
}

void joint::setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, double initialangle1value, double initialangle2value, double initialangle3value, double angle1value, double angle2value, double angle3value){
    name=namevalue;
    bodyname=bodynamevalue;
    currentbody=currentbodyvalue;
    joint_type=joint_typevalue;
    relative_pos=relative_posvalue;
    absolute_pos = localtoglobal(currentbody->getbodybasic()->getposition(), currentbody->getbodybasic()->getaxis(), relative_pos);
    axisvector={0.0,0.0,0.0};
    absolute_axisvector={0.0,0.0,0.0};
    initialrotationangle={initialangle1value,initialangle2value,initialangle3value};
    rotationangle={angle1value, angle2value, angle3value};
    allrotationangle={};
}

void joint::setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<double>& initialrotationanglevalue, const std::vector<double>& rotationanglevalue){
    name=namevalue;
    bodyname=bodynamevalue;
    currentbody=currentbodyvalue;
    joint_type=joint_typevalue;
    relative_pos=relative_posvalue;
    absolute_pos = localtoglobal(currentbody->getbodybasic()->getposition(), currentbody->getbodybasic()->getaxis(), relative_pos);
    absolute_axisvector=matrix33time31sepcol(currentbody->getbodybasic()->getaxis(), axisvectorvalue);
    axisvector=axisvectorvalue;
    initialrotationangle=initialrotationanglevalue;
    rotationangle=rotationanglevalue;
    allrotationangle={};
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

std::vector<double> joint::getallrotationangle(){
    return allrotationangle;
}

std::vector<double> joint::getinitialrotationangle(){
    return initialrotationangle;
}

void joint::revolute_update(int nodenum,body* Body, int addrotate){
    
    if(addrotate){
        if(allrotationangle.empty()){
            allrotationangle.push_back(initialrotationangle[0]);
        }
        else{
            allrotationangle.push_back(allrotationangle.back()+(rotationangle[0]-initialrotationangle[0])/nodenum);
        }
    }
    double rotationanglestep=0.0;
    if(allrotationangle.size()==1){
        rotationanglestep=allrotationangle[0]/180.0*M_PI;
    }
    else{
        rotationanglestep=(allrotationangle[allrotationangle.size()-1]-allrotationangle[allrotationangle.size()-2])/180.0*M_PI;
    }
    std::vector<std::vector<double>> R;
    if(rotationanglestep<3){
        R=CayleyMap(vector3timeconstant(absolute_axisvector, rotationanglestep));
    }
    else{
        R=RodriguesMap(vector3timeconstant(absolute_axisvector, rotationanglestep));
    }
     
    std::vector<double> rhobody=vector3minus(Body->getbodybasic()->getposition(),absolute_pos);

    std::vector<double> position_new=vector3plus(vector3minus(matrix33time31tog(R, rhobody),rhobody),Body->getbodybasic()->getposition());
    std::vector<std::vector<double>> axis_new;
    std::vector<std::vector<double>> axis=Body->getbodybasic()->getaxis();
    for(int i=0; i<3; i++){
        axis_new.push_back(matrix33time31tog(R, axis[i]));
    }
    
    Body->getbodybasic()->addnewbodybasic(position_new,axis_new);
    Body->getbodybasic()->setrotatestatus(1);
    if(Body->getchild().size()>0){
        std::vector<body*> allchild=Body->getchild();
        for(int i=0;i<Body->getchild().size();i++){
            revolute_update(nodenum,allchild[i],0);
        }
    }
}

void joint::spherical_update(int nodenum,body* Body, int addrotate){
    if(false){
    Body->getbodybasic()->setrotatestatus(1);
    if(addrotate){
        if(allrotationangle.empty()){
            allrotationangle.push_back(0.0);
        }
        else{
            allrotationangle.push_back(allrotationangle[allrotationangle.size()-1]+rotationangle[0]/nodenum);
        }
    }
    if(Body->getchild().size()>0){
        std::vector<body*> allchild=Body->getchild();
        for(int i=0;i<Body->getchild().size();i++){
            spherical_update(nodenum,allchild[i],0);
        }
    }
    }
}

void joint::updateall(int nodenum){
    if(joint_type=="revolute joint"){
        revolute_update(nodenum,currentbody,1);
    }
    if(joint_type=="spherical joint"){
        spherical_update(nodenum,currentbody,1);
    }
}

void joint::resetforrecalc(){
    allrotationangle.erase(allrotationangle.begin(), allrotationangle.end());
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



