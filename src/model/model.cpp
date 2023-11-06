/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "model.h"

model::model(const std::string& modelname):
    modelname(modelname)
{
    parm=new Parm();
    Solveeq=new solveeq();
    Postprocessing=new postprocessing();
    //// space fix
    std::vector<double> qall;
    std::vector<double> origin = {0.0, 0.0, 0.0};
    std::vector<double> e1 = {1.0, 0.0, 0.0};
    std::vector<double> e2 = {0.0, 1.0, 0.0};
    std::vector<double> e3 = {0.0, 0.0, 1.0};
    qall=pushback(qall,origin);
    qall=pushback(qall,e1);
    qall=pushback(qall,e2);
    qall=pushback(qall,e3);
    parm->addbody("fix_space",qall);
}

model::~model(){
    delete parm;
    delete Solveeq;
    delete Postprocessing;
}

void model::setmodelname(const std::string& modelnamevalue){
    modelname=modelnamevalue;
}

Parm* model::getparm(){
    return parm;
}

solveeq* model::getSolveeq(){
    return Solveeq;
}

std::string model::getmodelname(){
    return modelname;
}

postprocessing* model::getPostprocessing(){
    return Postprocessing;
}

void model::solve_signorini(){
    parm->setallmuscleinitialeta();
    Solveeq->solvesignorini(parm);
}

void model::do_postprocessing(){
    Postprocessing->do_postprocessingall(parm);
}

std::vector<double> model::pushback(std::vector<double>& q, const std::vector<double>& value){
    for(int i=0; i<value.size(); i++){
        q.push_back(value[i]);
    }
    return q;
}
