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
    joint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<std::vector<std::vector<double>>>& move_setting_value, const std::vector<std::vector<double>>& movement_value, int move_all_body_value);
    
    ~joint();

    void setjoint(const std::string& namevalue, const std::string& bodynamevalue, body* currentbodyvalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<std::vector<std::vector<double>>>& move_setting_value, const std::vector<std::vector<double>>& movement_value, int move_all_body_value);
    std::string getname();
    std::string getbodyname();
    body* getcurrentbody();
    std::string getjoint_type();
    std::vector<double> getrelative_pos();
    std::vector<std::vector<double>> getabsolute_pos();
    std::vector<double> getaxisvector();
    std::vector<std::vector<double>> getabsolute_axisvector();
    std::vector<int> getcurrentstep();
    std::vector<std::vector<std::vector<double>>> get_move_setting();
    std::vector<std::vector<double>> get_movement();
    std::vector<std::vector<double>> get_movement_per_step();
    int getwritemomentarm();
    int getjoint_stepnum();
    int getread_from_movement();
    int getmove_all_body();
    void setmove_all_body(int move_all_body_value);
    void setread_from_movement(int read_from_movement_value);
    void setjoint_stepnum(int joint_stepnum_value);
    void setwritemomentarm();
    std::vector<double> set_movement_single_axis_single_interval(std::vector<double> move_setting_axis_interval, std::vector<double> move_setting_axis_all);
    std::vector<double> set_movement_from_move_setting_single_axis(std::vector<std::vector<double>> move_setting_axis);
    void set_movement_from_move_setting();
    void set_movement_per_step();
    void absolute_pos_axis_update(int currentstepnum);
    std::vector<std::vector<double>>  rotation_matrix_update(double rotation_angle, int currentstepnum);
    void revolute_update(body* Body, const std::vector<std::vector<double>> R);
    double spherical_axis_angle(double angle1value, double angle2value, double angle3value);
    void spherical_update(body* Body, std::vector<std::vector<double>> R);
    void translational_update(body* Body, std::vector<double> translation_per_step);
    void updateall(int currentstepnum);
    void resetforrecalc();
    static std::vector<std::string> alljoint_type;
    int getjointtypeindex(std::string joint_typename);

private:
    std::string name;
    std::string bodyname;
    body* currentbody=nullptr;
    std::string joint_type;
    std::vector<double> relative_pos={0.0,0.0,0.0};
    std::vector<std::vector<double>> absolute_pos={};
    std::vector<double> axisvector={0.0,0.0,0.0};
    std::vector<std::vector<double>> absolute_axisvector={};
    int move_all_body=0;
    /*
    save initial and final rotation also stepnum and acceleration
    move_setting={  x:{
                       {initial, final, stepnum, acceleration},
                       {initial, final, stepnum, acceleration},
                      }
                    y:{}
                    z:{}
                }
    */
    std::vector<std::vector<std::vector<double>>> move_setting={};
    /*set movement each step*/
    std::vector<std::vector<double>> movement={};
    std::vector<std::vector<double>> movement_per_step={};
    // double rotation_axis_module=0.0;
    std::vector<int> currentstep;
    int writemomentarm = 0;
    int joint_stepnum = 0;
    int read_from_movement=0;
};

#endif // JOINT_H
