/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <iostream>

class shape {
public:
    shape();
    shape(double avalue, double bvalue, double cvalue, const std::string& shapenamevalue);
    shape(double lengthvalue, double radiusvalue, const std::string& shapenamevalue);
    ~shape();
    void setshape(double avalue, double bvalue, double cvalue, const std::string& shapenamevalue);
    void setshape(double lengthvalue, double radiusvalue, const std::string& shapenamevalue);
    void PrintParameters();
    double getlength();
    double getradius();
    double geta();
    double getb();
    double getc();
    std::string getshapename();
    static std::vector<std::string> allshape;
    static int getshapeindex(const std::string& shapename);

private:
    std::string shapename;
    double length;
    double radius;
    double a;
    double b;
    double c;
};

#endif // std::string name;_H
