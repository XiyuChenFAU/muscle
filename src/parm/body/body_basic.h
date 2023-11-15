/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef BODY_BASIC_H
#define BODY_BASIC_H

#include <vector>
#include <cmath>
#include <iostream>
#include "../../math/math.h"

class bodybasic {
public:
    bodybasic(const std::vector<double>& q0);
    bodybasic(const std::vector<double>& q0, const std::vector<double>& naxisvalue, double rotationanglevalue, const std::vector<double>& rhobodyvalue);
    bodybasic(const std::vector<double>& lastbodyposition, const std::vector<std::vector<double>>& lastbodyaxis, const std::vector<double>& naxisvalue, double rotationanglevalue, const std::vector<double>& rhobodyvalue);
    void setbodybasic(const std::vector<double>& q0);
    void setbodybasic(const std::vector<double>& q0, const std::vector<double>& naxisvalue, double rotationanglevalue, const std::vector<double>& rhobodyvalue);
    void setbodybasic(const std::vector<double>& lastbodyposition, const std::vector<std::vector<double>>& lastbodyaxis, const std::vector<double>& naxisvalue, double rotationanglevalue, const std::vector<double>& rhobodyvalue);
    void updatebodybasic(const std::vector<double>& lastbodyposition, const std::vector<std::vector<double>>& lastbodyaxis);
    ~bodybasic();
    std::vector<double> getposition();
    std::vector<std::vector<double>> getaxis();
    std::vector<std::vector<double>> getq();
    std::vector<double> getrhobody();
    int getrotatestatus();
    std::vector<double> getinitialsetting_naxis();
    double getinitialsetting_angle();
    std::vector<std::vector<double>> getaxisangle_ref();
    void setrotatestatus(int newstatus);
    void  setpoistionaxis(const std::vector<double>& q);
    std::vector<double> pushback(std::vector<double>& q, const std::vector<double>& value);
    void norotateaddvalue();
    void  addnewbodybasic(const std::vector<double>& newbodyposition, const std::vector<std::vector<double>>& newbodyaxis);
    void PrintParameters(const std::vector<double>& q0);
    std::vector<double> matrix_to_axisangle_ref_fix_space();
    void resetforrecalc();

private:
    std::vector<double> position;
    std::vector<std::vector<double>> axis;
    std::vector<std::vector<double>> q;
    std::vector<double> rhobody;
    std::vector<std::vector<double>> axisangle_ref;
    int rotatestatus;
    std::vector<double> initialsetting_naxis;
    double initialsetting_angle;
};

#endif // BODY_BASIC_H
