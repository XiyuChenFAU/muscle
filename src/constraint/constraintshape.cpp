/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "constraintshape.h"

constraintshape::constraintshape(){

}

std::vector<std::vector<SX>> constraintshape::Jacobianshape(const std::vector<SX>& gamma, Parm* parm){
    std::vector<std::vector<SX>> G;
    std::vector<body*> allbody=parm->getallbody();
    for(int i=0;i<parm->getn_bodies();i++){
        if(allbody[i+1]->getshape()->getshapename()=="ellipsoid"){
            std::vector<SX> Gbody = Jacobianellipsoid(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
        if(allbody[i+1]->getshape()->getshapename()=="cylinder"){
            std::vector<SX> Gbody = Jacobiancylinder(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
    }
    return G;
}

std::vector<SX> constraintshape::Jacobianellipsoid(const std::vector<SX>& gamma, body* Body){
    std::vector<std::vector<double>>  qall=Body->getbodybasic()->getq();
    std::vector<double>q = qall[qall.size()-1];

    std::vector<SX> x;

    for (int i = 0; i < 3; i++) {
        x.push_back(gamma[i] - q[i]);
    }

    std::vector<SX> G(3);
    std::vector<std::vector<double>> axis=Body->getbodybasic()->getaxis();

    for (int i = 0; i < 3; i++) {
        G[i] = 2.0 * ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) * axis[0][i] +
            (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) * axis[1][i] +
            (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) / (Body->getshape()->getc()*Body->getshape()->getc()) * axis[2][i] );
    }
    return G;
}

std::vector<SX> constraintshape::Jacobiancylinder(const std::vector<SX>& gamma, body* Body){
    std::vector<std::vector<double>>  qall=Body->getbodybasic()->getq();
    std::vector<double>q = qall[qall.size()-1];

    std::vector<SX> x;

    for (int i = 0; i < 3; i++) {
        x.push_back(gamma[i] - q[i]);
    }

    std::vector<SX> G(3);
    std::vector<std::vector<double>> axis=Body->getbodybasic()->getaxis();
    SX binary_condition = if_else(fabs(x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) <= Body->getshape()->getc(), 1.0, 0.0);

    std::vector<double> surfacejacobian={0.0,0.0,axis[0][2]};

    for (int i = 0; i < 3; i++) {
        SX G1 = 2.0 * ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) * axis[0][i] +
            (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) * axis[1][i]);
        G[i] = binary_condition*G1+(1-binary_condition)*surfacejacobian[i];
    }
    return G;
}

std::vector<SX> constraintshape::constraint_shape(const std::vector<SX>& gamma, Parm* parm){
    std::vector<SX> G;
    std::vector<body*> allbody=parm->getallbody();
    for(int i=0;i<parm->getn_bodies();i++){
        if(allbody[i+1]->getshape()->getshapename()=="ellipsoid"){
            SX Gbody = constraint_ellipsoid(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
        if(allbody[i+1]->getshape()->getshapename()=="cylinder"){
            SX Gbody = constraint_cylinder(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
    }
    return G;
}

SX constraintshape::constraint_ellipsoid(const std::vector<SX>& gamma, body* Body){
    std::vector<std::vector<double>>  qall=Body->getbodybasic()->getq();
    std::vector<double>q = qall[qall.size()-1];

    std::vector<SX> x;

    for (int i = 0; i < 3; i++) {
        x.push_back(gamma[i] - q[i]);
    }

    std::vector<std::vector<double>> axis=Body->getbodybasic()->getaxis();


    SX G = (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) +
        (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) +
        (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) * (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) / (Body->getshape()->getc()*Body->getshape()->getc()) - 1;
    return G;
}

SX constraintshape::constraint_cylinder(const std::vector<SX>& gamma, body* Body){
    std::vector<std::vector<double>>  qall=Body->getbodybasic()->getq();
    std::vector<double>q = qall[qall.size()-1];

    std::vector<SX> x;

    for (int i = 0; i < 3; i++) {
        x.push_back(gamma[i] - q[i]);
    }

    std::vector<std::vector<double>> axis=Body->getbodybasic()->getaxis();

    SX binary_condition = if_else(fabs(x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) <= Body->getshape()->getc(), 1.0, 0.0);


    SX G1 = (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) +
        (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) - 1;

    SX surface = fabs(x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) - Body->getshape()->getc();

    SX G = binary_condition*G1+(1-binary_condition)*1;
    return G;
}

