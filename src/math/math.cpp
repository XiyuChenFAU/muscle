/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "math.h"

using namespace std;

std::vector<std::vector<double>> rotationMatrix(const std::vector<double>& axis, double angle) {
    double x = axis[0];
    double y = axis[1];
    double z = axis[2];

    double c = std::cos(angle);
    double s = std::sin(angle);
    double t = 1 - c;

    std::vector<std::vector<double>> matrix(3, std::vector<double>(3));

    matrix[0][0] = t * x * x + c;
    matrix[0][1] = t * x * y - s * z;
    matrix[0][2] = t * x * z + s * y;

    matrix[1][0] = t * x * y + s * z;
    matrix[1][1] = t * y * y + c;
    matrix[1][2] = t * y * z - s * x;

    matrix[2][0] = t * x * z - s * y;
    matrix[2][1] = t * y * z + s * x;
    matrix[2][2] = t * z * z + c;

    return matrix;
}

std::vector<double> localtoglobal(const std::vector<double>& position, const std::vector<std::vector<double>>& axis, const std::vector<double>& rho){
    return vector3plus(position, matrix33time31sepcol(axis, rho));
}

std::vector<double> globaltolocal(const std::vector<double>& position, const std::vector<std::vector<double>>& axis, const std::vector<double>& rho){
    return matrix33time31tog(axis, vector3minus(rho, position));
}

std::vector<std::vector<double>> CayleyMap(const std::vector<double>& eta){
    std::vector<std::vector<double>> A= HatVec(eta);
    std::vector<std::vector<double>> Ahalf= matrixtimeconstant(A,0.5);
    std::vector<std::vector<double>> eye= matrixeye(eta.size());
    std::vector<std::vector<double>> temp1=matrixminus(eye,Ahalf);
    std::vector<std::vector<double>> temp2=matrixplus(eye,Ahalf);
    std::vector<std::vector<double>> R=matrixtime(inverse(temp1),temp2);
    return R;

}

std::vector<std::vector<double>> HatVec(const std::vector<double>& n) {
    std::vector<std::vector<double>> hat={
        {0,-n[2],n[1]},
        {n[2],0,-n[0]},
        {-n[1],n[0],0}
    };
    return hat;
}

std::vector<double> InverseHat(const std::vector<std::vector<double>>& hat) {
    std::vector<double> inv = {0.5 * (hat[2][1] - hat[1][2]), 0.5 * (hat[0][2] - hat[2][0]), 0.5 * (hat[1][0] - hat[0][1])};
    return inv;
}

std::vector<std::vector<double>> RodriguesMap(const std::vector<double>& eta) {
    double phi = sqrt(eta[0] * eta[0] + eta[1] * eta[1] + eta[2] * eta[2]);
    std::vector<std::vector<double>> R;
    if (phi < 1e-12) {
        for (int i = 0; i < eta.size(); ++i) {
            std::vector<double>  Rraw;
            R.push_back(Rraw);
            for (int j = 0; j < 3; ++j) {
                if (i == j)
                    R[i].push_back(1.0);
                else
                    R[i].push_back(0.0);
            }
        }
    } else {
        std::vector<double>  n = vector3timeconstant(eta, 1/phi);
        std::vector<std::vector<double>> n_hat=HatVec(n);
        R=matrixplus(matrixplus(matrixeye(eta.size()),matrixtimeconstant(n_hat,std::sin(phi))),matrixtimeconstant(matrixtime(n_hat,n_hat),1.0-std::cos(phi)));
    }
    return R;
}

std::vector<double> matrix33time31sepcol(const std::vector<std::vector<double>>& columnmatrix, const std::vector<double>& vector){
    std::vector<double> naxis;
    double value=0;
    for(int i=0; i<columnmatrix[0].size(); i++){
        value=0;
        for(int j=0; j<columnmatrix.size(); j++){
            value=value+columnmatrix[j][i]*vector[j];
        }
        naxis.push_back(value);
    }
    return naxis;
}

std::vector<double> matrix33time31tog(const std::vector<std::vector<double>>& columnmatrix, const std::vector<double>& vector){
    std::vector<double> naxis;
    double value=0;
    for(int i=0; i<columnmatrix.size(); i++){
        value=0;
        for(int j=0; j<columnmatrix[0].size(); j++){
            value=value+columnmatrix[i][j]*vector[j];
        }
        naxis.push_back(value);
    }
    return naxis;
}

std::vector<double> vector3timeconstant(const std::vector<double>& vector, double value){
    std::vector<double> vectornew;
    for(int i=0; i<vector.size(); i++){
        vectornew.push_back(value*vector[i]);
    }
    return vectornew;
}

std::vector<double> vector3plus(const std::vector<double>& vector1, const std::vector<double>& vector2){
    std::vector<double> vectornew;
    for(int i=0; i<vector1.size(); i++){
        vectornew.push_back(vector1[i] + vector2[i]);
    }
    return vectornew;
}

std::vector<double> vector3minus(const std::vector<double>& vector1, const std::vector<double>& vector2){
    std::vector<double> vectornew;
    for(int i=0; i<vector1.size(); i++){
        vectornew.push_back(vector1[i] - vector2[i]);
    }
    return vectornew;
}

double sumvector(const std::vector<double>& vector1){
    double value=0;
    for(int i=0; i<vector1.size(); i++){
        value=value+vector1[i];
    }
    return value;
}

double vectortime1(const std::vector<double>& vector1, const std::vector<double>& vector2){
    double value=0;
    for(int i=0; i<vector1.size(); i++){
        value=value+vector1[i] * vector2[i];
    }
    return value;
}

std::vector<std::vector<double>> matrixplus(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2){
    std::vector<std::vector<double>> matrixnew;
    for(int i=0; i<matrix1.size(); i++){
        std::vector<double> matrixnew1;
        matrixnew.push_back(matrixnew1);
        for(int j=0; j<matrix1[0].size(); j++){
            matrixnew[i].push_back(matrix1[i][j]+matrix2[i][j]);
        }
    }
    return matrixnew;
}

std::vector<std::vector<double>> matrixminus(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2){
    std::vector<std::vector<double>> matrixnew;
    for(int i=0; i<matrix1.size(); i++){
        std::vector<double> matrixnew1;
        matrixnew.push_back(matrixnew1);
        for(int j=0; j<matrix1[0].size(); j++){
            matrixnew[i].push_back(matrix1[i][j]-matrix2[i][j]);
        }
    }
    return matrixnew;
}

std::vector<std::vector<double>> matrixtime(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2){
    std::vector<std::vector<double>> matrixnew;
    double value=0;
    for(int i=0; i<matrix1.size(); i++){
        std::vector<double> matrixnew1;
        matrixnew.push_back(matrixnew1);
        for(int j=0; j<matrix2[0].size(); j++){
            value=0;
            for(int k=0; k<matrix1[0].size(); k++){
                value=value+matrix1[i][k]*matrix2[k][j];
            }
            matrixnew[i].push_back(value);
        }
    }
    return matrixnew;
}

std::vector<std::vector<double>> matrixtranspose(const std::vector<std::vector<double>>& matrix){
    std::vector<std::vector<double>> matrixnew;
    for(int i=0; i<matrix[0].size(); i++){
        std::vector<double> matrixnew1;
        matrixnew.push_back(matrixnew1);
        for(int j=0; j<matrix.size(); j++){
            matrixnew[i].push_back(matrix[j][i]);
        }
    }
    return matrixnew;
}

std::vector<std::vector<double>> matrixeye(int length){
    std::vector<std::vector<double>> matrixnew;
    for(int i=0; i<length; i++){
        std::vector<double> matrixnew1;
        matrixnew.push_back(matrixnew1);
        for(int j=0; j<length; j++){
            if(i==j){
                matrixnew[i].push_back(1.0);
            }
            else{
                matrixnew[i].push_back(0.0);
            }
        }
    }
    return matrixnew;
}

std::vector<std::vector<double>> matrixtimeconstant(const std::vector<std::vector<double>>& matrix, double value){
    std::vector<std::vector<double>> matrixnew;
    for(int i=0; i<matrix.size(); i++){
        std::vector<double> matrixnew1;
        matrixnew.push_back(matrixnew1);
        for(int j=0; j<matrix[0].size(); j++){
            matrixnew[i].push_back(matrix[i][j]*value);
        }
    }
    return matrixnew;
}

double determinant(const std::vector<std::vector<double>>& matrix) {
    int n = matrix.size();

    if (n == 1) {
        return matrix[0][0];
    }

    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    double det = 0.0;
    for (int j = 0; j < n; j++) {
        std::vector<std::vector<double>> submatrix(n - 1, std::vector<double>(n - 1, 0.0));
        for (int i = 1; i < n; i++) {
            for (int k = 0; k < n; k++) {
                if (k != j) {
                    submatrix[i - 1][k > j ? k - 1 : k] = matrix[i][k];
                }
            }
        }

        det += matrix[0][j] * pow(-1, j) * determinant(submatrix);
    }

    return det;
}

std::vector<std::vector<double>> adjoint(const std::vector<std::vector<double>>& matrix) {
    int n = matrix.size();
    std::vector<std::vector<double>> adj(n, std::vector<double>(n, 0.0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::vector<std::vector<double>> submatrix;
            for (int row = 0; row < n; row++) {
                if (row != i) {
                    std::vector<double> subrow;
                    for (int col = 0; col < n; col++) {
                        if (col != j) {
                            subrow.push_back(matrix[row][col]);
                        }
                    }
                    submatrix.push_back(subrow);
                }
            }
            double submatrix_det = determinant(submatrix);

            adj[i][j] = submatrix_det * pow(-1, i + j);
        }
    }

    return matrixtranspose(adj);
}

std::vector<std::vector<double>> inverse(const std::vector<std::vector<double>>& matrix) {

    double det = determinant(matrix);
    
    if (std::abs(det) < 1e-6) {
        std::cerr << "no inverse matrix" << std::endl;
        exit(1);
    }

    int n = matrix.size();
    std::vector<std::vector<double>> adj = adjoint(matrix);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            adj[i][j] /= det;
        }
    }

    return adj;
}

std::vector<double> solveAx_0(const std::vector<std::vector<double>>& A) {
    double detA = determinant(A);
    std::vector<double> solveres;

    if (detA == 0) {
        solveres={0.0,0.0,0.0};
        return solveres;
    }

    solveres.push_back((A[1][1] * A[2][2] - A[1][2] * A[2][1]) / detA);
    solveres.push_back((A[0][2] * A[2][1] - A[0][1] * A[2][2]) / detA);
    solveres.push_back((A[0][1] * A[1][2] - A[0][2] * A[1][1]) / detA);

    return solveres;
}

std::vector<double> crossProduct(const std::vector<double>& vector1,const std::vector<double>& vector2) {
    std::vector<double> result(3);
    
    result[0] = vector1[1] * vector2[2] - vector1[2] * vector2[1];
    result[1] = vector1[2] * vector2[0] - vector1[0] * vector2[2];
    result[2] = vector1[0] * vector2[1] - vector1[1] * vector2[0];

    return result;
}

std::vector<double> calculationaxisangle(const std::vector<double>& vector1,const std::vector<double>& vector2){
    std::vector<double> vector1unit=vector3timeconstant(vector1,1.0/std::sqrt(vectortime1(vector1,vector1)));
    std::vector<double> vector2unit=vector3timeconstant(vector2,1.0/std::sqrt(vectortime1(vector2,vector2)));

    std::vector<double> rotationaxis=crossProduct(vector1unit,vector2unit);

    double rotation_sin=std::sqrt(vectortime1(rotationaxis,rotationaxis));
    double rotation_cos=vectortime1(vector1unit,vector2unit);
    double angle=std::atan2(rotation_sin,rotation_cos);
    std::vector<double> rotationaxisunit=vector3timeconstant(rotationaxis,1.0/rotation_sin);
    rotationaxisunit.push_back(angle);
    return rotationaxisunit;
}


