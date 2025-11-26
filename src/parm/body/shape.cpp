/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "shape.h"

using namespace std;

std::vector<std::string> shape::allshape={"ellipsoid","cylinder","torus"};

shape::shape (){
    a=0;
    b=0;
    c=0;
    length=0;
    radius=0;
    shapename="";
    //PrintParameters();;
}

shape::shape (double avalue, double bvalue, double cvalue, const std::string& shapenamevalue){
    setshape(avalue, bvalue, cvalue, shapenamevalue);
    //PrintParameters();;
}

shape::shape (double lengthvalue, double radiusvalue, const std::string& shapenamevalue){
    setshape(lengthvalue, radiusvalue, shapenamevalue);
    //PrintParameters();
}

shape::~shape() {
    // Destructor
}

void shape::setshape(double avalue, double bvalue, double cvalue, const std::string& shapenamevalue){
    a=avalue;
    b=bvalue;
    c=cvalue;
    shapename=shapenamevalue;
    length=c*2;
    radius=(a+b)/2;
}

void shape::setshape(double lengthvalue, double radiusvalue, const std::string& shapenamevalue){
    length=lengthvalue;
    radius=radiusvalue;
    shapename=shapenamevalue;
    a=radius;
    b=radius;
    c=length/2;
}

void shape::PrintParameters() {
    std::cout << "shape a: " << a << std::endl;
    std::cout << "shape b: " << b << std::endl;
    std::cout << "shape c: " << c << std::endl;
    std::cout << "shape length: " << length << std::endl;
    std::cout << "shape radius: " << radius << std::endl;
}

double shape::getlength() {
    return length;
}

double shape::getradius() {
    return radius;
}

double shape::geta(){
    return a;
}

double shape::getb(){
    return b;
}

double shape::getc(){
    return c;
}

std::string shape::getshapename(){
    return shapename;
}

int shape::getshapeindex(const std::string& shapename){
    int index=-1;
    for(int i=0;i<allshape.size();i++){
        if(allshape[i]==shapename){
            index=i;
            break;
        }
    }
    return index;
}

