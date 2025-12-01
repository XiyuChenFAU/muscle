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
    n_joints=0;
    fsolve=new Fsolve();
    variablenum=0;
    run_total_step=0;
}

Parm::~Parm() {
    for (body* ptr : allbody) {
        delete ptr;
    }

    for (muscle* ptr : allmuscle) {
        delete ptr;
    }

    for (joint* ptr : alljoint) {
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

int Parm::getn_joints(){
    return n_joints;
}

int Parm::get_write_moment_joints(){
    int write_moment_joints_nr=0;
    for(int i=0;i<alljoint.size();i++){
        if(alljoint[i]->getwritemomentarm()){
            write_moment_joints_nr+=1;
        }
    }
    return write_moment_joints_nr;
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

std::vector<joint*> Parm::getalljoint(){
    return alljoint;
}

std::vector<int> Parm::getallstep(){
    std::vector<int> all_step={};
    for(int i=0; i<run_total_step+1;i++){all_step.push_back(i);}
    return all_step;
}
    
joint* Parm::getjointindex(int index){
    joint* Joint=nullptr;
    if(index<alljoint.size()){Joint=alljoint[index];}
    return Joint;
}

std::vector<muscle*> Parm::getallmuscle(){
    return allmuscle;
}

Fsolve* Parm::getfsolve(){
    return fsolve;
}

int Parm::get_run_total_step(){
    return run_total_step;
}

void Parm::set_run_total_step(int run_total_step_value){
    run_total_step=run_total_step_value;
}

void Parm::set_body_R_initial(){
    for(int i=0;i<allbody.size();i++){
        allbody[i]->getbodybasic()->set_R({{1,0,0},{0,1,0},{0,0,1}});
    }
}

void Parm::check_body_R(){
    for(int i=0;i<allbody.size();i++){
        if(!allbody[i]->getbodybasic()->check_R_value()){
            std::cout<<allbody[i]->getname()<<" not same #################\n";
        }
    }
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

void Parm::addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double a, double b, double c, double length, double radius, const std::string& shapename,int global){
    if(a && b && c){
        addbody(bodyname, parentbodyname, naxis, rotationangle, rhobody, a, b, c, shapename, global);
    }
    else{
        addbody(bodyname, parentbodyname, naxis, rotationangle, rhobody, length, radius, shapename, global);
    }
}

void Parm::addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double a, double b, double c, const std::string& shapename,int global){
    int addnew=1;
    for(int i=0;i<allbody.size();i++){
        if(bodyname==allbody[i]->getname()){
            addnew=0;
            body* parentbody=findbody(parentbodyname);
            allbody[i]->setparent(parentbody);
            allbody[i]->setshape(a,b,c,shapename,addnew);
            if(global){
                allbody[i]->setbodybasic(naxis, rotationangle, rhobody,addnew,global);
            }
            else{
                allbody[i]->setbodybasic(naxis, rotationangle, rhobody,addnew);
            }
            updatechildbody(allbody[i]);
            break;
        }
    }
    if(addnew){
        std::cout << "Adding new body("<< shapename << ") with shapes: " << a << ", " << b << ", " << c << std::endl;
        body* Body=new body(bodyname);
        Body->setid(n_bodies);
        body* parentbody=findbody(parentbodyname);
        Body->setparent(parentbody);
        Body->setshape(a,b,c,shapename,addnew);
        if(global){
            Body->setbodybasic(naxis, rotationangle, rhobody,addnew,global);
        }
        else{
            Body->setbodybasic(naxis, rotationangle, rhobody,addnew);
        }
        allbody.push_back(Body);
        n_bodies=n_bodies+1;
    }
}

void Parm::addbody(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double length, double radius, const std::string& shapename,int global){
    int addnew=1;
    for(int i=0;i<allbody.size();i++){
        if(bodyname==allbody[i]->getname()){
            addnew=0;
            body* parentbody=findbody(parentbodyname);
            allbody[i]->setparent(parentbody);
            allbody[i]->setshape(length,radius,shapename,addnew);
            if(global){
                allbody[i]->setbodybasic(naxis, rotationangle, rhobody,addnew,global);
            }
            else{
                allbody[i]->setbodybasic(naxis, rotationangle, rhobody,addnew);
            }
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
        if(global){
            Body->setbodybasic(naxis, rotationangle, rhobody,addnew,global);
        }
        else{
            Body->setbodybasic(naxis, rotationangle, rhobody,addnew);
        }
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

void Parm::addmuscle(const std::vector<double>& gamma_o, const std::string& rhoo_bodyname, const std::vector<double>& gamma_i, const std::string& rhoi_bodyname, const std::string& name, int nodenum, int global){
    int add_muscle=1;
    for(int i=0;i<allmuscle.size();i++){
        if(allmuscle[i]->getname()==name){
            if(global){
                allmuscle[i]->setmuscle(allbody, gamma_o, rhoo_bodyname, gamma_i, rhoi_bodyname, name, nodenum, global);
            }
            else{
                allmuscle[i]->setmuscle(allbody, gamma_o, rhoo_bodyname, gamma_i, rhoi_bodyname, name, nodenum);
            }
            add_muscle=0;
            break;
        }
    }
    if(add_muscle){
        muscle* Muscle=nullptr;
        if(global){
            Muscle=new muscle(allbody, gamma_o, rhoo_bodyname, gamma_i, rhoi_bodyname, name, nodenum, global);
        }
        else{
            Muscle=new muscle(allbody, gamma_o, rhoo_bodyname, gamma_i, rhoi_bodyname, name, nodenum);
        }
        allmuscle.push_back(Muscle);
        n_muscles=n_muscles+1;
    }   
}

void Parm::addmuscle(const std::vector<double>& gamma_o, const std::string& rhoo_bodyname, const std::vector<double>& gamma_i, const std::string& rhoi_bodyname, const std::string& name, int nodenum, int global, const std::vector<double>& gammavalue, const std::vector<double>& etavalue){
    int add_muscle=1;
    for(int i=0;i<allmuscle.size();i++){
        if(allmuscle[i]->getname()==name){
            if(global){
                allmuscle[i]->setmuscle(allbody, gamma_o, rhoo_bodyname, gamma_i, rhoi_bodyname, name, nodenum, global, gammavalue, etavalue);
            }
            else{
                allmuscle[i]->setmuscle(allbody, gamma_o, rhoo_bodyname, gamma_i, rhoi_bodyname, name, nodenum, gammavalue, etavalue);
            }
            add_muscle=0;
            break;
        }
    }
    if(add_muscle){
        muscle* Muscle=nullptr;
        if(global){
            Muscle=new muscle(allbody, gamma_o, rhoo_bodyname, gamma_i, rhoi_bodyname, name, nodenum, global, gammavalue, etavalue);
        }
        else{
            Muscle=new muscle(allbody, gamma_o, rhoo_bodyname, gamma_i, rhoi_bodyname, name, nodenum, gammavalue, etavalue);
        }
        allmuscle.push_back(Muscle);
        n_muscles=n_muscles+1;
    }   
}

void Parm::set_single_read_muscle_value(const std::string& name, int read_muscle_value){
    for(int i=0;i<allmuscle.size();i++){
        if(allmuscle[i]->getname()==name){
            allmuscle[i]->set_read_muscle_value(read_muscle_value);
            break;
        }
    }
}
    
void Parm::addjoint(const std::string& namevalue, const std::string& bodynamevalue, const std::string& joint_typevalue, const std::vector<double>& relative_posvalue, const std::vector<double>& axisvectorvalue, const std::vector<std::vector<std::vector<double>>>& move_setting_value, const std::vector<std::vector<double>>& movement_value){
    body* currentbodyvalue=findbody(bodynamevalue);
    int add_joint=1;
    for(int i=0;i<alljoint.size();i++){
        if(alljoint[i]->getname() ==namevalue){
            alljoint[i]->setjoint(namevalue, bodynamevalue, currentbodyvalue, joint_typevalue, relative_posvalue, axisvectorvalue, move_setting_value, movement_value);
            if(run_total_step<alljoint[i]->getjoint_stepnum()){run_total_step=alljoint[i]->getjoint_stepnum();}
            if(!movement_value.empty()){alljoint[i]->setread_from_movement(1);} else{alljoint[i]->setread_from_movement(0);}
            add_joint=0;
            break;
        }
    }
    if(add_joint){
        joint* Joint=new joint(namevalue, bodynamevalue, currentbodyvalue, joint_typevalue, relative_posvalue, axisvectorvalue, move_setting_value, movement_value);
        if(run_total_step<Joint->getjoint_stepnum()){run_total_step=Joint->getjoint_stepnum();}
        if(!movement_value.empty()){Joint->setread_from_movement(1);} else{Joint->setread_from_movement(0);}
        alljoint.push_back(Joint);
        n_joints=n_joints+1;
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

void Parm::rotatebodyupdate(int currentstepnum){
    for(int i=0;i<alljoint.size();i++){
        alljoint[i]->updateall(currentstepnum);
    }
    for(int i=0;i<allbody.size();i++){
        if(allbody[i]->getbodybasic()->getrotatestatus()){
            allbody[i]->getbodybasic()->rotateaddvalue();
        }
        else{
            allbody[i]->getbodybasic()->norotateaddvalue();
        }
    }
    for(int i=0;i<alljoint.size();i++){
        alljoint[i]->absolute_pos_axis_update(currentstepnum);
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

void Parm::setallmuscleinitialeta_gamma(){
    for(int i=0;i<n_muscles;i++){
        allmuscle[i]->setinitialeta_gamma(allbody);
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

int Parm::deletejoint(const std::string& jointname){
    int index=-1;
    for (int i=0;i<alljoint.size();i++) {
        if(alljoint[i]->getname()==jointname){
            delete alljoint[i]; 
            alljoint.erase(alljoint.begin() + i); 
            index=i;
            n_joints=n_joints-1;
            int total_step=0;
            for(int i=0;i<n_joints;i++){
                if(total_step<alljoint[i]->getjoint_stepnum()){total_step=alljoint[i]->getjoint_stepnum();}
            }
            run_total_step=total_step;
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
    for(int i=0;i<alljoint.size();i++){
        alljoint[i]->resetforrecalc();
    }
}