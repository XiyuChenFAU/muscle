/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include <iostream>
#include "../body/body.h"
#include "../../math/math.h"

class joint {
public:
    joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<double>& initialrotationanglevalue, const std::vector<double>& rotationanglevalue);
    joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, double initialanglevalue, double anglevalue);
    joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, double initialangle1value, double initialangle2value, double initialangle3value, double angle1value, double angle2value, double angle3value);
    
    ~joint();

    void setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<double>& initialrotationanglevalue, const std::vector<double>& rotationanglevalue);
    void setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, double initialanglevalue, double anglevalue);
    void setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, double initialangle1value, double initialangle2value, double initialangle3value, double angle1value, double angle2value, double angle3value);

    std::string getname();
    std::string getbodyname();
    body* getcurrentbody();
    std::string getjoint_type();
    std::vector<double> getrelative_pos();
    std::vector<double> getabsolute_pos();
    std::vector<double> getaxisvector();
    std::vector<double> getabsolute_axisvector();
    std::vector<double> getrotationangle();
    std::vector<double> getinitialrotationangle();
    std::vector<double> getallrotationangle();
    void revolute_update(int nodenum,body* Body, int addrotate);
    void spherical_update(int nodenum,body* Body, int addrotate);
    void updateall(int nodenum);
    void resetforrecalc();
    static std::vector<std::string> alljoint_type;
    int getjointtypeindex(std::string joint_typename);

private:
    std::string name;
    std::string bodyname;
    body* currentbody;
    std::string joint_type;
    std::vector<double> relative_pos;
    std::vector<double> absolute_pos;
    std::vector<double> axisvector={0.0,0.0,0.0};
    std::vector<double> absolute_axisvector={0.0,0.0,0.0};
    std::vector<double> initialrotationangle={0.0,0.0,0.0};
    std::vector<double> rotationangle={0.0,0.0,0.0};
    std::vector<double> allrotationangle;
    
};

#endif // JOINT_H
