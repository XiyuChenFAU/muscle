/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "body.h"

using namespace std;

body::body(const std::string& name):
    name(name)
{
    id=-1;
    parent=nullptr;
}

body::~body() {
    delete Shape;
    delete BodyBasic;
}

void body::PrintParameters() {
    // Print parameters if needed
}

shape* body::getshape(){
    return Shape;
}

bodybasic* body::getbodybasic(){
    return BodyBasic;
}

int body::getid(){
    return id;
}

body* body::getparent(){
    return parent;
}

std::vector<body*> body::getchild(){
    return child;
}

std::string body::getname(){
    return name;
}

void body::setid(int idvalue){
    id=idvalue;
}

void body::setparent(body* parentbody){
    if(parent!=parentbody){
        if(parent!=nullptr){
            std::vector<body*> oldparentallchild=parent->getchild();
            for (int i=0;i<oldparentallchild.size();i++) {
                if(oldparentallchild[i]->getname()==name){
                    oldparentallchild.erase(oldparentallchild.begin() + i); 
                    parent->setchildall(oldparentallchild);
                    break;
                }
            }
        }
        parent=parentbody;
        std::vector<body*> parentallchild=parent->getchild();
        int setthischild=1;
        for(int i=0;i<parentallchild.size();i++){
            if(parentallchild[i]==this){
                setthischild=0;
                break;
            }
        }
        if(setthischild){
            parent->setchild(this);
        }
    }
}

void body::setchild(body* childbody){
    child.push_back(childbody);
}

void body::setchildall(const std::vector<body*>& childbody){
    child=childbody;
}

void body::setshape(int addnew){
    if(addnew){
        Shape=new shape();
    }
    
}

void body::setshape(double a, double b, double c, const std::string& shapename, int addnew){
    if(addnew){
        Shape=new shape(a, b, c, shapename);
    }
    else{
        Shape->setshape(a, b, c, shapename);
    }
    
}

void body::setshape(double length, double radius, const std::string& shapename, int addnew){
    if(addnew){
        Shape=new shape(length, radius, shapename);
    }
    else{
        Shape->setshape(length, radius, shapename);
    }
    
}

void body::setbodybasic(const std::vector<double>& q0, int addnew){
    if(addnew){
        BodyBasic=new bodybasic(q0);
    }
    else{
        BodyBasic->setbodybasic(q0);
    }
    
}

void body::setbodybasic(const std::vector<double>& naxisvalueglobal, double rotationanglevalueglobal, const std::vector<double>& positionglobal, int addnew, int global){
    if(addnew){
        BodyBasic=new bodybasic(positionglobal, naxisvalueglobal, rotationanglevalueglobal, parent->getbodybasic());
    }
    else{
        BodyBasic->setbodybasic(positionglobal, naxisvalueglobal, rotationanglevalueglobal, parent->getbodybasic());
    }
}

void body::setbodybasic(const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, int addnew){
    if(addnew){
        BodyBasic=new bodybasic(parent->getbodybasic()->getposition(), parent->getbodybasic()->getaxis(), naxis, rotationangle, rhobody);
    }
    else{
        BodyBasic->setbodybasic(parent->getbodybasic()->getposition(), parent->getbodybasic()->getaxis(), naxis, rotationangle, rhobody);
    }
}

void body::updatebodybasic(){
    BodyBasic->updatebodybasic(parent->getbodybasic()->getposition(), parent->getbodybasic()->getaxis());
}

