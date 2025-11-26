/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef BODY_H
#define BODY_H

#include "shape.h"
#include "body_basic.h"

class shape;
class bodybasic;

class body {
public:
    body(const std::string& name);
    ~body();
    void PrintParameters();
    shape* getshape();
    bodybasic* getbodybasic();
    int getid();
    body* getparent();
    std::vector<body*> getchild();
    std::string getname();
    std::vector<double> getbodyglobalnaxis();
    std::vector<double> get_local_axis_angle_pos(int i);
    void setid(int idvalue);
    void setparent(body* parentbody);
    void setchild(body* childbody);
    void setchildall(const std::vector<body*>& childbody);
    void setshape(int addnew);
    void setshape(double a, double b, double c, const std::string& shapename, int addnew);
    void setshape(double length, double radius, const std::string& shapename, int addnew);
    void setbodybasic(const std::vector<double>& q0, int addnew);
    void setbodybasic(const std::vector<double>& naxisvalueglobal, double rotationanglevalueglobal, const std::vector<double>& positionglobal, int addnew, int global);
    void setbodybasic(const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, int addnew);
    double phi_ellipsoid(const std::vector<double>& positionlocal);
    double phi_cylinder(const std::vector<double>& positionlocal);
    double phi_torus(const std::vector<double>& positionlocal);
    double phi_shape(const std::vector<double>& gamma, int timenum);
    double phi_shape_current(const std::vector<double>& gamma);
    void updatebodybasic();

private:

    int id;
    body* parent=nullptr;
    std::vector<body*> child;
    std::string name;
    bodybasic * BodyBasic=nullptr;
    shape * Shape=nullptr;
};

#endif // BODY_H
