/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "objective.h"

using namespace std;
using namespace casadi;


objective::objective(){
    casenum=0;
}

objective::~objective(){

}

void objective::setcasenum(int casenumvalue){
    casenum=casenumvalue;
}

void objective::setlengthconstant(double lengthconstantvalue){
    lengthconstant=lengthconstantvalue;
}

double objective::getlengthconstant(){
    return lengthconstant;
}

int objective::getcasenum(){
    return casenum;
}

SX objective::getlengthdisdiff(muscle* Muscle, const std::vector<std::vector<SX>>& gammaallnode){

    std::vector<std::vector<double>> gammapreviousall=Muscle->getgammaall();
    std::vector<double> gammalaststep=gammapreviousall.back();
    SX f_length=0;
    for(int j=0;j<Muscle->getnodenum()-1;j++){
        std::vector<SX> gammaallnodemuscle1; 
        f_length=f_length + abs((gammaallnode[j+1][0] -gammaallnode[j][0]) *(gammaallnode[j+1][0] -gammaallnode[j][0])+(gammaallnode[j+1][1] -gammaallnode[j][1]) *(gammaallnode[j+1][1] -gammaallnode[j][1])+(gammaallnode[j+1][2] -gammaallnode[j][2]) *(gammaallnode[j+1][2] -gammaallnode[j][2]) -
            ((gammalaststep[j*3+3]-gammalaststep[j*3])*(gammalaststep[j*3+3]-gammalaststep[j*3])+(gammalaststep[j*3+4]-gammalaststep[j*3+1])*(gammalaststep[j*3+4]-gammalaststep[j*3+1])+(gammalaststep[j*3+5]-gammalaststep[j*3+2])*(gammalaststep[j*3+5]-gammalaststep[j*3+2])));
    }
    return f_length;
}

SX objective::getnodelengthdisdiffall(Parm* parm, SX x, const std::vector<std::vector<double>>& jointposition, int musclenum){
    std::vector<std::vector<std::vector<SX>>> dataall=rearrange_gamma_eta(parm, x, musclenum);
    std::vector<std::vector<SX>> gammaallnode=dataall[0];
    muscle* Muscle=parm->getmuscleindex(musclenum);
    std::vector<SX> nodediff=getnodedisdiff(Muscle,gammaallnode);
    std::vector<double> massmatrix=getmassmatrix(Muscle, jointposition);

    double massmatrixsum=0.0;
    for(int j=0;j<massmatrix.size();j++){
        massmatrixsum=massmatrixsum+massmatrix[j];
    }
    massmatrixsum=massmatrixsum/massmatrix.size();
    SX f_mass=getobjectivemass(nodediff,massmatrix);
    SX lengthdiff=getlengthdisdiff(Muscle, gammaallnode);

    SX lengthdiff_all=f_mass+lengthdiff*massmatrixsum*lengthconstant;

    return lengthdiff_all;
}

std::vector<SX> objective::getnodedisdiff(muscle* Muscle, const std::vector<std::vector<SX>>& gammaallnode){
    std::vector<SX> nodediff;
    std::vector<std::vector<double>> gammapreviousall=Muscle->getgammaall();
    std::vector<double> gammalaststep=gammapreviousall[gammapreviousall.size()-1];
    for(int j=0;j<Muscle->getnodenum();j++){        
        for(int k=0;k<3;k++){
            nodediff.push_back(gammaallnode[j][k]-gammalaststep[j*3+k]);
        }
    }
    return nodediff;
}

std::vector<double> objective::getmassmatrix(muscle* Muscle, const std::vector<std::vector<double>>& jointposition){
    std::vector<std::vector<double>> gammapreviousall=Muscle->getgammaall();
    std::vector<double> gammalaststep=gammapreviousall.back();
    std::vector<double> massmatrix;

    for(int j=0;j<Muscle->getnodenum();j++){
        if(casenum==1){
            for(int k=0;k<3;k++){
                massmatrix.push_back(1.0);
            }
        }
        if(casenum==2 || casenum==3){
            double massvalue=0.0;
            for(int k=0; k<jointposition.size();k++){
                double massvaluei=std::sqrt((gammalaststep[3*j+0]-jointposition[k][0])*(gammalaststep[3*j+0]-jointposition[k][0])+(gammalaststep[3*j+1]-jointposition[k][1])*(gammalaststep[3*j+1]-jointposition[k][1])+(gammalaststep[3*j+2]-jointposition[k][2])*(gammalaststep[3*j+2]-jointposition[k][2]));
                if(massvalue!=0){
                    massvalue=massvalue+1.0/massvaluei;
                }
                else{
                    massvalue=massvalue+1000000;
                }
            }
            double massmatrixvalue=1.0;
            if(jointposition.size()>0){massmatrixvalue=massvalue/jointposition.size();}
            for(int k=0;k<3;k++){
                massmatrix.push_back(massmatrixvalue);
            }
        }            
    }

    return massmatrix;
}

SX objective::getobjectivemass(const std::vector<SX>& nodediff, const std::vector<double>& massmatrix){
    SX f_mass=0;
    for(int i=0;i<nodediff.size();i++){
        f_mass=f_mass+nodediff[i]*nodediff[i]*massmatrix[i];
    }
    return f_mass;
}

SX objective::getnodedisdiffall(Parm* parm, SX x, const std::vector<std::vector<double>>& jointposition, int musclenum){
    std::vector<std::vector<std::vector<SX>>> dataall=rearrange_gamma_eta(parm, x, musclenum);
    std::vector<std::vector<SX>> gammaallnode=dataall[0];
    muscle* Muscle=parm->getmuscleindex(musclenum);
    std::vector<SX> nodediff=getnodedisdiff(Muscle,gammaallnode);
    std::vector<double> massmatrix=getmassmatrix(Muscle, jointposition);
    SX f_mass=getobjectivemass(nodediff,massmatrix);
    return f_mass;
}

std::vector<std::vector<std::vector<SX>>> objective::rearrange_gamma_eta(Parm* parm, SX x, int musclenum){
    std::vector<std::vector<std::vector<SX>>> dataall;
    std::vector<std::vector<SX>> gammaallnodemuscle;
    std::vector<std::vector<SX>> etamuscle;
    muscle* Muscle=parm->getmuscleindex(musclenum);
    for(int j=0;j<Muscle->getnodenum();j++){
        std::vector<SX> gammaallnodemuscle1;            
        for(int k=0;k<3;k++){
            gammaallnodemuscle1.push_back(x(j*3+k));
        }
        gammaallnodemuscle.push_back(gammaallnodemuscle1);  
        if(j>0 && j<Muscle->getnodenum()-1){
            std::vector<SX> etamuscle1;
            for(int k=0;k<parm->getn_bodies();k++){
                etamuscle1.push_back(x(Muscle->getnodenum()*3+(j-1)*parm->getn_bodies()+k));
            }
            etamuscle.push_back(etamuscle1);
        }            
    }

    dataall.push_back(gammaallnodemuscle);
    dataall.push_back(etamuscle);

    return dataall;
}

SX objective::getobjective(Parm* parm, SX x, const std::vector<std::vector<double>>& jointposition, int musclenum){
    switch(casenum){
        case 0:
            f = 0;
            break;
        case 1:
            f = getnodedisdiffall(parm, x, jointposition, musclenum);
            break;
        case 2:
            f = getnodedisdiffall(parm, x, jointposition, musclenum);
            break;
        case 3:
            f = getnodelengthdisdiffall(parm, x, jointposition, musclenum);
    }
    return f;
}
