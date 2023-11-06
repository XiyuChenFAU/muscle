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
#include "fsolve.h"


class Parm {
public:
    Parm();
    ~Parm();

    int getn_bodies();
    int getn_muscles();
    std::vector<body*> getallbody();
    body* getbodyindex(int index);
    std::vector<muscle*> getallmuscle();
    muscle* getmuscleindex(int index);
    Fsolve* getfsolve();
    void addbody(body* Body, const std::string& parentbodyname);
    void addbody(const std::string& bodyname, const std::vector<double>& q0);
    void addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double a, double b, double c, double length, double radius, const std::string& shapename);
    void addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double a, double b, double c, const std::string& shapename);
    void addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double length, double radius, const std::string& shapename);
    void addmuscle(muscle* Muscle);
    void addmuscle(const std::vector<double>& gamma_o, const std::string& rhoo_bodyname, const std::vector<double>& gamma_i, const std::string& rhoi_bodyname, const std::string& name, int nodenum,const std::string& global);
    void addmuscle(const std::vector<double>& rho_o, const std::string& rhoo_bodyname, const std::vector<double>& rho_i, const std::string& rhoi_bodyname, const std::string& name, int nodenum);
    body* findbody(const std::string& bodyname);
    int findbodyindex(const std::string& bodyname);
    void addmuslcesolution(const std::vector<double>& solution);
    void rotatebodyupdate(const std::string& bodyname, const std::vector<double>& naxis, double rotationangle);
    void rotatebody(body* Body, const std::vector<double>& naxis, double rotationangle);
    int getvariable();
    void setallmuscleinitialeta();
    int deletebody(const std::string& bodyname);
    int deletemuscle(const std::string& musclename);

private:
    int n_bodies;
    int n_muscles;
    std::vector<body*> allbody;
    std::vector<muscle*> allmuscle;
    Fsolve* fsolve=nullptr;
    int variablenum;
};

#endif // PARM_H
