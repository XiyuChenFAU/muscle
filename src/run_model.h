/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
31.10.2023
Xiyu Chen

*/
#ifndef RUN_MODEL_H
#define RUN_MODEL_H

#include "model/model.h"
#include "IO/IO.h"
#include <ctime>

class runmodel {
public:
    runmodel();
    runmodel(const std::string& modelname);
    runmodel(const std::string& jsonfile, int json);
    void setrunmodel(const std::string& modelname);
    void setrunmodel(const std::string& jsonfile, int json);
    double runprogram();
    void savemodel();
    ~runmodel();
    IO* getio();
    model* getModel();

private:
    IO* io=nullptr;
    model* Model=nullptr;
};

#endif // RUN_MODEL_H

