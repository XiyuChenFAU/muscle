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

int objective::getcasenum(){
    return casenum;
}

SX objective::getlengthdisdiff(Parm* parm, std::vector<std::vector<std::vector<SX>>> gammaallnode){
    std::vector<SX> nodediff;
    std::vector<muscle*> Muscle=parm->getallmuscle();
    SX f_length=0;
    for(int i=0;i<parm->getn_muscles();i++){
        std::vector<std::vector<double>> gammapreviousall=Muscle[i]->getgammaall();
        std::vector<double> gammalaststep=gammapreviousall[gammapreviousall.size()-1];
        for(int j=0;j<Muscle[i]->getnodenum()-1;j++){
            std::vector<SX> gammaallnodemuscle1; 
            f_length=f_length+(gammaallnode[i][j+1][0] -gammaallnode[i][j][0]) *(gammaallnode[i][j+1][0] -gammaallnode[i][j][0])+(gammaallnode[i][j+1][1] -gammaallnode[i][j][1]) *(gammaallnode[i][j+1][1] -gammaallnode[i][j][1])+(gammaallnode[i][j+1][2] -gammaallnode[i][j][2]) *(gammaallnode[i][j+1][2] -gammaallnode[i][j][2]);
            f_length=f_length-((gammalaststep[j*3+3]-gammalaststep[j*3])*(gammalaststep[j*3+3]-gammalaststep[j*3])+(gammalaststep[j*3+4]-gammalaststep[j*3+1])*(gammalaststep[j*3+4]-gammalaststep[j*3+1])+(gammalaststep[j*3+5]-gammalaststep[j*3+2])*(gammalaststep[j*3+5]-gammalaststep[j*3+2]));
        }
    }
    
    return f_length;
}

SX objective::getnodelengthdisdiffall(Parm* parm, SX x, std::vector<std::vector<double>> jointposition){
    std::vector<std::vector<std::vector<std::vector<SX>>>> dataall=rearrange_gamma_eta(parm, x);
    std::vector<std::vector<std::vector<SX>>> gammaallnode=dataall[0];
    std::vector<SX> nodediff=getnodedisdiff(parm,gammaallnode);
    std::vector<double> massmatrix=getmassmatrix(parm, jointposition);
    SX f_mass=getobjectivemass(nodediff,massmatrix);
    f_mass=f_mass+getlengthdisdiff(parm, gammaallnode);
    return f_mass;
}

std::vector<SX> objective::getnodedisdiff(Parm* parm, std::vector<std::vector<std::vector<SX>>> gammaallnode){
    std::vector<SX> nodediff;
    std::vector<muscle*> Muscle=parm->getallmuscle();
    for(int i=0;i<parm->getn_muscles();i++){
        std::vector<std::vector<double>> gammapreviousall=Muscle[i]->getgammaall();
        std::vector<double> gammalaststep=gammapreviousall[gammapreviousall.size()-1];
        for(int j=0;j<Muscle[i]->getnodenum();j++){
            std::vector<SX> gammaallnodemuscle1;            
            for(int k=0;k<3;k++){
                nodediff.push_back(gammaallnode[i][j][k]-gammalaststep[j*3+k]);
            }
        }
    }
    return nodediff;
}

std::vector<double> objective::getmassmatrix(Parm* parm, std::vector<std::vector<double>> jointposition){
    std::vector<double> massmatrix;
    std::vector<muscle*> Muscle=parm->getallmuscle();
    for(int i=0;i<parm->getn_muscles();i++){
        std::vector<std::vector<double>> gammapreviousall=Muscle[i]->getgammaall();
        std::vector<double> gammalaststep=gammapreviousall[gammapreviousall.size()-1];
        for(int j=0;j<Muscle[i]->getnodenum();j++){
            std::vector<SX> gammaallnodemuscle1;
            if(casenum==1 || casenum==3){
                for(int k=0;k<3;k++){
                    massmatrix.push_back(1.0);
                }
            }
            if(casenum==2){
                double massmatrixvalue=100000;
                double massvalue=1.0;
                for(int k=0; k<jointposition.size();k++){
                    massvalue=massvalue*std::sqrt((gammalaststep[3*j+0]-jointposition[k][0])*(gammalaststep[3*j+0]-jointposition[k][0])+(gammalaststep[3*j+1]-jointposition[k][1])*(gammalaststep[3*j+1]-jointposition[k][1])+(gammalaststep[3*j+2]-jointposition[k][2])*(gammalaststep[3*j+2]-jointposition[k][2]));
                }
                if(massvalue!=0){
                    massmatrixvalue=1.0/massvalue;
                }
                for(int k=0;k<3;k++){
                    massmatrix.push_back(massmatrixvalue);
                }
            }            
            
        }
    }
    return massmatrix;
}

SX objective::getobjectivemass(std::vector<SX> nodediff, std::vector<double> massmatrix){
    SX f_mass=0;
    for(int i=0;i<nodediff.size();i++){
        f_mass=f_mass+nodediff[i]*nodediff[i]*massmatrix[i];
    }
    return f_mass;
}

SX objective::getnodedisdiffall(Parm* parm, SX x, std::vector<std::vector<double>> jointposition){
    std::vector<std::vector<std::vector<std::vector<SX>>>> dataall=rearrange_gamma_eta(parm, x);
    std::vector<std::vector<std::vector<SX>>> gammaallnode=dataall[0];
    std::vector<SX> nodediff=getnodedisdiff(parm,gammaallnode);
    std::vector<double> massmatrix=getmassmatrix(parm, jointposition);
    SX f_mass=getobjectivemass(nodediff,massmatrix);
    return f_mass;
}

std::vector<std::vector<std::vector<std::vector<SX>>>> objective::rearrange_gamma_eta(Parm* parm, SX x){
    std::vector<std::vector<std::vector<std::vector<SX>>>> dataall;
    std::vector<std::vector<std::vector<SX>>> gammaallnode;
    std::vector<std::vector<std::vector<SX>>> etaall;
    std::vector<muscle*> Muscle=parm->getallmuscle();
    int constraintnum_muscle=0;
    for(int i=0;i<parm->getn_muscles();i++){
        std::vector<std::vector<SX>> gammaallnodemuscle;
        std::vector<std::vector<SX>> etamuscle;

        for(int j=0;j<Muscle[i]->getnodenum();j++){
            std::vector<SX> gammaallnodemuscle1;            
            for(int k=0;k<3;k++){
                gammaallnodemuscle1.push_back(x(constraintnum_muscle+j*3+k));
            }
            gammaallnodemuscle.push_back(gammaallnodemuscle1);  
            if(j>0 && j<Muscle[i]->getnodenum()-1){
                std::vector<SX> etamuscle1;
                for(int k=0;k<parm->getn_bodies();k++){
                    etamuscle1.push_back(x(constraintnum_muscle+Muscle[i]->getnodenum()*3+(j-1)*parm->getn_bodies()+k));
                }
                etamuscle.push_back(etamuscle1);
            }            
        }

        constraintnum_muscle=constraintnum_muscle+Muscle[i]->getnodenum()*3+(Muscle[i]->getnodenum()-2)*parm->getn_bodies();
        gammaallnode.push_back(gammaallnodemuscle);
        etaall.push_back(etamuscle);
    }

    dataall.push_back(gammaallnode);
    dataall.push_back(etaall);

    return dataall;
}

SX objective::getobjective(Parm* parm, SX x, std::vector<std::vector<double>> jointposition){
    switch(casenum){
        case 0:
            f = 0;
            break;
        case 1:
            f = getnodedisdiffall(parm, x, jointposition);
            break;
        case 2:
            f = getnodedisdiffall(parm, x, jointposition);
            break;
        case 3:
            f = getnodelengthdisdiffall(parm, x, jointposition);
    }
    return f;
}
