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
    joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<double>& initialrotationanglevalue, const std::vector<double>& rotationanglevalue, const std::vector<double>& initialtranslationvalue, const std::vector<double>& translationvalue);
    joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, double initialanglevalue, double anglevalue);
    joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, double initialangle1value, double initialangle2value, double initialangle3value, double angle1value, double angle2value, double angle3value);
    joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, double initialtranslationx, double initialtranslationy, double initialtranslationz, double translationx, double translationy, double translationz);
    
    ~joint();

    void setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<double>& initialrotationanglevalue, const std::vector<double>& rotationanglevalue, const std::vector<double>& initialtranslationvalue, const std::vector<double>& translationvalue);
    void setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, double initialanglevalue, double anglevalue);
    void setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, double initialangle1value, double initialangle2value, double initialangle3value, double angle1value, double angle2value, double angle3value);
    void setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, double initialtranslationx, double initialtranslationy, double initialtranslationz, double translationx, double translationy, double translationz);

    std::string getname();
    std::string getbodyname();
    body* getcurrentbody();
    std::string getjoint_type();
    std::vector<double> getrelative_pos();
    std::vector<double> getabsolute_pos();
    std::vector<double> getaxisvector();
    std::vector<double> getabsolute_axisvector();
    std::vector<double> getrotationangle();
    std::vector<double> getinitialtranslation();
    std::vector<double> gettranslation();
    std::vector<double> getabsolute_translation();
    std::vector<double> getinitialrotationangle();
    std::vector<int> getcurrentstep();
    int getwritemomentarm();
    void setwritemomentarm();
    void absolute_pos_axis_update();
    std::vector<std::vector<double>>  rotation_matrix_update(double rotation_angle);
    void revolute_update(int stepnum, body* Body, const std::vector<std::vector<double>> R);
    void spherical_update(int stepnum, body* Body, std::vector<std::vector<double>> R, int initial_change);
    void translational_update(body* Body, std::vector<double> translation_per_step);
    void updateall(int stepnum,int currentstepnum);
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
    std::vector<double> initialtranslation={0.0,0.0,0.0};
    std::vector<double> translation={0.0,0.0,0.0};
    std::vector<double> absolute_translation={0.0,0.0,0.0};
    double rotation_axis_module=0.0;
    std::vector<int> currentstep;
    int writemomentarm = 0;
    
};

#endif // JOINT_H
