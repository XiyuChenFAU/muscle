/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef MUSCLE_H
#define MUSCLE_H

#include <vector>
#include <iostream>
#include "../body/body.h"
#include "../../math/math.h"

class muscle {
public:
    muscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, int global);
    muscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue);
    ~muscle();
    void PrintParameters();

    int getnodenum();
    std::string getname();
    // std::vector<double> getrho_o();
    // std::vector<double> getrho_i();
    void setmuscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, int global);
    void setmuscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue);
    std::vector<std::vector<double>> getgammaall();
    std::vector<std::vector<double>> getetaall();
    std::vector<std::vector<double>> getgamma();
    std::vector<std::vector<double>> geteta();
    std::vector<std::vector<double>> getmuscleparm();
    std::string getrhoo_bodyname();
    std::vector<double> getrho_o();
    std::string getrhoi_bodyname();
    std::vector<double> getrho_i();
    std::vector<double> getrho_o_position();
    std::vector<double> getrho_i_position();
    std::vector<double> getrho_o_position_initial_global();
    std::vector<double> getrho_i_position_initial_global();

    void addgamma(const std::vector<std::vector<double>>& gammanew);
    void addeta(const std::vector<std::vector<double>>& etanew);
    void addgammaall(const std::vector<double>& gammanew);
    void addetaall(const std::vector<double>& etanew);
    void addmuscleparm(const std::vector<double>& muscleparmnew);

    std::vector<std::vector<double>> interpolation(const std::vector<double>& gamma_o, const std::vector<double>& gamma_i, int nodenum);
    std::vector<double> rearrangeto1D(const std::vector<std::vector<double>>& value);
    std::vector<std::vector<double>> rearrangeto2D(const std::vector<double>& value, int rownum);
    body* findbody(const std::vector<body*>& allbody, const std::string& bodyname);
    void setinitialeta_gamma(const std::vector<body*>& allbody);
    void deletegammaalllast();

    void print2Dvalue(const std::vector<std::vector<double>>& value);
    void printmuscleinfo();
    void resetforrecalc();
    int getvariablenum(int n_bodies);


private:
    int nodenum;
    std::string name;
    //std::vector<double> rho_o;
    //std::vector<double> rho_i;
    std::vector<std::vector<double>> gammaall;
    std::vector<std::vector<double>> etaall;
    std::vector<std::vector<double>> gamma;
    std::vector<std::vector<double>> eta;
    std::vector<std::vector<double>> muscleparm;
    std::string rhoo_bodyname;
    std::vector<double> rho_o;
    std::string rhoi_bodyname;
    std::vector<double> rho_i;
    body* rhoo_body=nullptr;
    body* rhoi_body=nullptr;
};

#endif // MUSCLE_H
