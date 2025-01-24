/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef INITIALGUESS_H
#define INITIALGUESS_H

#include <vector>
#include <iostream>
#include "../parm/parm.h"
#include "../math/math.h"

class initialguess {
public:
    initialguess(int mode_nr);
    ~initialguess();

    void setmode_nr(int number);
    int getmode_nr();
    void setselect_bodyname(std::string bodyname);
    std::string getselect_bodyname();

    void setpartition(Parm* parm);
    void set_initialguessvalue(Parm* parm);
    std::vector<std::vector<double>> get_initialguessvalue();
    std::vector<double> get_initialguessvalueindex(int index);
    void resetforrecalc();


private:
    std::string select_bodyname="fix_space";
    int mode_number=0;
    std::vector<std::vector<body*>> body_partition={};
    std::vector<std::vector<double>> initialguessvalue={};
};

#endif // INITIALGUESS_H
