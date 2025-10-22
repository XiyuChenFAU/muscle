/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef IO_H
#define IO_H


#include "../model/model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <cstdio>
#include "../json/json/json.h"


class IO {
public:
    IO();
    ~IO();
    void writemusclebodyresultToFileAll(model* Model);
    void writeanalyzeresultToFileAll(model* Model);
    void writejson(model* Model, int write_gamma, int currentstepnum);
    model* readmodel(const std::string& jsonfilename);

private:
    void write2DvalueToFile(const std::vector<std::vector<double>>& value, std::ofstream& file, const std::string& name, const std::string& typenamevalue);
    void writephiToFile(model* Model);
    void writelengthToFile(model* Model);
    void writeforcenodeToFile(model* Model);
    void writetotalforceToFile(model* Model);
    void writebody_stateToFile(model* Model);
    void writemomentarmnodeToFile(model* Model);
    void writemomentarmToFile(model* Model);
    void writerunningtimeToFile(model* Model);
};

#endif // IO_H
