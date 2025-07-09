/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef PRINT_H
#define PRINT_H

#include <iostream>
#include <vector>

void print3DVector(const std::vector<std::vector<std::vector<double>>>& vec);
void print2DVector(const std::vector<std::vector<double>>& vec);
void print1DVector(const std::vector<double>& vec);
void print3D(const std::string& name, const std::vector<std::vector<std::vector<double>>>& vec);
void print2D(const std::string& name, const std::vector<std::vector<double>>& vec);
void print1D(const std::string& name, const std::vector<double>& vec);

#endif // PRINT_H
