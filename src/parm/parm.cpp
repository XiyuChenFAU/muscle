/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "parm.h"

using namespace std;

Parm::Parm() {
    n_bodies=-1;
    n_muscles=0;
    fsolve=new Fsolve();
    variablenum=0;
}

Parm::~Parm() {
    for (body* ptr : allbody) {
        delete ptr;
    }

    for (muscle* ptr : allmuscle) {
        delete ptr;
    }
    delete fsolve;
}

int Parm::getn_bodies(){
    return n_bodies;
}

int Parm::getn_muscles(){
    return n_muscles;
}

std::vector<body*> Parm::getallbody(){
    return allbody;
}

body* Parm::getbodyindex(int index){
    return allbody[index+1];
}

muscle* Parm::getmuscleindex(int index){
    return allmuscle[index];
}

std::vector<muscle*> Parm::getallmuscle(){
    return allmuscle;
}

Fsolve* Parm::getfsolve(){
    return fsolve;
}

void Parm::addbody(body* Body, const std::string& parentbodyname){
    int addnew=1;
    for(int i=0;i<allbody.size();i++){
        if(Body->getname()==allbody[i]->getname()){
            addnew=0;
            body* parentbody=findbody(parentbodyname);
            Body->setparent(parentbody);
            break;
        }
    }
    if(addnew){
        Body->setid(n_bodies);
        body* parentbody=findbody(parentbodyname);
        Body->setparent(parentbody);
        allbody.push_back(Body);
        n_bodies=n_bodies+1;
    }
    
}

void Parm::addbody(const std::string& bodyname, const std::vector<double>& q0){
    int addnew=1;
    for(int i=0;i<allbody.size();i++){
        if(bodyname==allbody[i]->getname()){
            addnew=0;
            allbody[i]->setshape(addnew);
            allbody[i]->setbodybasic(q0,addnew);
            updatechildbody(allbody[i]);
            break;
        }
    }
    if(addnew){
        body* Body=new body(bodyname);
        Body->setshape(addnew);
        Body->setbodybasic(q0,addnew);
        Body->setid(n_bodies);
        allbody.push_back(Body);
        n_bodies=n_bodies+1;
    }
}

void Parm::addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double a, double b, double c, double length, double radius, const std::string& shapename){
    if(a && b && c){
        addbody(bodyname, parentbodyname, naxis, rotationangle, rhobody, a, b, c, shapename);
    }
    else{
        addbody(bodyname, parentbodyname, naxis, rotationangle, rhobody, length, radius, shapename);
    }
}

void Parm::addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double a, double b, double c, const std::string& shapename){
    int addnew=1;
    for(int i=0;i<allbody.size();i++){
        if(bodyname==allbody[i]->getname()){
            addnew=0;
            body* parentbody=findbody(parentbodyname);
            allbody[i]->setparent(parentbody);
            allbody[i]->setshape(a,b,c,shapename,addnew);
            allbody[i]->setbodybasic(naxis, rotationangle, rhobody,addnew);
            updatechildbody(allbody[i]);
            break;
        }
    }
    if(addnew){
        body* Body=new body(bodyname);
        Body->setid(n_bodies);
        body* parentbody=findbody(parentbodyname);
        Body->setparent(parentbody);
        Body->setshape(a,b,c,shapename,addnew);
        Body->setbodybasic(naxis, rotationangle, rhobody,addnew);
        allbody.push_back(Body);
        n_bodies=n_bodies+1;
    }
}

void Parm::addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double length, double radius, const std::string& shapename){
    int addnew=1;
    for(int i=0;i<allbody.size();i++){
        if(bodyname==allbody[i]->getname()){
            addnew=0;
            body* parentbody=findbody(parentbodyname);
            allbody[i]->setparent(parentbody);
            allbody[i]->setshape(length,radius,shapename,addnew);
            allbody[i]->setbodybasic(naxis, rotationangle, rhobody,addnew);
            updatechildbody(allbody[i]);
            break;
        }
    }
    if(addnew){
        body* Body=new body(bodyname);
        Body->setid(n_bodies);
        body* parentbody=findbody(parentbodyname);
        Body->setparent(parentbody);
        Body->setshape(length,radius,shapename,addnew);
        Body->setbodybasic(naxis, rotationangle, rhobody,addnew);
        allbody.push_back(Body);
        n_bodies=n_bodies+1;
    }
    
}

void Parm::updatechildbody(body* Body){
    std::vector<body*> allchild=Body->getchild();
    for(int i=0;i<allchild.size();i++){
        allchild[i]->updatebodybasic();
        updatechildbody(allchild[i]);
    }
}

void Parm::addmuscle(muscle* Muscle){
    int add_muscle=1;
    for(int i=0;i<allmuscle.size();i++){
        if(allmuscle[i]->getname()==Muscle->getname()){
            allmuscle[i]=Muscle;
            add_muscle=0;
            break;
        }
    }
    if(add_muscle){
        allmuscle.push_back(Muscle);
        n_muscles=n_muscles+1;
    }    
}

void Parm::addmuscle(const std::vector<double>& gamma_o, const std::string& rhoo_bodyname, const std::vector<double>& gamma_i, const std::string& rhoi_bodyname, const std::string& name, int nodenum, const std::string& global){
    int add_muscle=1;
    for(int i=0;i<allmuscle.size();i++){
        if(allmuscle[i]->getname()==name){
            allmuscle[i]->setmuscle(allbody, gamma_o, rhoo_bodyname, gamma_i, rhoi_bodyname, name, nodenum, global);
            add_muscle=0;
            break;
        }
    }
    if(add_muscle){
        muscle* Muscle=new muscle(allbody, gamma_o, rhoo_bodyname, gamma_i, rhoi_bodyname, name, nodenum, global);
        allmuscle.push_back(Muscle);
        n_muscles=n_muscles+1;
    }   
}

void Parm::addmuscle(const std::vector<double>& rho_o, const std::string& rhoo_bodyname, const std::vector<double>& rho_i, const std::string& rhoi_bodyname, const std::string& name, int nodenum){
    int add_muscle=1;
    for(int i=0;i<allmuscle.size();i++){
        if(allmuscle[i]->getname()==name){
            allmuscle[i]->setmuscle(allbody, rho_o, rhoo_bodyname, rho_i, rhoi_bodyname, name, nodenum);
            add_muscle=0;
            break;
        }
    }
    if(add_muscle){
        muscle* Muscle=new muscle(allbody, rho_o, rhoo_bodyname, rho_i, rhoi_bodyname, name, nodenum);
        allmuscle.push_back(Muscle);
        n_muscles=n_muscles+1;
    }
}

body* Parm::findbody(const std::string& bodyname){
    for (body* Body : allbody) {
        if(Body->getname()==bodyname){return Body;}
    }
    std::cout<<"not find any body\n";
    return nullptr;
}

int Parm::findbodyindex(const std::string& bodyname){
    int index=-1;
    for (int i=0;i<allbody.size();i++) {
        if(allbody[i]->getname()==bodyname){return i-1;}
    }
    std::cout<<"not find any body\n";
    return  index;
}

void Parm::addmuslcesolution(const std::vector<double>& solution){
    int t=0;
    for(int i=0;i<n_muscles;i++){
        int variablemusclenum=allmuscle[i]->getnodenum()*3+(allmuscle[i]->getnodenum()-2)*n_bodies;
        std::vector<double> solution1;
        for(int j=0;j<variablemusclenum;j++){
            solution1.push_back(solution[t+j]);
        }
        allmuscle[i]->addmuscleparm(solution1);
        t=t+variablemusclenum;
    }
}

void Parm::rotatebodyupdate(const std::string& bodyname, const std::vector<double>& naxis, double rotationangle){
    body* findBody=findbody(bodyname);
    rotatebody(findBody,naxis,rotationangle);
    for(int i=0;i<allbody.size();i++){
        if(allbody[i]->getbodybasic()->getrotatestatus()){
            allbody[i]->getbodybasic()->setrotatestatus(0);
        }
        else{
            allbody[i]->getbodybasic()->norotateaddvalue();
        }
    }
}

void Parm::rotatebody(body* Body, const std::vector<double>& naxis, double rotationangle){
    Body->getbodybasic()->nodalupdate(Body->getparent()->getbodybasic()->getposition(), Body->getparent()->getbodybasic()->getaxis(), naxis, rotationangle);
    Body->getbodybasic()->setrotatestatus(1);
    std::vector<body*> child = Body->getchild();
    if(child.size()>0){
        for(int i=0;i<child.size();i++){
            rotatebody(child[i],naxis,rotationangle);
        }
    }
}

int Parm::getvariable(){
    int vari=0;
    for(int i=0;i<n_muscles;i++){
        vari=vari+allmuscle[i]->getnodenum()*3+(allmuscle[i]->getnodenum()-2)*n_bodies;
    }
    variablenum=vari;
    return vari;
}

void Parm::setallmuscleinitialeta(){
    for(int i=0;i<n_muscles;i++){
        allmuscle[i]->setinitialeta(n_bodies);
    }
}

int Parm::deletebody(const std::string& bodyname){
    int index=-1;
    for (int i=0;allbody.size();i++) {
        if(allbody[i]->getname()==bodyname){
            std::vector<body*> allchild =allbody[i]->getchild();
            for(int j=0;j<allchild.size();j++){
                allchild[j]->setparent(allbody[i]->getparent());
            }
            delete allbody[i]; 
            allbody.erase(allbody.begin() + i);
            n_bodies=n_bodies-1;
            index=i-1;
            break;
        }
    }
    return index;
}

int Parm::deletemuscle(const std::string& musclename){
    int index=-1;
    for (int i=0;i<allmuscle.size();i++) {
        if(allmuscle[i]->getname()==musclename){
            delete allmuscle[i]; 
            allmuscle.erase(allmuscle.begin() + i); 
            index=i;
            n_muscles=n_muscles-1;
            break;
        }
    }
    return index;
}

void Parm::resetallforrecalc(){
    for(int i=0;i<allbody.size();i++){
        allbody[i]->getbodybasic()->resetforrecalc();
    }
    for(int i=0;i<n_muscles;i++){
        allmuscle[i]->resetforrecalc();
    }
}
