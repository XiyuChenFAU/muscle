/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include <iostream>
#include "constraintshape.h"


constraintshape::constraintshape(){

}


std::vector<std::vector<MX>> constraintshape::Jacobianshape(const std::vector<MX>& gamma, Parm* parm){
    std::vector<std::vector<MX>> G;
    std::vector<body*> allbody=parm->getallbody();
    for(int i=0;i<parm->getn_bodies();i++){
        if(allbody[i+1]->getshape()->getshapename()=="ellipsoid"){
            std::vector<MX> Gbody = Jacobianellipsoid(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
        if(allbody[i+1]->getshape()->getshapename()=="cylinder"){
            std::vector<MX> Gbody = Jacobiancylinder(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
        // new torus
        if(allbody[i+1]->getshape()->getshapename()=="torus"){
            std::vector<MX> Gbody = Jacobiantorus(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
    }
    return G;
}

std::vector<MX> constraintshape::Jacobianellipsoid(const std::vector<MX>& gamma, body* Body){
    std::vector<std::vector<double>>  qall=Body->getbodybasic()->getq();
    std::vector<double>q = qall[qall.size()-1];

    std::vector<MX> x;

    for (int i = 0; i < 3; i++) {
        x.push_back(gamma[i] - q[i]);
    }

    std::vector<MX> G(3);
    std::vector<std::vector<double>> axis=Body->getbodybasic()->getaxis();

    for (int i = 0; i < 3; i++) {
        G[i] = 2.0 * ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) * axis[0][i] +
            (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) * axis[1][i] +
            (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) / (Body->getshape()->getc()*Body->getshape()->getc()) * axis[2][i] );
    }
    return G;
}

std::vector<MX> constraintshape::Jacobiancylinder(const std::vector<MX>& gamma, body* Body){
    std::vector<std::vector<double>>  qall=Body->getbodybasic()->getq();
    std::vector<double>q = qall[qall.size()-1];

    std::vector<MX> x;

    for (int i = 0; i < 3; i++) {
        x.push_back(gamma[i] - q[i]);
    }

    std::vector<MX> G(3);
    std::vector<std::vector<double>> axis=Body->getbodybasic()->getaxis();
    MX binary_condition = if_else(fabs(x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) <= Body->getshape()->getc(), 1.0, 0.0);


    for (int i = 0; i < 3; i++) {
        MX G1 = 2.0 * ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) * axis[0][i] +
            (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) * axis[1][i]);
        MX surfacejacobian = 2.0 * ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) * axis[0][i] +
            (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) * axis[1][i] +
            (fabs(x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2])-(Body->getshape()->geta()+Body->getshape()->getb())/2) / ((Body->getshape()->geta()+Body->getshape()->getb())/2*(Body->getshape()->geta()+Body->getshape()->getb())/2) * axis[2][i] );
        G[i] = binary_condition*G1+(1-binary_condition)*surfacejacobian;
    }
    return G;
}

// new torus test
std::vector<MX> constraintshape::Jacobiantorus(const std::vector<MX> &gamma, body *Body)
{   
    std::cout << "using Jacobian torus" << std::endl;

    std::vector<std::vector<double>>  qall=Body->getbodybasic()->getq();
    std::vector<double>q = qall[qall.size()-1];

    std::vector<MX> x;

    for (int i = 0; i < 3; i++) {
        x.push_back(gamma[i] - q[i]);
    }

    std::vector<MX> G(3);
    std::vector<std::vector<double>> axis=Body->getbodybasic()->getaxis();

    for (int i = 0; i < 3; i++) {
        G[i] = 2 * ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) + (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) + (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) * (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) +
            Body->getshape()->geta() * Body->getshape()->geta() - Body->getshape()->getb() * Body->getshape()->getb()) * (2 * (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * axis[0][i] + 2 * (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * axis[1][i] + 2 * (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) * axis[2][i]) -
            8 * Body->getshape()->geta() * Body->getshape()->geta() * ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * axis[0][i] + (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * axis[1][i]);
    }
    return G;
}



std::vector<MX> constraintshape::constraint_shape(const std::vector<MX>& gamma, Parm* parm){
    std::vector<MX> G;
    std::vector<body*> allbody=parm->getallbody();
    for(int i=0;i<parm->getn_bodies();i++){
        if(allbody[i+1]->getshape()->getshapename()=="ellipsoid"){
            MX Gbody = constraint_ellipsoid(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
        if(allbody[i+1]->getshape()->getshapename()=="cylinder"){
            MX Gbody = constraint_cylinder(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
        // new torus
        if(allbody[i+1]->getshape()->getshapename()=="torus"){
            MX Gbody = constraint_torus(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
    }
    return G;
}

MX constraintshape::constraint_ellipsoid(const std::vector<MX>& gamma, body* Body){
    std::vector<std::vector<double>>  qall=Body->getbodybasic()->getq();
    std::vector<double>q = qall[qall.size()-1];

    std::vector<MX> x;

    for (int i = 0; i < 3; i++) {
        x.push_back(gamma[i] - q[i]);
    }

    std::vector<std::vector<double>> axis=Body->getbodybasic()->getaxis();


    MX G = (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) +
        (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) +
        (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) * (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) / (Body->getshape()->getc()*Body->getshape()->getc()) - 1;
    return G;
}

MX constraintshape::constraint_cylinder(const std::vector<MX>& gamma, body* Body){
    std::vector<std::vector<double>>  qall=Body->getbodybasic()->getq();
    std::vector<double>q = qall[qall.size()-1];

    std::vector<MX> x;

    for (int i = 0; i < 3; i++) {
        x.push_back(gamma[i] - q[i]);
    }

    std::vector<std::vector<double>> axis=Body->getbodybasic()->getaxis();

    MX binary_condition = if_else(fabs(x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) <= Body->getshape()->getc(), 1.0, 0.0);


    MX G1 = (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) +
        (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) - 1;

    MX surface = (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) +
        (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) +
        (fabs(x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2])-(Body->getshape()->geta()+Body->getshape()->getb())/2) * (fabs(x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2])-(Body->getshape()->geta()+Body->getshape()->getb())/2) / ((Body->getshape()->geta()+Body->getshape()->getb())/2*(Body->getshape()->geta()+Body->getshape()->getb())/2) - 1;

    MX G = binary_condition*G1+(1-binary_condition)*1;
    return G;
}


// new torus test
MX constraintshape::constraint_torus(const std::vector<MX> &gamma, body *Body)
{
    std::cout << "using constraint torus" << std::endl;
    // body position
    std::vector<std::vector<double>> qall = Body->getbodybasic()->getq();
    std::vector<double> q = qall.back();

    // global coords
    std::vector<MX> x;
    for (int i = 0; i < 3; i++) {
        x.push_back(gamma[i] - q[i]);
    }

    // local axis
    std::vector<std::vector<double>> axis = Body->getbodybasic()->getaxis();

    double R = Body->getshape()->geta();   // Major radius
    double r = Body->getshape()->getb();   // Minor radius

    MX G = ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) + (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) + (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) * (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) +
        Body->getshape()->geta() * Body->getshape()->geta() - Body->getshape()->getb() * Body->getshape()->getb()) * ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) + (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) + (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) * (x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2]) +
        Body->getshape()->geta() * Body->getshape()->geta() - Body->getshape()->getb() * Body->getshape()->getb()) - 4.0 * Body->getshape()->geta() * Body->getshape()->geta() * ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) + (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]));
    return G;
}
