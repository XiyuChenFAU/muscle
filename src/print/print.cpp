#include "print.h"


void print3DVector(const std::vector<std::vector<std::vector<double>>>& vec){
    for (size_t i = 0; i < vec.size(); i++) {
        print2DVector(vec[i]);
    }
}

void print2DVector(const std::vector<std::vector<double>>& vec){
    for (size_t i = 0; i < vec.size(); i++) {
        print1DVector(vec[i]);
    }
}

void print1DVector(const std::vector<double>& vec){
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << "\t";
    }
    std::cout<<std::endl;
}

void print3D(const std::string& name, const std::vector<std::vector<std::vector<double>>>& vec){
    std::cout<<name<<":\n";
    print3DVector(vec);
}

void print2D(const std::string& name, const std::vector<std::vector<double>>& vec){
    std::cout<<name<<":\n";
    print2DVector(vec);
}

void print1D(const std::string& name, const std::vector<double>& vec){
    std::cout<<name<<":\n";
    print1DVector(vec);
}
