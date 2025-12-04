/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include <iostream>
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
        // new torus
        if(allbody[i+1]->getshape()->getshapename()=="torus"){
            std::vector<SX> Gbody = Jacobiantorus(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
        // new capsule
        if(allbody[i+1]->getshape()->getshapename()=="capsule"){
            std::vector<SX> Gbody = Jacobiancapsule(gamma,allbody[i+1]);
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


    for (int i = 0; i < 3; i++) {
        SX G1 = 2.0 * ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) * axis[0][i] +
            (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) * axis[1][i]);
        SX surfacejacobian = 2.0 * ((x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) * axis[0][i] +
            (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) * axis[1][i] +
            (fabs(x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2])-(Body->getshape()->geta()+Body->getshape()->getb())/2) / ((Body->getshape()->geta()+Body->getshape()->getb())/2*(Body->getshape()->geta()+Body->getshape()->getb())/2) * axis[2][i] );
        G[i] = binary_condition*G1+(1-binary_condition)*surfacejacobian;
    }
    return G;
}

// new torus test
std::vector<SX> constraintshape::Jacobiantorus(const std::vector<SX> &gamma, body *Body)
{   
    std::cout << "using Jacobian torus" << std::endl;
    // /*
    // last body-offset (timestep)
    std::vector<std::vector<double>> qall = Body->getbodybasic()->getq();
    std::vector<double> q = qall[qall.size()-1];

    // x = gamma - q  (SX)
    std::vector<SX> x(3);
    for (int i = 0; i < 3; ++i) {
        x[i] = gamma[i] - q[i];
    }

    // local axis
    std::vector<std::vector<double>> axis = Body->getbodybasic()->getaxis();
    // axis[0], axis[1], axis[2] je vector<double> mit 3 Komponenten

    // Major/Minor Radii (R=a, r=b)
    double R = Body->getshape()->geta(); // Major radius
    double r = Body->getshape()->getb(); // Minor radius

    // projections c0,c1,c2 (in SX)
    SX c0 = x[0]*axis[0][0] + x[1]*axis[0][1] + x[2]*axis[0][2];
    SX c1 = x[0]*axis[1][0] + x[1]*axis[1][1] + x[2]*axis[1][2];
    SX c2 = x[0]*axis[2][0] + x[1]*axis[2][1] + x[2]*axis[2][2];

    // help-variables S und T
    SX S = c0*c0 + c1*c1 + c2*c2 + SX(R*R - r*r); // S = sum squares + R^2 - r^2
    SX T = c0*c0 + c1*c1;

    // partial derivation wrt c_j
    //    dF/dc0 = 4 * c0 * (S - 2 R^2)
    //    dF/dc1 = 4 * c1 * (S - 2 R^2)
    //    dF/dc2 = 4 * c2 * S
    SX common = S - SX(2.0 * R * R);
    SX dF_dc0 = SX(4.0) * c0 * common;
    SX dF_dc1 = SX(4.0) * c1 * common;
    SX dF_dc2 = SX(4.0) * c2 * S;

    // derivation -> gamma: sum_j (dF/dc_j) * a_j  (a_j are axis[j])
    std::vector<SX> G(3);
    for (int i = 0; i < 3; ++i) {
        // Komponente i (x,y,z)
        G[i] = dF_dc0 * SX(axis[0][i])
             + dF_dc1 * SX(axis[1][i])
             + dF_dc2 * SX(axis[2][i]);
    }

    return G;
    // */

    ///////////////////////////////////
    // scalable version
    /*
    std::vector<std::vector<double>> qall = Body->getbodybasic()->getq();
    std::vector<double> q = qall[qall.size()-1];

    std::vector<SX> x(3);
    for (int i = 0; i < 3; ++i) {
        x[i] = gamma[i] - q[i];
    }
    
    std::vector<std::vector<double>> axis = Body->getbodybasic()->getaxis();
    
    double R = Body->getshape()->geta(); // Major radius
    double r = Body->getshape()->getb(); // Minor radius

    std::vector<double> scale = {1.0, 1.0, 1.0};// Body->getshape()->getc()}; // <-- ersetzen falls nötig
    
    SX c0 = x[0]*axis[0][0] + x[1]*axis[0][1] + x[2]*axis[0][2]; // x·a0
    SX c1 = x[0]*axis[1][0] + x[1]*axis[1][1] + x[2]*axis[1][2]; // x·a1
    SX c2 = x[0]*axis[2][0] + x[1]*axis[2][1] + x[2]*axis[2][2]; // x·a2

    // avoid division by zero: convert scales to SX and maybe clamp small values
    SX s0 = SX(scale[0]);
    SX s1 = SX(scale[1]);
    SX s2 = SX(scale[2]);

    // u_j = c_j / s_j
    SX u0 = c0 / s0;
    SX u1 = c1 / s1;
    SX u2 = c2 / s2;

    SX Su = u0*u0 + u1*u1 + u2*u2 + SX(R*R - r*r); // S_u

    SX common = Su - SX(2.0 * R * R);
    SX dF_du0 = SX(4.0) * u0 * common;
    SX dF_du1 = SX(4.0) * u1 * common;
    SX dF_du2 = SX(4.0) * u2 * Su;

    // 9) chain rule: du_j/dγ = a_j / s_j  -> G = sum_j dF_duj * (a_j / s_j)
    std::vector<SX> G(3);
    for (int i = 0; i < 3; ++i) {
        G[i] = dF_du0 * (SX(axis[0][i]) / s0)
             + dF_du1 * (SX(axis[1][i]) / s1)
             + dF_du2 * (SX(axis[2][i]) / s2);
    }
    return G;
    */
}

// new capsule test
std::vector<SX> constraintshape::Jacobiancapsule(const std::vector<SX> &gamma, body *Body)
{
    std::vector<std::vector<double>> qall = Body->getbodybasic()->getq();
    std::vector<double> q = qall[qall.size()-1];

    std::vector<SX> x(3);
    for(int i=0;i<3;i++){
        x[i] = gamma[i] - q[i];
    }

    std::vector<std::vector<double>> axis = Body->getbodybasic()->getaxis();
    SX x_x = x[0]*axis[0][0] + x[1]*axis[0][1] + x[2]*axis[0][2];
    SX x_y = x[0]*axis[1][0] + x[1]*axis[1][1] + x[2]*axis[1][2];
    SX x_z = x[0]*axis[2][0] + x[1]*axis[2][1] + x[2]*axis[2][2];

    double L = Body->getshape()->getlength();  // total capsule length
    double r = Body->getshape()->getradius();  // radius
    double Lhalf = L/2.0 - r;

    SX insideCylinder = if_else(fabs(x_z) <= Lhalf, 1.0, 0.0);

    std::vector<SX> G(3);

    for(int i=0;i<3;i++)
    {
        SX ex = axis[0][i];
        SX ey = axis[1][i];
        SX ez = axis[2][i];

        // Cylinder gradient
        SX Gcyl = 2.0*( x_x*ex + x_y*ey ) / (r*r);

        // Sphere gradients
        SX Gtop    = 2.0*( x_x*ex + x_y*ey + (x_z - Lhalf)*ez ) / (r*r);
        SX Gbottom = 2.0*( x_x*ex + x_y*ey + (x_z + Lhalf)*ez ) / (r*r);

        // Choose correct hemisphere
        SX Gcap = if_else(Gtop < Gbottom, Gtop, Gbottom);

        // Result Jacobian
        G[i] = insideCylinder * Gcyl + (1 - insideCylinder) * Gcap;
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
        // new torus
        if(allbody[i+1]->getshape()->getshapename()=="torus"){
            SX Gbody = constraint_torus(gamma,allbody[i+1]);
            G.push_back(Gbody);
        }
        // new capsule
        if(allbody[i+1]->getshape()->getshapename()=="capsule"){
            SX Gbody = constraint_capsule(gamma,allbody[i+1]);
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

    SX surface = (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) * (x[0] * axis[0][0] + x[1] * axis[0][1]+ x[2] * axis[0][2]) / (Body->getshape()->geta()*Body->getshape()->geta()) +
        (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) * (x[0] * axis[1][0] + x[1] * axis[1][1]+ x[2] * axis[1][2]) / (Body->getshape()->getb()*Body->getshape()->getb()) +
        (fabs(x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2])-(Body->getshape()->geta()+Body->getshape()->getb())/2) * (fabs(x[0] * axis[2][0] + x[1] * axis[2][1]+ x[2] * axis[2][2])-(Body->getshape()->geta()+Body->getshape()->getb())/2) / ((Body->getshape()->geta()+Body->getshape()->getb())/2*(Body->getshape()->geta()+Body->getshape()->getb())/2) - 1;

    SX G = binary_condition*G1+(1-binary_condition)*1;
    return G;
}


// new torus test
SX constraintshape::constraint_torus(const std::vector<SX> &gamma, body *Body)
{
    std::cout << "using constraint torus" << std::endl;
    // body position
    std::vector<std::vector<double>> qall = Body->getbodybasic()->getq();
    std::vector<double> q = qall.back();

    // global coords
    std::vector<SX> X(3);
    for (int i = 0; i < 3; i++) {
        X[i] = gamma[i] - q[i]; // are these global reference?
    }

    // local axis
    std::vector<std::vector<double>> axis = Body->getbodybasic()->getaxis();

    // Projektion auf lokale Achsen
    SX x = X[0] * axis[0][0] + X[1] * axis[0][1] + X[2] * axis[0][2];
    SX y = X[0] * axis[1][0] + X[1] * axis[1][1] + X[2] * axis[1][2];
    SX z = X[0] * axis[2][0] + X[1] * axis[2][1] + X[2] * axis[2][2];

    // Torus-Parameter
    double R = Body->getshape()->geta();   // Major radius
    double r = Body->getshape()->getb();   // Minor radius

    // local sclae
    double sx = 1; 
    double sy = 1;
    double sz = 1; // Body->getshape()->getc();

    // scale coords
    SX xs = x / sx;
    SX ys = y / sy;
    SX zs = z / sz;

    // Torus surface distance function
    // S and T
    SX S = x*x + y*y + z*z + (R*R - r*r);
    SX T = x*x + y*y;

    // phi = S^2 - 4 a^2 T
    SX phi = S*S - 4.0 * R * R * T;
    /*
    SX radial = sqrt(xs*xs + ys*ys);
    SX phi = (radial - R) * (radial - R) + zs*zs - r*r;
    */

    return phi;
}

// new caspule test
SX constraintshape::constraint_capsule(const std::vector<SX> &gamma, body *Body)
{
    std::vector<std::vector<double>> qall = Body->getbodybasic()->getq();
    std::vector<double> q = qall[qall.size()-1];

    std::vector<SX> x(3);
    for(int i=0;i<3;i++){
        x[i] = gamma[i] - q[i];
    }

    std::vector<std::vector<double>> axis = Body->getbodybasic()->getaxis();
    SX x_x = x[0]*axis[0][0] + x[1]*axis[0][1] + x[2]*axis[0][2];
    SX x_y = x[0]*axis[1][0] + x[1]*axis[1][1] + x[2]*axis[1][2];
    SX x_z = x[0]*axis[2][0] + x[1]*axis[2][1] + x[2]*axis[2][2];

    double L = Body->getshape()->getlength();  // total capsule length L
    double r = Body->getshape()->getradius();  // radius

    double Lhalf = L/2.0 - r;

    SX insideCylinder = if_else(fabs(x_z) <= Lhalf, 1.0, 0.0);

    SX Gcyl = (x_x*x_x + x_y*x_y)/(r*r) - 1.0;

    SX Gtop    = (x_x*x_x + x_y*x_y + (x_z - Lhalf)*(x_z - Lhalf)) / (r*r) - 1.0;
    SX Gbottom = (x_x*x_x + x_y*x_y + (x_z + Lhalf)*(x_z + Lhalf)) / (r*r) - 1.0;

    SX Gcap = if_else(Gtop < Gbottom, Gtop, Gbottom);

    SX G = insideCylinder * Gcyl + (1 - insideCylinder) * Gcap;

    return G;
}