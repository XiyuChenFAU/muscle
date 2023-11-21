/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "body_basic.h"

using namespace std;

std::vector<std::string> bodybasic::alltype={"local","global"};

bodybasic::bodybasic(const std::vector<double>& q0){
    setbodybasic(q0);
    //PrintParameters(q0);
}

bodybasic::bodybasic(const std::vector<double>& positionglobal, const std::vector<double>& naxisvalueglobal, double rotationanglevalueglobal, bodybasic* parentbodybasic){
    setbodybasic(positionglobal, naxisvalueglobal, rotationanglevalueglobal, parentbodybasic);
    //PrintParameters(q0);
}

bodybasic::bodybasic(const std::vector<double>& lastbodyposition, const std::vector<std::vector<double>>& lastbodyaxis, const std::vector<double>& naxisvalue, double rotationanglevalue, const std::vector<double>& rhobodyvalue){
    setbodybasic(lastbodyposition, lastbodyaxis, naxisvalue, rotationanglevalue, rhobodyvalue);
}

void bodybasic::setbodybasic(const std::vector<double>& q0){
    initialsetting_naxis = {0.0, 1.0, 0.0};
    initialsetting_angle = 0;
    setpoistionaxis(q0);
    rhobody = {0.0, 0.0, 0.0};
    if(q.empty()){
        q.push_back(q0);
    }
    else{
        q[0]=q0;
    }
    rotatestatus=0;
    if(axisangle_ref.empty()){
        axisangle_ref.push_back(matrix_to_axisangle_ref_fix_space());
    }
    else{
        axisangle_ref[0]=matrix_to_axisangle_ref_fix_space();
    }
    //PrintParameters(q0);
}

void bodybasic::setbodybasic(const std::vector<double>& positionglobal, const std::vector<double>& naxisvalueglobal, double rotationanglevalueglobal, bodybasic* parentbodybasic){

    initialsetting_naxis=globaltolocal(parentbodybasic->getposition(),parentbodybasic->getaxis(), naxisvalueglobal);
    initialsetting_angle=rotationanglevalueglobal;
    rhobody=globaltolocal(parentbodybasic->getposition(),parentbodybasic->getaxis(), positionglobal);
    position=positionglobal;
    rotatestatus=0;

    std::vector<std::vector<double>> R=rotationMatrix(initialsetting_naxis, initialsetting_angle/180.0*M_PI);
    std::vector<double> qnewall;
    std::vector<std::vector<double>> axisnew;
    std::vector<std::vector<double>> lastbodyaxis=parentbodybasic->getaxis();
    qnewall=pushback(qnewall, positionglobal);
    for(int i=0; i<3; i++){
        std::vector<double> axisi = matrix33time31tog(R, lastbodyaxis[i]);
        axisnew.push_back(axisi);
        qnewall=pushback(qnewall, axisi);
    }
    axis=axisnew;
    if(q.empty()){
        q.push_back(qnewall);
    }
    else{
        q[0]=qnewall;
    }
    if(axisangle_ref.empty()){
        axisangle_ref.push_back(matrix_to_axisangle_ref_fix_space());
    }
    else{
        axisangle_ref[0]=matrix_to_axisangle_ref_fix_space();
    }
    //PrintParameters(q[q.size()-1]);
}

void bodybasic::setbodybasic(const std::vector<double>& lastbodyposition, const std::vector<std::vector<double>>& lastbodyaxis, const std::vector<double>& naxisvalue, double rotationanglevalue, const std::vector<double>& rhobodyvalue){
    initialsetting_naxis=naxisvalue;
    initialsetting_angle=rotationanglevalue;
    rhobody=rhobodyvalue;
    //std::vector<double> n_axis=matrix33time31sepcol(lastbodyaxis, initialsetting_naxis);
    //n_axis=vector3timeconstant(n_axis, initialsetting_angle/180.0*M_PI);
    //std::vector<std::vector<double>> R=RodriguesMap(n_axis);
    std::vector<std::vector<double>> R=rotationMatrix(initialsetting_naxis, initialsetting_angle/180.0*M_PI);
    std::vector<double> qnewall;
    std::vector<double> phi_body=localtoglobal(lastbodyposition, lastbodyaxis, rhobody);
    position=phi_body;
    std::vector<std::vector<double>> axisnew;
    qnewall=pushback(qnewall, phi_body);
    for(int i=0; i<3; i++){
        std::vector<double> axisi = matrix33time31tog(R, lastbodyaxis[i]);
        axisnew.push_back(axisi);
        qnewall=pushback(qnewall, axisi);
    }
    axis=axisnew;
    if(q.empty()){
        q.push_back(qnewall);
    }
    else{
        q[0]=qnewall;
    }
    rotatestatus=0;
    if(axisangle_ref.empty()){
        axisangle_ref.push_back(matrix_to_axisangle_ref_fix_space());
    }
    else{
        axisangle_ref[0]=matrix_to_axisangle_ref_fix_space();
    }
    //PrintParameters(q[q.size()-1]);
}

void bodybasic::updatebodybasic(const std::vector<double>& lastbodyposition, const std::vector<std::vector<double>>& lastbodyaxis){
    std::vector<double> n_axis=matrix33time31sepcol(lastbodyaxis, initialsetting_naxis);
    //n_axis=vector3timeconstant(n_axis, initialsetting_angle/180.0*M_PI);
    //std::vector<std::vector<double>> R=RodriguesMap(n_axis);
    std::vector<std::vector<double>> R=rotationMatrix(n_axis, initialsetting_angle/180.0*M_PI);
    std::vector<double> qnewall;
    std::vector<double> phi_body1=matrix33time31sepcol(lastbodyaxis, rhobody);
    std::vector<double> phi_body=vector3plus(lastbodyposition, phi_body1);
    position=phi_body;
    std::vector<std::vector<double>> axisnew;
    qnewall=pushback(qnewall, phi_body);
    for(int i=0; i<3; i++){
        std::vector<double> axisi = matrix33time31tog(R, lastbodyaxis[i]);
        axisnew.push_back(axisi);
        qnewall=pushback(qnewall, axisi);
    }
    axis=axisnew;
    if(q.empty()){
        q.push_back(qnewall);
    }
    else{
        q[0]=qnewall;
    }
    rotatestatus=0;
    if(axisangle_ref.empty()){
        axisangle_ref.push_back(matrix_to_axisangle_ref_fix_space());
    }
    else{
        axisangle_ref[0]=matrix_to_axisangle_ref_fix_space();
    }
    //PrintParameters(q[q.size()-1]);
}

bodybasic::~bodybasic(){

}

std::vector<double> bodybasic::getposition(){
    return position;
}

std::vector<std::vector<double>> bodybasic::getaxis(){
    return axis;
}

std::vector<std::vector<double>> bodybasic::getq(){
    return q;
}

std::vector<double> bodybasic::getrhobody(){
    return rhobody;
}

int bodybasic::getrotatestatus(){
    return rotatestatus;
}

std::vector<double> bodybasic::getinitialsetting_naxis(){
    return initialsetting_naxis;
}
    
double bodybasic::getinitialsetting_angle(){
    return initialsetting_angle;
}

std::vector<std::vector<double>> bodybasic::getaxisangle_ref(){
    return axisangle_ref;
}

std::vector<double> bodybasic::getglobalnaxis(bodybasic* parentbodybasic){
    return localtoglobal(parentbodybasic->getposition(),parentbodybasic->getaxis(), initialsetting_naxis);
}
    
void bodybasic::setrotatestatus(int newstatus){
    rotatestatus=newstatus;
}

void bodybasic::setpoistionaxis(const std::vector<double>& q){
    std::vector<double> positionnew;
    std::vector<std::vector<double>> axisnew;
    for(int i=0; i<3; i++){
        positionnew.push_back(q[i]);
        std::vector<double> axisnew1;
        for (int j = 3+i*3; j < 6+i*3 ; j++) {
            axisnew1.push_back(q[j]);
        }
        axisnew.push_back(axisnew1);
    }
    position=positionnew;
    axis=axisnew;
}

std::vector<double> bodybasic::pushback(std::vector<double>& q, const std::vector<double>& value){
    for(int i=0; i<value.size(); i++){
        q.push_back(value[i]);
    }
    return q;
}

void bodybasic::norotateaddvalue(){
    q.push_back(q[q.size()-1]);
    axisangle_ref.push_back(axisangle_ref[axisangle_ref.size()-1]);
}

void bodybasic::addnewbodybasic(const std::vector<double>& newbodyposition, const std::vector<std::vector<double>>& newbodyaxis){

    std::vector<double> qnewall;
    qnewall=pushback(qnewall, newbodyposition);
    for(int i=0; i<3; i++){
        qnewall=pushback(qnewall, newbodyaxis[i]);
    }
    q.push_back(qnewall);
    setpoistionaxis(qnewall);
    axisangle_ref.push_back(matrix_to_axisangle_ref_fix_space());
}

void bodybasic::PrintParameters(const std::vector<double>& q0){
    for(int i=0; i<q0.size(); i++){
        std::cout << "q" << i << ": " << q0[i] << std::endl;
    }
}

std::vector<double> bodybasic::matrix_to_axisangle_ref_fix_space(){
    std::vector<std::vector<double>> R=matrixtranspose(axis);
    std::vector<double> naxis_ref1={R[2][1]-R[1][2],R[0][2]-R[2][0],R[1][0]-R[0][1]};
    double naxis_ref_value=std::sqrt(vectortime1(naxis_ref1,naxis_ref1));
    std::vector<double> naxis_ref;
    double rotation_sin=naxis_ref_value/2.0;
    double rotation_cos=(R[0][0]+R[1][1]+R[2][2]-1)/2.0;
    double angle=std::atan2(rotation_sin,rotation_cos);
    if(angle){
        naxis_ref=vector3timeconstant(naxis_ref1,1.0/naxis_ref_value);
    }
    else{
        naxis_ref={0.0,0.0,0.0};
    }
    naxis_ref.push_back(angle/M_PI*180);
    return naxis_ref;
}

void bodybasic::resetforrecalc(){    
    if (q.size() > 1) {
        setpoistionaxis(q[0]);
        q.erase(q.begin() + 1, q.end());
    }
    if (axisangle_ref.size() > 1) {
        axisangle_ref.erase(axisangle_ref.begin() + 1, axisangle_ref.end());
    }
}