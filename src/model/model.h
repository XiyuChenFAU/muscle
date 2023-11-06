/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef MODEL_H
#define MODEL_H


#include "../solveeq/solveeq.h"
#include "../postprocessing/postprocessing.h"


class model {
public:
    model(const std::string& modelname);
    ~model();
    void setmodelname(const std::string& modelnamevalue);
    Parm* getparm();
    solveeq* getSolveeq();
    std::string getmodelname();
    postprocessing* getPostprocessing();
    void solve_signorini();
    void do_postprocessing();
    std::vector<double> pushback(std::vector<double>& q, const std::vector<double>& value);

private:
    std::string modelname;
    Parm* parm=nullptr;
    solveeq* Solveeq=nullptr;
    postprocessing* Postprocessing=nullptr;
};

#endif // MODEL_H
