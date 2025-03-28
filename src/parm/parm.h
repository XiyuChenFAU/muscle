/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef PARM_H
#define PARM_H

#include "../math/math.h"
#include "body/body.h"
#include "muscle/muscle.h"
#include "joint/joint.h"
#include "fsolve.h"


class Parm {
public:
    Parm();
    ~Parm();

    int getn_bodies();
    int getn_muscles();
    int getn_joints();
    int get_write_moment_joints();
    std::vector<body*> getallbody();
    body* getbodyindex(int index);
    std::vector<muscle*> getallmuscle();
    muscle* getmuscleindex(int index);
    std::vector<joint*> getalljoint();
    joint* getjointindex(int index);
    Fsolve* getfsolve();
    int get_read_muscle_value();
    void set_read_muscle_value(int value);
    void addbody(body* Body, const std::string& parentbodyname);
    void addbody(const std::string& bodyname, const std::vector<double>& q0);
    void addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double a, double b, double c, double length, double radius, const std::string& shapename,int global=0);
    void addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double a, double b, double c, const std::string& shapename,int global=0);
    void addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double length, double radius, const std::string& shapename,int global=0);
    void updatechildbody(body* Body);

    void addmuscle(muscle* Muscle);
    void addmuscle(const std::vector<double>& gamma_o, const std::string& rhoo_bodyname, const std::vector<double>& gamma_i, const std::string& rhoi_bodyname, const std::string& name, int nodenum,int global=0);
    void addmuscle(const std::vector<double>& gamma_o, const std::string& rhoo_bodyname, const std::vector<double>& gamma_i, const std::string& rhoi_bodyname, const std::string& name, int nodenum,int global, const std::vector<double>& gammavalue, const std::vector<double>& etavalue);

    void addjoint(const std::string& namevalue, const std::string& bodynamevalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, double initialanglevalue, double anglevalue);
    void addjoint(const std::string& namevalue, const std::string& bodynamevalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, double initialangle1value, double initialangle2value, double initialangle3value, double angle1value, double angle2value, double angle3value);
    void addjoint(const std::string& namevalue, const std::string& bodynamevalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<double>& initialrotationanglevalue, const std::vector<double>& rotationanglevalue, const std::vector<double>& initialtranslationvalue, const std::vector<double>& translationvalue);
    void addjoint(const std::string& namevalue, const std::string& bodynamevalue, const std::string& joint_typevalue, double initialtranslationx, double initialtranslationy, double initialtranslationz, double translationx, double translationy, double translationz);

    body* findbody(const std::string& bodyname);
    int findbodyindex(const std::string& bodyname);
    void addmuslcesolution(const std::vector<double>& solution);
    void rotatebodyupdate(int nodenum,int stepnum);
    int getvariable();
    void setallmuscleinitialeta_gamma();

    int deletebody(const std::string& bodyname);
    int deletemuscle(const std::string& musclename);
    int deletejoint(const std::string& jointname);

    void resetallforrecalc();

private:
    int n_bodies;
    int n_muscles;
    int n_joints;
    std::vector<body*> allbody;
    std::vector<muscle*> allmuscle;
    std::vector<joint*> alljoint;
    Fsolve* fsolve=nullptr;
    int variablenum;
    int read_muscle_value=0;
};

#endif // PARM_H
