/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef MATH_H
#define MATH_H

#include <vector>
#include <iostream>
#include <cmath>

std::vector<double> globaltolocal(const std::vector<double>& position, const std::vector<std::vector<double>>& axis, const std::vector<double>& rho);
std::vector<double> localtoglobal(const std::vector<double>& position, const std::vector<std::vector<double>>& axis, const std::vector<double>& rho);

std::vector<std::vector<double>> rotationMatrix(const std::vector<double>& axis, double angle);
std::vector<std::vector<double>> CayleyMap(const std::vector<double>& eta);
std::vector<std::vector<double>> HatVec(const std::vector<double>& n);
std::vector<double> InverseHat(const std::vector<std::vector<double>>& hat);
std::vector<std::vector<double>> RodriguesMap(const std::vector<double>& eta);

std::vector<double> matrix33time31sepcol(const std::vector<std::vector<double>>& columnmatrix, const std::vector<double>& vector);//local->global
std::vector<double> matrix33time31tog(const std::vector<std::vector<double>>& columnmatrix, const std::vector<double>& vector);//global->local
std::vector<double> vector3timeconstant(const std::vector<double>& vector, double value);
std::vector<double> vector3plus(const std::vector<double>& vector1, const std::vector<double>& vector2);
std::vector<double> vector3minus(const std::vector<double>& vector1, const std::vector<double>& vector2);
double sumvector(const std::vector<double>& vector1);
double vectortime1(const std::vector<double>& vector1, const std::vector<double>& vector2);
std::vector<std::vector<double>> matrixplus(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2);
std::vector<std::vector<double>> matrixminus(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2);
std::vector<std::vector<double>> matrixtime(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2);
std::vector<std::vector<double>> matrixtranspose(const std::vector<std::vector<double>>& matrix);
std::vector<std::vector<double>> matrixeye(int length);
std::vector<std::vector<double>> matrixtimeconstant(const std::vector<std::vector<double>>& matrix, double value);

double determinant(const std::vector<std::vector<double>>& matrix);
std::vector<std::vector<double>> adjoint(const std::vector<std::vector<double>>& matrix);
std::vector<std::vector<double>> inverse(const std::vector<std::vector<double>>& matrix);

std::vector<double> solveAx_0(const std::vector<std::vector<double>>& A);

std::vector<double> crossProduct(const std::vector<double>& vector1,const std::vector<double>& vector2);
std::vector<double> calculationaxisangle(const std::vector<double>& vector1,const std::vector<double>& vector2);

#endif // MATH_H
