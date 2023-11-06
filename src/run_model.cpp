/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
31.10.2023
Xiyu Chen

*/

#include "run_model.h"

runmodel::runmodel(){
    io=new IO();
}

runmodel::runmodel(const std::string& modelname){
    io=new IO();
    Model= new model(modelname);
}

runmodel::runmodel(const std::string& jsonfile, int json){
    
    io=new IO();
    Model= io->readmodel(jsonfile);
}

void runmodel::setrunmodel(const std::string& modelname){
    if(Model==nullptr){
        Model= new model(modelname);
    }
    else{
        Model->setmodelname(modelname);
    }
}

void runmodel::setrunmodel(const std::string& jsonfile, int json){
    if(Model==nullptr){
        Model= io->readmodel(jsonfile);
    }
    else{
        delete Model;
        Model=io->readmodel(jsonfile);
    }
}

double runmodel::runprogram(){
    clock_t start_time = clock();
    Model->solve_signorini();
    Model->do_postprocessing();


    io->writemusclebodyresultToFileAll(Model);
    io->writeanalyzeresultToFileAll(Model);

    clock_t end_time = clock();
    double elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
    return elapsed_time;
}

void runmodel::savemodel(){
    io->writejson(Model);
}

runmodel::~runmodel(){
    delete io;
    delete Model;
}

IO* runmodel::getio(){
    return io;
}

model* runmodel::getModel(){
    return Model;
}







