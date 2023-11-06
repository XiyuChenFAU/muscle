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
#include "../json/json/json.h"


class IO {
public:
    IO();
    ~IO();
    void writemusclebodyresultToFileAll(model* Model, const std::string& folderPath = "");
    void writeanalyzeresultToFileAll(model* Model, const std::string& folderPath = "");
    void writejson(model* Model, const std::string& folderPath="");
    model* readmodel(std::string jsonfilename);

private:
    void write2DvalueToFile(const std::vector<std::vector<double>>& value, std::ofstream& file, const std::string& name, const std::string& typenamevalue);
    void writephiToFile(model* Model, const std::string& folderPath);
    void writelengthToFile(model* Model, const std::string& folderPath);
    void writeforcenodeToFile(model* Model, const std::string& folderPath);
    void writetotalforceToFile(model* Model, const std::string& folderPath);
    void writebody_stateToFile(model* Model, const std::string& folderPath);
};

#endif // IO_H
