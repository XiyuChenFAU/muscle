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

MX objective::getlengthdisdiff(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode){

    std::vector<double> gammalaststep=Muscle->getgamma_step(-1);
    MX f_length=0;
    for(int j=0;j<Muscle->getnodenum()-1;j++){
        std::vector<MX> gammaallnodemuscle1; 
        f_length=f_length + (gammaallnode[j+1][0] -gammaallnode[j][0]) *(gammaallnode[j+1][0] -gammaallnode[j][0])+(gammaallnode[j+1][1] -gammaallnode[j][1]) *(gammaallnode[j+1][1] -gammaallnode[j][1])+(gammaallnode[j+1][2] -gammaallnode[j][2]) *(gammaallnode[j+1][2] -gammaallnode[j][2]) -
            ((gammalaststep[j*3+3]-gammalaststep[j*3])*(gammalaststep[j*3+3]-gammalaststep[j*3])+(gammalaststep[j*3+4]-gammalaststep[j*3+1])*(gammalaststep[j*3+4]-gammalaststep[j*3+1])+(gammalaststep[j*3+5]-gammalaststep[j*3+2])*(gammalaststep[j*3+5]-gammalaststep[j*3+2]));
    }
    return f_length*f_length;
}

MX objective::getlengthdisdiff(const std::vector<MX>& gammalaststep, const std::vector<std::vector<MX>>& gammaallnode){

    MX f_length=0;
    for(int j=0;j<gammalaststep.size()/3-1;j++){
        std::vector<MX> gammaallnodemuscle1; 
        f_length=f_length + (gammaallnode[j+1][0] -gammaallnode[j][0]) *(gammaallnode[j+1][0] -gammaallnode[j][0])+(gammaallnode[j+1][1] -gammaallnode[j][1]) *(gammaallnode[j+1][1] -gammaallnode[j][1])+(gammaallnode[j+1][2] -gammaallnode[j][2]) *(gammaallnode[j+1][2] -gammaallnode[j][2]) -
            ((gammalaststep[j*3+3]-gammalaststep[j*3])*(gammalaststep[j*3+3]-gammalaststep[j*3])+(gammalaststep[j*3+4]-gammalaststep[j*3+1])*(gammalaststep[j*3+4]-gammalaststep[j*3+1])+(gammalaststep[j*3+5]-gammalaststep[j*3+2])*(gammalaststep[j*3+5]-gammalaststep[j*3+2]));
    }
    return f_length*f_length;
}

MX objective::getnodelengthdisdiffall(Parm* parm, MX x, const std::vector<std::vector<double>>& jointposition, int musclenum, int local_mode_number, int use_p_variable, const std::vector<MX>& p_gamma_previous, const std::vector<MX>& p_mass_matrix){
    std::vector<std::vector<std::vector<MX>>> dataall=rearrange_gamma_eta(parm, x, musclenum);
    std::vector<std::vector<MX>> gammaallnode=dataall[0];
    muscle* Muscle=parm->getmuscleindex(musclenum);
    std::vector<double> massmatrix=getmassmatrix(Muscle, jointposition);

    double massmatrixsum=0.0;
    for(int j=0;j<massmatrix.size();j++){
        massmatrixsum=massmatrixsum+massmatrix[j];
    }
    massmatrixsum=massmatrixsum/massmatrix.size();
    if(massmatrixsum==0.0){massmatrixsum=1.0;}else{massmatrixsum=1.0/massmatrixsum;}

    MX lengthdiff_all;
    if(use_p_variable){
        std::vector<MX> nodediff=getnodedisdiff(Muscle, p_gamma_previous, gammaallnode, local_mode_number);
        MX f_mass=getobjectivemass(nodediff,p_mass_matrix);
        MX lengthdiff=getlengthdisdiff(p_gamma_previous, gammaallnode);
        lengthdiff_all=f_mass+lengthdiff*massmatrixsum*lengthconstant;
    } else {
        std::vector<MX> nodediff=getnodedisdiff(Muscle,gammaallnode, local_mode_number);
        MX f_mass=getobjectivemass(nodediff,massmatrix);
        MX lengthdiff=getlengthdisdiff(Muscle, gammaallnode);
        lengthdiff_all=f_mass+lengthdiff*massmatrixsum*lengthconstant;
    }
    

    return lengthdiff_all;
}

std::vector<MX> objective::getnodedisdiff(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int local_mode_number){
    std::vector<MX> nodediff;
    std::vector<double> gammalaststep=Muscle->getgamma_step(-1);
    std::vector<node*> allnodes = Muscle->get_allnodes();
    
    for(int n=0;n<allnodes.size();n++){
        if(local_mode_number==0){
            std::vector<std::vector<double>> q_ref = allnodes[n]->get_ref_init_body(-1)->getbodybasic()->getq();
            std::vector<double>  q_previous=q_ref[q_ref.size()-2];
            std::vector<double> position_previous;
            std::vector<std::vector<double>> axis_previous;
            for(int i=0; i<3; i++){
                position_previous.push_back(q_previous[i]);
                std::vector<double> axis1_previous;
                for (int j = 3+i*3; j < 6+i*3 ; j++) {
                    axis1_previous.push_back(q_previous[j]);
                }
                axis_previous.push_back(axis1_previous);
            }
            std::vector<double> position_previous_minus;
            for(int i=0; i<3; i++){
                position_previous_minus.push_back(gammalaststep[n*3+i]-position_previous[i]);
            }
            std::vector<double> local_position_previous;
            for(int i=0; i<axis_previous.size(); i++){
                double value=0;
                for(int j=0; j<axis_previous[0].size(); j++){
                    value=value+axis_previous[i][j]*position_previous_minus[j];
                }
                local_position_previous.push_back(value);
            }

            std::vector<double>  q_current=q_ref[q_ref.size()-1];
            std::vector<double> position_current;
            std::vector<std::vector<double>> axis_current;
            for(int i=0; i<3; i++){
                position_current.push_back(q_current[i]);
                std::vector<double> axis1_current;
                for (int j = 3+i*3; j < 6+i*3 ; j++) {
                    axis1_current.push_back(q_current[j]);
                }
                axis_current.push_back(axis1_current);
            }
            std::vector<MX> position_current_minus;
            for(int i=0; i<3; i++){
                position_current_minus.push_back(gammaallnode[n][i]-position_current[i]);
            }
            std::vector<MX> local_position_current;
            for(int i=0; i<axis_current.size(); i++){
                MX value=0;
                for(int j=0; j<axis_current[0].size(); j++){
                    value=value+axis_current[i][j]*position_current_minus[j];
                }
                local_position_current.push_back(value);
            }
            for(int i=0;i<3;i++){
                nodediff.push_back(local_position_current[i]-local_position_previous[i]);
            }
        }
        else{
            std::vector<std::vector<double>> q_ref = allnodes[n]->get_ref_body(-1)->getbodybasic()->getq();
            std::vector<double>  q_previous=q_ref[q_ref.size()-2];
            std::vector<double> position_previous;
            std::vector<std::vector<double>> axis_previous;
            for(int i=0; i<3; i++){
                position_previous.push_back(q_previous[i]);
                std::vector<double> axis1_previous;
                for (int j = 3+i*3; j < 6+i*3 ; j++) {
                    axis1_previous.push_back(q_previous[j]);
                }
                axis_previous.push_back(axis1_previous);
            }
            std::vector<double> position_previous_minus;
            for(int i=0; i<3; i++){
                position_previous_minus.push_back(gammalaststep[n*3+i]-position_previous[i]);
            }
            std::vector<double> local_position_previous;
            for(int i=0; i<axis_previous.size(); i++){
                double value=0;
                for(int j=0; j<axis_previous[0].size(); j++){
                    value=value+axis_previous[i][j]*position_previous_minus[j];
                }
                local_position_previous.push_back(value);
            }

            for(int i=0;i<3;i++){
                nodediff.push_back(gammaallnode[n][i]-local_position_previous[i]);
            }
        }
    }
    return nodediff;
}

std::vector<MX> objective::getnodedisdiff(muscle* Muscle, const std::vector<MX>& gammaprevious, const std::vector<std::vector<MX>>& gammaallnode, int local_mode_number){
    std::vector<MX> nodediff;
    std::vector<node*> allnodes = Muscle->get_allnodes();
    for(int n=0;n<allnodes.size();n++){
        if(local_mode_number==0){
            std::vector<std::vector<double>> q_ref = allnodes[n]->get_ref_init_body(-1)->getbodybasic()->getq();
            std::vector<double>  q_previous=q_ref[q_ref.size()-2];
            std::vector<double> position_previous;
            std::vector<std::vector<double>> axis_previous;
            for(int i=0; i<3; i++){
                position_previous.push_back(q_previous[i]);
                std::vector<double> axis1_previous;
                for (int j = 3+i*3; j < 6+i*3 ; j++) {
                    axis1_previous.push_back(q_previous[j]);
                }
                axis_previous.push_back(axis1_previous);
            }
            std::vector<MX> position_previous_minus;
            for(int i=0; i<3; i++){
                position_previous_minus.push_back(gammaprevious[n*3+i]-position_previous[i]);
            }
            std::vector<MX> local_position_previous;
            for(int i=0; i<axis_previous.size(); i++){
                MX value=0;
                for(int j=0; j<axis_previous[0].size(); j++){
                    value=value+axis_previous[i][j]*position_previous_minus[j];
                }
                local_position_previous.push_back(value);
            }

            std::vector<double>  q_current=q_ref[q_ref.size()-1];
            std::vector<double> position_current;
            std::vector<std::vector<double>> axis_current;
            for(int i=0; i<3; i++){
                position_current.push_back(q_current[i]);
                std::vector<double> axis1_current;
                for (int j = 3+i*3; j < 6+i*3 ; j++) {
                    axis1_current.push_back(q_current[j]);
                }
                axis_current.push_back(axis1_current);
            }
            std::vector<MX> position_current_minus;
            for(int i=0; i<3; i++){
                position_current_minus.push_back(gammaallnode[n][i]-position_current[i]);
            }
            std::vector<MX> local_position_current;
            for(int i=0; i<axis_current.size(); i++){
                MX value=0;
                for(int j=0; j<axis_current[0].size(); j++){
                    value=value+axis_current[i][j]*position_current_minus[j];
                }
                local_position_current.push_back(value);
            }
            for(int i=0;i<3;i++){
                nodediff.push_back(local_position_current[i]-local_position_previous[i]);
            }
        }
        else{
            std::vector<std::vector<double>> q_ref = allnodes[n]->get_ref_body(-1)->getbodybasic()->getq();
            std::vector<double>  q_previous=q_ref[q_ref.size()-2];
            std::vector<double> position_previous;
            std::vector<std::vector<double>> axis_previous;
            for(int i=0; i<3; i++){
                position_previous.push_back(q_previous[i]);
                std::vector<double> axis1_previous;
                for (int j = 3+i*3; j < 6+i*3 ; j++) {
                    axis1_previous.push_back(q_previous[j]);
                }
                axis_previous.push_back(axis1_previous);
            }
            std::vector<MX> position_previous_minus;
            for(int i=0; i<3; i++){
                position_previous_minus.push_back(gammaprevious[n*3+i]-position_previous[i]);
            }
            std::vector<MX> local_position_previous;
            for(int i=0; i<axis_previous.size(); i++){
                MX value=0;
                for(int j=0; j<axis_previous[0].size(); j++){
                    value=value+axis_previous[i][j]*position_previous_minus[j];
                }
                local_position_previous.push_back(value);
            }

            for(int i=0;i<3;i++){
                nodediff.push_back(gammaallnode[n][i]-local_position_previous[i]);
            }
        }
    }
    return nodediff;
}

std::vector<double> objective::getmassmatrix(muscle* Muscle, const std::vector<std::vector<double>>& jointposition){
    std::vector<double> gammalaststep=Muscle->getgamma_step(-1);
    std::vector<double> massmatrix;

    for(int j=0;j<Muscle->getnodenum();j++){
        if(casenum==0){
            for(int k=0;k<3;k++){
                massmatrix.push_back(0.0);
            }
        }
        if(casenum==1 || casenum==4){
            for(int k=0;k<3;k++){
                massmatrix.push_back(1.0);
            }
        }
        if(casenum==2 || casenum==3 || casenum==5){
            double massvalue=0.0;
            for(int k=0; k<jointposition.size();k++){
                double massvaluei=std::sqrt((gammalaststep[3*j+0]-jointposition[k][0])*(gammalaststep[3*j+0]-jointposition[k][0])+(gammalaststep[3*j+1]-jointposition[k][1])*(gammalaststep[3*j+1]-jointposition[k][1])+(gammalaststep[3*j+2]-jointposition[k][2])*(gammalaststep[3*j+2]-jointposition[k][2]));
                if(massvalue!=0){
                    double massvaluei_rev=1.0/massvaluei;
                    if(massvaluei_rev > massvalue){massvalue=massvaluei_rev;}
                }
                else{
                    massvalue=10000000;
                }
            }
            if(massvalue == 0.0){massvalue=1.0;}
            for(int k=0;k<3;k++){
                massmatrix.push_back(massvalue);
            }
        }            
    }

    return massmatrix;
}

MX objective::getobjectivemass(const std::vector<MX>& nodediff, const std::vector<double>& massmatrix){
    MX f_mass=0;
    for(int i=0;i<nodediff.size();i++){
        f_mass=f_mass+nodediff[i]*nodediff[i]*massmatrix[i];
    }
    return f_mass;
}

MX objective::getobjectivemass(const std::vector<MX>& nodediff, const std::vector<MX>& massmatrix){
    MX f_mass=0;
    for(int i=0;i<nodediff.size();i++){
        f_mass=f_mass+nodediff[i]*nodediff[i]*massmatrix[i];
    }
    return f_mass;
}

MX objective::getnodedisdiffall(Parm* parm, MX x, const std::vector<std::vector<double>>& jointposition, int musclenum, int local_mode_number, int use_p_variable, const std::vector<MX>& p_gamma_previous, const std::vector<MX>& p_mass_matrix){
    std::vector<std::vector<std::vector<MX>>> dataall=rearrange_gamma_eta(parm, x, musclenum);
    std::vector<std::vector<MX>> gammaallnode=dataall[0];
    muscle* Muscle=parm->getmuscleindex(musclenum);
    MX f_mass;
    if(use_p_variable){
        std::vector<MX> nodediff=getnodedisdiff(Muscle, p_gamma_previous, gammaallnode, local_mode_number);
        f_mass=getobjectivemass(nodediff,p_mass_matrix);
        return f_mass;
    }else{
        std::vector<MX> nodediff=getnodedisdiff(Muscle,gammaallnode, local_mode_number);
        std::vector<double> massmatrix=getmassmatrix(Muscle, jointposition);
        f_mass=getobjectivemass(nodediff,massmatrix);
    }  
    return f_mass;  
}

std::vector<std::vector<std::vector<MX>>> objective::rearrange_gamma_eta(Parm* parm, MX x, int musclenum){
    std::vector<std::vector<std::vector<MX>>> dataall;
    std::vector<std::vector<MX>> gammaallnodemuscle;
    std::vector<std::vector<MX>> etamuscle;
    std::vector<std::vector<MX>> eta_via_muscle;
    muscle* Muscle=parm->getmuscleindex(musclenum);
    std::vector<node*> allnodes = Muscle->get_allnodes();
    int fix_point_num=0;
    for(int j=0;j<allnodes.size();j++){
        std::vector<MX> gammaallnodemuscle1;            
        for(int k=0;k<3;k++){
            gammaallnodemuscle1.push_back(x(j*3+k));
        }
        gammaallnodemuscle.push_back(gammaallnodemuscle1);  
        if(!allnodes[j]->get_fixpoint()){
            std::vector<MX> etamuscle1;
            for(int k=0;k<Muscle->get_consider_bodynum(parm->getn_bodies());k++){
                etamuscle1.push_back(x(Muscle->getnodenum()*3+(j-fix_point_num)*Muscle->get_consider_bodynum(parm->getn_bodies())+k));
            }
            etamuscle.push_back(etamuscle1);
        } else {
            fix_point_num=fix_point_num+1;
        }            
    }

    dataall.push_back(gammaallnodemuscle);
    dataall.push_back(etamuscle);

    return dataall;
}

std::vector<MX>  objective::getnode_offset_dis(muscle* Muscle, const std::vector<std::vector<MX>>& gammaallnode, int local_mode_number){
    std::vector<MX> node_offset_diff;
    std::vector<double> gammalaststep=Muscle->getgamma_step(-1);
    std::vector<node*> allnodes = Muscle->get_allnodes();
    int fix_point_num=0;
    
    for(int n=0;n<allnodes.size();n++){
        if(allnodes[n]->get_fixpoint()){
            fix_point_num=fix_point_num+1;
        } else {
            if(local_mode_number==0){
                std::vector<std::vector<double>> q_ref = allnodes[n]->get_ref_init_body(-1)->getbodybasic()->getq();
                std::vector<double>  q_previous=q_ref[q_ref.size()-2];
                std::vector<double> position_previous;
                std::vector<std::vector<double>> axis_previous;
                for(int i=0; i<3; i++){
                    position_previous.push_back(q_previous[i]);
                    std::vector<double> axis1_previous;
                    for (int j = 3+i*3; j < 6+i*3 ; j++) {
                        axis1_previous.push_back(q_previous[j]);
                    }
                    axis_previous.push_back(axis1_previous);
                }
                std::vector<double> position_vector_left={gammalaststep[(n-1)*3]-gammalaststep[n*3], gammalaststep[(n-1)*3+1]-gammalaststep[n*3+1], gammalaststep[(n-1)*3+2]-gammalaststep[n*3+2]};
                std::vector<double> position_vector_right={gammalaststep[(n+1)*3]-gammalaststep[n*3], gammalaststep[(n+1)*3+1]-gammalaststep[n*3+1], gammalaststep[(n+1)*3+2]-gammalaststep[n*3+2]};
                std::vector<double> position_vector_cross={position_vector_left[1]*position_vector_right[2]-position_vector_left[2]*position_vector_right[1], position_vector_left[2]*position_vector_right[0]-position_vector_left[0]*position_vector_right[2], position_vector_left[0]*position_vector_right[1]-position_vector_left[1]*position_vector_right[0]};
                double position_vector_cross_norm=std::sqrt(position_vector_cross[0]*position_vector_cross[0]+position_vector_cross[1]*position_vector_cross[1]+position_vector_cross[2]*position_vector_cross[2]);
                std::vector<double> position_vector_cross_normalized={position_vector_cross[0]/position_vector_cross_norm, position_vector_cross[1]/position_vector_cross_norm, position_vector_cross[2]/position_vector_cross_norm};

                std::vector<double> position_position_vector_cross_normalized_minus;
                for(int i=0; i<3; i++){
                    position_position_vector_cross_normalized_minus.push_back(position_vector_cross_normalized[i]-position_previous[i]);
                }
                std::vector<double> local_position_unit_norm;
                for(int i=0; i<axis_previous.size(); i++){
                    double value=0;
                    for(int j=0; j<axis_previous[0].size(); j++){
                        value=value+axis_previous[i][j]*position_position_vector_cross_normalized_minus[j];
                    }
                    local_position_unit_norm.push_back(value);
                }

                std::vector<double> position_previous_minus;
                for(int i=0; i<3; i++){
                    position_previous_minus.push_back(gammalaststep[n*3+i]-position_previous[i]);
                }
                std::vector<double> local_position_previous;
                for(int i=0; i<axis_previous.size(); i++){
                    double value=0;
                    for(int j=0; j<axis_previous[0].size(); j++){
                        value=value+axis_previous[i][j]*position_previous_minus[j];
                    }
                    local_position_previous.push_back(value);
                }

                std::vector<double>  q_current=q_ref[q_ref.size()-1];
                std::vector<double> position_current;
                std::vector<std::vector<double>> axis_current;
                for(int i=0; i<3; i++){
                    position_current.push_back(q_current[i]);
                    std::vector<double> axis1_current;
                    for (int j = 3+i*3; j < 6+i*3 ; j++) {
                        axis1_current.push_back(q_current[j]);
                    }
                    axis_current.push_back(axis1_current);
                }
                std::vector<MX> position_current_minus;
                for(int i=0; i<3; i++){
                    position_current_minus.push_back(gammaallnode[n][i]-position_current[i]);
                }
                std::vector<MX> local_position_current;
                for(int i=0; i<axis_current.size(); i++){
                    MX value=0;
                    for(int j=0; j<axis_current[0].size(); j++){
                        value=value+axis_current[i][j]*position_current_minus[j];
                    }
                    local_position_current.push_back(value);
                }
                MX node_offset_diff1=0;
                for(int i=0;i<3;i++){
                    node_offset_diff1 = node_offset_diff1 + (local_position_current[i]-local_position_previous[i])*local_position_unit_norm[i];
                }
                node_offset_diff1 = fabs(node_offset_diff1);
                node_offset_diff.push_back(node_offset_diff1);
            }
            else{
                std::vector<std::vector<double>> q_ref = allnodes[n]->get_ref_body(-1)->getbodybasic()->getq();
                std::vector<double>  q_previous=q_ref[q_ref.size()-2];
                std::vector<double> position_previous;
                std::vector<std::vector<double>> axis_previous;
                for(int i=0; i<3; i++){
                    position_previous.push_back(q_previous[i]);
                    std::vector<double> axis1_previous;
                    for (int j = 3+i*3; j < 6+i*3 ; j++) {
                        axis1_previous.push_back(q_previous[j]);
                    }
                    axis_previous.push_back(axis1_previous);
                }
                std::vector<double> position_vector_left={gammalaststep[(n-1)*3]-gammalaststep[n*3], gammalaststep[(n-1)*3+1]-gammalaststep[n*3+1], gammalaststep[(n-1)*3+2]-gammalaststep[n*3+2]};
                std::vector<double> position_vector_right={gammalaststep[(n+1)*3]-gammalaststep[n*3], gammalaststep[(n+1)*3+1]-gammalaststep[n*3+1], gammalaststep[(n+1)*3+2]-gammalaststep[n*3+2]};
                std::vector<double> position_vector_cross={position_vector_left[1]*position_vector_right[2]-position_vector_left[2]*position_vector_right[1], position_vector_left[2]*position_vector_right[0]-position_vector_left[0]*position_vector_right[2], position_vector_left[0]*position_vector_right[1]-position_vector_left[1]*position_vector_right[0]};
                double position_vector_cross_norm=std::sqrt(position_vector_cross[0]*position_vector_cross[0]+position_vector_cross[1]*position_vector_cross[1]+position_vector_cross[2]*position_vector_cross[2]);
                std::vector<double> position_vector_cross_normalized={position_vector_cross[0]/position_vector_cross_norm, position_vector_cross[1]/position_vector_cross_norm, position_vector_cross[2]/position_vector_cross_norm};

                std::vector<double> position_position_vector_cross_normalized_minus;
                for(int i=0; i<3; i++){
                    position_position_vector_cross_normalized_minus.push_back(position_vector_cross_normalized[i]-position_previous[i]);
                }
                std::vector<double> local_position_unit_norm;
                for(int i=0; i<axis_previous.size(); i++){
                    double value=0;
                    for(int j=0; j<axis_previous[0].size(); j++){
                        value=value+axis_previous[i][j]*position_position_vector_cross_normalized_minus[j];
                    }
                    local_position_unit_norm.push_back(value);
                }

                std::vector<double> position_previous_minus;
                for(int i=0; i<3; i++){
                    position_previous_minus.push_back(gammalaststep[n*3+i]-position_previous[i]);
                }
                std::vector<double> local_position_previous;
                for(int i=0; i<axis_previous.size(); i++){
                    double value=0;
                    for(int j=0; j<axis_previous[0].size(); j++){
                        value=value+axis_previous[i][j]*position_previous_minus[j];
                    }
                    local_position_previous.push_back(value);
                }
                MX node_offset_diff1=0;
                for(int i=0;i<3;i++){
                    node_offset_diff1 = node_offset_diff1 + (gammaallnode[n][i]-local_position_previous[i])*local_position_unit_norm[i];
                }
                node_offset_diff1 = fabs(node_offset_diff1);
                node_offset_diff.push_back(node_offset_diff1);
            }
        }
    }
    return node_offset_diff;
}

std::vector<MX>  objective::getnode_offset_dis(muscle* Muscle, const std::vector<MX>& gammaprevious, const std::vector<std::vector<MX>>& gammaallnode, int local_mode_number){
    std::vector<MX> node_offset_diff;
    std::vector<node*> allnodes = Muscle->get_allnodes();
    int fix_point_num=0;
    
    for(int n=0;n<allnodes.size();n++){
        if(allnodes[n]->get_fixpoint()){
            fix_point_num=fix_point_num+1;
        } else {
            if(local_mode_number==0){
                std::vector<std::vector<double>> q_ref = allnodes[n]->get_ref_init_body(-1)->getbodybasic()->getq();
                std::vector<double>  q_previous=q_ref[q_ref.size()-2];
                std::vector<double> position_previous;
                std::vector<std::vector<double>> axis_previous;
                for(int i=0; i<3; i++){
                    position_previous.push_back(q_previous[i]);
                    std::vector<double> axis1_previous;
                    for (int j = 3+i*3; j < 6+i*3 ; j++) {
                        axis1_previous.push_back(q_previous[j]);
                    }
                    axis_previous.push_back(axis1_previous);
                }
                std::vector<MX> position_vector_left={gammaprevious[(n-1)*3]-gammaprevious[n*3], gammaprevious[(n-1)*3+1]-gammaprevious[n*3+1], gammaprevious[(n-1)*3+2]-gammaprevious[n*3+2]};
                std::vector<MX> position_vector_right={gammaprevious[(n+1)*3]-gammaprevious[n*3], gammaprevious[(n+1)*3+1]-gammaprevious[n*3+1], gammaprevious[(n+1)*3+2]-gammaprevious[n*3+2]};
                std::vector<MX> position_vector_cross={position_vector_left[1]*position_vector_right[2]-position_vector_left[2]*position_vector_right[1], position_vector_left[2]*position_vector_right[0]-position_vector_left[0]*position_vector_right[2], position_vector_left[0]*position_vector_right[1]-position_vector_left[1]*position_vector_right[0]};
                MX position_vector_cross_norm=MX::sqrt(position_vector_cross[0]*position_vector_cross[0]+position_vector_cross[1]*position_vector_cross[1]+position_vector_cross[2]*position_vector_cross[2]);
                std::vector<MX> position_vector_cross_normalized={position_vector_cross[0]/position_vector_cross_norm, position_vector_cross[1]/position_vector_cross_norm, position_vector_cross[2]/position_vector_cross_norm};

                std::vector<MX> position_position_vector_cross_normalized_minus;
                for(int i=0; i<3; i++){
                    position_position_vector_cross_normalized_minus.push_back(position_vector_cross_normalized[i]-position_previous[i]);
                }
                std::vector<MX> local_position_unit_norm;
                for(int i=0; i<axis_previous.size(); i++){
                    MX value=0;
                    for(int j=0; j<axis_previous[0].size(); j++){
                        value=value+axis_previous[i][j]*position_position_vector_cross_normalized_minus[j];
                    }
                    local_position_unit_norm.push_back(value);
                }

                std::vector<MX> position_previous_minus;
                for(int i=0; i<3; i++){
                    position_previous_minus.push_back(gammaprevious[n*3+i]-position_previous[i]);
                }
                std::vector<MX> local_position_previous;
                for(int i=0; i<axis_previous.size(); i++){
                    MX value=0;
                    for(int j=0; j<axis_previous[0].size(); j++){
                        value=value+axis_previous[i][j]*position_previous_minus[j];
                    }
                    local_position_previous.push_back(value);
                }

                std::vector<double>  q_current=q_ref[q_ref.size()-1];
                std::vector<double> position_current;
                std::vector<std::vector<double>> axis_current;
                for(int i=0; i<3; i++){
                    position_current.push_back(q_current[i]);
                    std::vector<double> axis1_current;
                    for (int j = 3+i*3; j < 6+i*3 ; j++) {
                        axis1_current.push_back(q_current[j]);
                    }
                    axis_current.push_back(axis1_current);
                }
                std::vector<MX> position_current_minus;
                for(int i=0; i<3; i++){
                    position_current_minus.push_back(gammaallnode[n][i]-position_current[i]);
                }
                std::vector<MX> local_position_current;
                for(int i=0; i<axis_current.size(); i++){
                    MX value=0;
                    for(int j=0; j<axis_current[0].size(); j++){
                        value=value+axis_current[i][j]*position_current_minus[j];
                    }
                    local_position_current.push_back(value);
                }
                MX node_offset_diff1=0;
                for(int i=0;i<3;i++){
                    node_offset_diff1 = node_offset_diff1 + (local_position_current[i]-local_position_previous[i])*local_position_unit_norm[i];
                }
                node_offset_diff1 = fabs(node_offset_diff1);
                node_offset_diff.push_back(node_offset_diff1);
            }
            else{
                std::vector<std::vector<double>> q_ref = allnodes[n]->get_ref_body(-1)->getbodybasic()->getq();
                std::vector<double>  q_previous=q_ref[q_ref.size()-2];
                std::vector<double> position_previous;
                std::vector<std::vector<double>> axis_previous;
                for(int i=0; i<3; i++){
                    position_previous.push_back(q_previous[i]);
                    std::vector<double> axis1_previous;
                    for (int j = 3+i*3; j < 6+i*3 ; j++) {
                        axis1_previous.push_back(q_previous[j]);
                    }
                    axis_previous.push_back(axis1_previous);
                }
                std::vector<MX> position_vector_left={gammaprevious[(n-1)*3]-gammaprevious[n*3], gammaprevious[(n-1)*3+1]-gammaprevious[n*3+1], gammaprevious[(n-1)*3+2]-gammaprevious[n*3+2]};
                std::vector<MX> position_vector_right={gammaprevious[(n+1)*3]-gammaprevious[n*3], gammaprevious[(n+1)*3+1]-gammaprevious[n*3+1], gammaprevious[(n+1)*3+2]-gammaprevious[n*3+2]};
                std::vector<MX> position_vector_cross={position_vector_left[1]*position_vector_right[2]-position_vector_left[2]*position_vector_right[1], position_vector_left[2]*position_vector_right[0]-position_vector_left[0]*position_vector_right[2], position_vector_left[0]*position_vector_right[1]-position_vector_left[1]*position_vector_right[0]};
                MX position_vector_cross_norm=MX::sqrt(position_vector_cross[0]*position_vector_cross[0]+position_vector_cross[1]*position_vector_cross[1]+position_vector_cross[2]*position_vector_cross[2]);
                std::vector<MX> position_vector_cross_normalized={position_vector_cross[0]/position_vector_cross_norm, position_vector_cross[1]/position_vector_cross_norm, position_vector_cross[2]/position_vector_cross_norm};

                std::vector<MX> position_position_vector_cross_normalized_minus;
                for(int i=0; i<3; i++){
                    position_position_vector_cross_normalized_minus.push_back(position_vector_cross_normalized[i]-position_previous[i]);
                }
                std::vector<MX> local_position_unit_norm;
                for(int i=0; i<axis_previous.size(); i++){
                    MX value=0;
                    for(int j=0; j<axis_previous[0].size(); j++){
                        value=value+axis_previous[i][j]*position_position_vector_cross_normalized_minus[j];
                    }
                    local_position_unit_norm.push_back(value);
                }

                std::vector<MX> position_previous_minus;
                for(int i=0; i<3; i++){
                    position_previous_minus.push_back(gammaprevious[n*3+i]-position_previous[i]);
                }
                std::vector<MX> local_position_previous;
                for(int i=0; i<axis_previous.size(); i++){
                    MX value=0;
                    for(int j=0; j<axis_previous[0].size(); j++){
                        value=value+axis_previous[i][j]*position_previous_minus[j];
                    }
                    local_position_previous.push_back(value);
                }

                MX node_offset_diff1=0;
                for(int i=0;i<3;i++){
                    node_offset_diff1 = node_offset_diff1 + (gammaallnode[n][i]-local_position_previous[i])*local_position_unit_norm[i];
                }
                node_offset_diff1 = fabs(node_offset_diff1);
                node_offset_diff.push_back(node_offset_diff1);
            }
        }
    }
    return node_offset_diff;
}

MX objective::getnode_offset_dis_all(Parm* parm, MX x, const std::vector<std::vector<double>>& jointposition, int musclenum, int local_mode_number, int use_p_variable, const std::vector<MX>& p_gamma_previous, const std::vector<MX>& p_mass_matrix){
    std::vector<std::vector<std::vector<MX>>> dataall=rearrange_gamma_eta(parm, x, musclenum);
    std::vector<std::vector<MX>> gammaallnode=dataall[0];
    muscle* Muscle=parm->getmuscleindex(musclenum);
    MX f_mass;
    if(use_p_variable){
        std::vector<MX> nodediff=getnode_offset_dis(Muscle, p_gamma_previous, gammaallnode, local_mode_number);
        MX f_mass=0;
        for(int i=0;i<nodediff.size();i++){
            f_mass=f_mass+nodediff[i]*nodediff[i]*p_mass_matrix[3*i];
        }
    }else{
        std::vector<MX> nodediff=getnode_offset_dis(Muscle,gammaallnode, local_mode_number);
        std::vector<double> massmatrix=getmassmatrix(Muscle, jointposition);
        MX f_mass=0;
        for(int i=0;i<nodediff.size();i++){
            f_mass=f_mass+nodediff[i]*nodediff[i]*massmatrix[3*i];
        }
    }  
    return f_mass;  
}

MX objective::getobjective(Parm* parm, MX x, const std::vector<std::vector<double>>& jointposition, int musclenum, int local_mode_number, int use_p_variable, const std::vector<MX>& p_gamma_previous, const std::vector<MX>& p_mass_matrix){
    switch(casenum){
        case 0:
            f = 0;
            break;
        case 1:
            f = getnodedisdiffall(parm, x, jointposition, musclenum, local_mode_number, use_p_variable, p_gamma_previous, p_mass_matrix);
            break;
        case 2:
            f = getnodedisdiffall(parm, x, jointposition, musclenum, local_mode_number, use_p_variable, p_gamma_previous, p_mass_matrix);
            break;
        case 3:
            f = getnodelengthdisdiffall(parm, x, jointposition, musclenum, local_mode_number, use_p_variable, p_gamma_previous, p_mass_matrix);
            break;
        case 4:
            f = getnode_offset_dis_all(parm, x, jointposition, musclenum, local_mode_number, use_p_variable, p_gamma_previous, p_mass_matrix);
            break;
        case 5:
            f = getnode_offset_dis_all(parm, x, jointposition, musclenum, local_mode_number, use_p_variable, p_gamma_previous, p_mass_matrix);
            break;
    }
    return f;
}
