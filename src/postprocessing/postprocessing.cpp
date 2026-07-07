#include "postprocessing.h"
#include <algorithm>
#include <numeric>

postprocessing::postprocessing(){

}

postprocessing::~postprocessing(){
    
}

void postprocessing::do_postprocessingall(Parm* parm){
    get_force_allmuscle(parm);
    get_momentarmall(parm);
    get_hill_passive_force_allmuscle(parm);
    get_hill_active_force_allmuscle(parm);
    get_hill_total_force_allmuscle(parm);
    get_hill_moment_allmuscle(parm);
}

void postprocessing::get_momentarmall(Parm* parm){
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    std::vector<joint*> alljoint=parm->getalljoint();
    std::vector<std::vector<std::vector<double>>> momentarmallnoderes;
    std::vector<std::vector<double>> momentarmallres;
    for(int i=0;i<allmuscle.size();i++){
        for(int j=0;j<alljoint.size();j++){
            if(alljoint[j]->getwritemomentarm()){
                std::vector<std::vector<double>> gammaall=get_momentarm_per_muscle(allmuscle[i]->getgammaall(),alljoint[j]);
                momentarmallnoderes.push_back(gammaall);
                std::vector<double> moment_re;
                for(int k=0;k<gammaall.size();k++){
                    if(gammaall[k].empty()){
                        moment_re.push_back(0.0);
                        continue;
                    }
                    double minimum_abs=fabs(gammaall[k][0]);
                    double minimum_no_abs=gammaall[k][0];
                    for(int l=1;l<gammaall[k].size();l++){
                        if(fabs(gammaall[k][l])<minimum_abs){
                            minimum_abs=fabs(gammaall[k][l]);
                            minimum_no_abs=gammaall[k][l];
                        }
                    }
                    moment_re.push_back(minimum_no_abs);
                }
                momentarmallres.push_back(moment_re);
            }
        }
    }
    momentarmall=momentarmallres;
    momentarmnodeall=momentarmallnoderes;
}

std::vector<std::vector<double>> postprocessing::get_momentarm_per_muscle(const std::vector<std::vector<double>>& gamma, joint* Joint){
    std::vector<std::vector<double>> gammaall;
    for(int i=1;i<gamma.size();i++){
        std::vector<double> gammaallnode=get_momentarm_per_node(gamma[i],Joint,i-1);
        gammaall.push_back(gammaallnode);
    }
    return gammaall;
}

std::vector<double> postprocessing::get_momentarm_per_node(const std::vector<double>& gamma, joint* Joint, int step){
    int gammasize=gamma.size()/3;
    std::vector<double> gammaallnode;
    if(gammasize<=0 || Joint==nullptr){
        return gammaallnode;
    }
    std::vector<std::vector<double>> movement_each_value=Joint->get_movement_per_step();
    std::vector<std::vector<double>> joint_absolute_axisvector=Joint->getabsolute_axisvector();
    std::vector<std::vector<double>> joint_absolute_pos=Joint->getabsolute_pos();
    if(joint_absolute_axisvector.empty() || joint_absolute_pos.empty()){
        return std::vector<double>(gammasize, 0.0);
    }
    int maxStep=std::min(static_cast<int>(joint_absolute_axisvector.size()), static_cast<int>(joint_absolute_pos.size()))-1;
    if(maxStep<0){
        return std::vector<double>(gammasize, 0.0);
    }
    if(step<0){
        step=0;
    }
    if(step>maxStep){
        step=maxStep;
    }
    if(joint_absolute_axisvector[step].size()<3 || joint_absolute_pos[step].size()<3){
        return std::vector<double>(gammasize, 0.0);
    }
    for(int i=0;i<gammasize;i++){
        std::vector<double> forcedir;
        if(i==0){
            forcedir=vector3minus({gamma[3*(i+1)],gamma[3*(i+1)+1],gamma[3*(i+1)+2]}, {gamma[3*i],gamma[3*i+1],gamma[3*i+2]});
        }
        else if(i==gammasize-1){
            forcedir=vector3minus({gamma[3*i],gamma[3*i+1],gamma[3*i+2]}, {gamma[3*(i-1)],gamma[3*(i-1)+1],gamma[3*(i-1)+2]});
        }
        else{
            forcedir=vector3minus({gamma[3*(i+1)],gamma[3*(i+1)+1],gamma[3*(i+1)+2]}, {gamma[3*(i-1)],gamma[3*(i-1)+1],gamma[3*(i-1)+2]});
        }
        double forcedir_value=-1.0*std::sqrt(vectortime1(forcedir,forcedir)); //-1.0 direction from insertion to origin
        if(std::abs(forcedir_value)<1e-12){
            gammaallnode.push_back(0.0);
            continue;
        }
        std::vector<double> forcedir_unit=vector3timeconstant(forcedir,1.0/forcedir_value);

        double angle_vhange_value=1.0;
        if(!movement_each_value.empty() && !movement_each_value[0].empty()){
            int movementStep=std::min(step, static_cast<int>(movement_each_value[0].size())-1);
            if(movement_each_value.size()<2 && movement_each_value[0][movementStep]<0){
                angle_vhange_value=-1.0;
            }
        }

        double axis_value=angle_vhange_value*std::sqrt(vectortime1(joint_absolute_axisvector[step],joint_absolute_axisvector[step]));
        if(std::abs(axis_value)<1e-12){
            gammaallnode.push_back(0.0);
            continue;
        }
        std::vector<double> axis_unit=vector3timeconstant(joint_absolute_axisvector[step],1.0/axis_value);
        std::vector<double> r_cross_F=crossProduct(vector3minus({gamma[3*i],gamma[3*i+1],gamma[3*i+2]}, joint_absolute_pos[step]),forcedir_unit);

        double gamma_one_node_res=vectortime1(r_cross_F,axis_unit);
        gammaallnode.push_back(gamma_one_node_res);
    }
    return gammaallnode;    
}

void postprocessing::get_force_allmuscle(Parm* parm){
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    std::vector<std::vector<double>> totalforceallvalue;
    std::vector<std::vector<std::vector<double>>> forceallnodevalue;
    get_length_allmuscle(parm);
    getphiall(parm);
    for(int i=0;i<allmuscle.size();i++){
        std::vector<double> totalforceallvalue1;
        std::vector<std::vector<double>> forceallnodevalue1;
        std::vector<std::vector<std::vector<double>>> force_each_muscle=get_force_each_muscle(parm, i);
        for(int j=0;j<force_each_muscle.size();j++){
            std::vector<double> totalforce_eachstep={0.0,0.0,0.0};
            std::vector<double> forceallnodevalue11;
            for(int k=0;k<force_each_muscle[j].size();k++){
                totalforce_eachstep=vector3plus(totalforce_eachstep,force_each_muscle[j][k]);
                double force_node_value=std::sqrt(vectortime1(force_each_muscle[j][k],force_each_muscle[j][k]));
                forceallnodevalue11.push_back(force_node_value);
            }
            forceallnodevalue1.push_back(forceallnodevalue11);
            double total_force_value=std::sqrt(vectortime1(totalforce_eachstep,totalforce_eachstep));
            totalforceallvalue1.push_back(total_force_value);
        }
        forceallnodevalue.push_back(forceallnodevalue1);
        totalforceallvalue.push_back(totalforceallvalue1);
    }
    totalforceall=totalforceallvalue;
    forceallnode=forceallnodevalue;
}

std::vector<std::vector<std::vector<double>>> postprocessing::get_force_each_muscle(Parm* parm, int musclenum){
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    std::vector<std::vector<double>> gammaallvalue=allmuscle[musclenum]->getgammaall();
    std::vector<std::vector<std::vector<double>>> force_eachmuscle;
    for(int i=1;i<gammaallvalue.size();i++){
        double initiallength=sumvector(lengthall[musclenum][1]);
        double lengthchange=(sumvector(lengthall[musclenum][i])-initiallength)/initiallength;
        std::vector<std::vector<double>> phicurrentall;
        for(int j=0;j<parm->getn_bodies();j++){
            phicurrentall.push_back(phiall[musclenum*parm->getn_bodies()+j][i]);
        }
        std::vector<std::vector<double>> force_eachmuscle_eachstep= get_force_each_muscle_each_time(gammaallvalue[i], phicurrentall, lengthchange);
        force_eachmuscle.push_back(force_eachmuscle_eachstep);
    }
    return force_eachmuscle;
}

std::vector<std::vector<double>> postprocessing::get_force_each_muscle_each_time(const std::vector<double>& gamma, const std::vector<std::vector<double>>& phi, double lengthchange){
    int gammasize=gamma.size()/3;
    std::vector<std::vector<double>> gammareshape;
    for(int i=0;i<gammasize;i++){
        std::vector<double> gammareshapei;
        gammareshapei.push_back(gamma[3*i]);
        gammareshapei.push_back(gamma[3*i+1]);
        gammareshapei.push_back(gamma[3*i+2]);
        gammareshape.push_back(gammareshapei);
    }

    std::vector<int> contactphi(gammasize,0);
    for(int i=0;i<phi.size();i++){
        for(int j=0;j<phi[0].size();j++){
            if(phi[i][j]<=tol){
                contactphi[j]+=1;
            }
        }
    }
    std::vector<std::vector<double>> forcedirall;
    for(int i=0;i<gammasize-1;i++){
        std::vector<double> forcedir;
        if(contactphi[i+1] || contactphi[i]){
            std::vector<double> tan_gamma_right;
            std::vector<double> tan_gamma_left;
            if(i==0){
                tan_gamma_right=vector3minus(gammareshape[0],gammareshape[1]);
                tan_gamma_left=vector3minus(gammareshape[2],gammareshape[0]);
            }
            else if(i==gammasize-2){
                tan_gamma_right=vector3minus(gammareshape[gammasize-3],gammareshape[gammasize-1]);
                tan_gamma_left=vector3minus(gammareshape[gammasize-1],gammareshape[gammasize-2]);
            }
            else{
                tan_gamma_right=vector3minus(gammareshape[i-1],gammareshape[i+1]);
                tan_gamma_left=vector3minus(gammareshape[i+2],gammareshape[i]);
            }
            double tan_gamma_right_norm=std::sqrt(vectortime1(tan_gamma_right,tan_gamma_right));
            double tan_gamma_left_norm=std::sqrt(vectortime1(tan_gamma_left,tan_gamma_left));
            if(lengthchange>0){
                forcedir=vector3timeconstant(vector3plus(vector3timeconstant(tan_gamma_right, 1.0/tan_gamma_right_norm), vector3timeconstant(tan_gamma_left, 1.0/tan_gamma_left_norm)),lengthchange*E*area);
            }
            else{
                forcedir={0.0,0.0,0.0};
            }
        }
        else{
            forcedir={0.0,0.0,0.0};
        }
        forcedirall.push_back(forcedir);
    }

    return forcedirall;
}

void postprocessing::get_length_allmuscle(Parm* parm){
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    std::vector<std::vector<std::vector<double>>> lengthallres;

    for(int i=0;i<allmuscle.size();i++){
        std::vector<std::vector<double>> lengthallvalue=get_length_each_muscle(allmuscle[i]->getgammaall());
        lengthallres.push_back(lengthallvalue);
    }
    lengthall=lengthallres;
}

std::vector<std::vector<double>> postprocessing::get_length_each_muscle(const std::vector<std::vector<double>>& gamma){
    std::vector<std::vector<double>> lengthallvalue;
    for(int i=0;i<gamma.size();i++){
        std::vector<double> lengthallvaluenode=get_length_each_muscle_each_node(gamma[i]);
        lengthallvalue.push_back(lengthallvaluenode);
    }
    return lengthallvalue;
}

std::vector<double> postprocessing::get_length_each_muscle_each_node(const std::vector<double>& gamma){
    int gammasize=gamma.size()/3;
    std::vector<std::vector<double>> gammareshape;
    for(int i=0;i<gammasize;i++){
        std::vector<double> gammareshapei;
        gammareshapei.push_back(gamma[3*i]);
        gammareshapei.push_back(gamma[3*i+1]);
        gammareshapei.push_back(gamma[3*i+2]);
        gammareshape.push_back(gammareshapei);
    }
    std::vector<double> lengthallvalue;
    for(int i=0;i<gammasize-1;i++){
        std::vector<double> lengthminus=vector3minus(gammareshape[i+1], gammareshape[i]);
        double length=std::sqrt(vectortime1(lengthminus,lengthminus));
        lengthallvalue.push_back(length);
    }
    return lengthallvalue;
}

void postprocessing::get_hill_passive_force_allmuscle(Parm* parm){
    hillpassiveforceall.clear();
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    for(muscle* Muscle : allmuscle){
        std::vector<double> hillPar = Muscle->get_hill_parameter();
        hillPar.resize(3, 0.0);
        if(hillPar[0]!=0.0 && hillPar[1]!=0.0 && hillPar[2]!=0.0){
            hillpassiveforceall.push_back(get_hill_passive_force_each_muscle(Muscle->getgammaall(), hillPar));
        } else{
            hillpassiveforceall.push_back({});
        }
    }
}

std::vector<double> postprocessing::get_hill_passive_force_each_muscle(const std::vector<std::vector<double>>& gamma, const std::vector<double>& hill_par){
    std::vector<double> force;
    const double Fmax = hill_par[0];
    const double Lopt = hill_par[1];
    const double k = 6.0;
    const double denominator = std::exp(k) - 1.0;
    std::vector<std::vector<double>> lengthValue = get_length_each_muscle(gamma);
    for(const std::vector<double>& lengthStep : lengthValue){
        const double l = std::accumulate(lengthStep.begin(), lengthStep.end(), 0.0);
        double force_value = Fmax * (std::exp(k * (l - Lopt) / Lopt) - 1.0) / denominator;
        if(force_value < 0.0){
            force_value = 0.0;
        }
        force.push_back(force_value);
    }
    return force;
}

void postprocessing::get_hill_active_force_allmuscle(Parm* parm){
    hillactiveforceall.clear();
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    for(muscle* Muscle : allmuscle){
        std::vector<double> hillPar = Muscle->get_hill_parameter();
        hillPar.resize(3, 0.0);
        if(hillPar[0]!=0.0 && hillPar[1]!=0.0 && hillPar[2]!=0.0){
            hillactiveforceall.push_back(get_hill_active_force_each_muscle(Muscle->getgammaall(), hillPar));
        } else{
            hillactiveforceall.push_back({});
        }
    }
}

std::vector<double> postprocessing::get_hill_active_force_each_muscle(const std::vector<std::vector<double>>& gamma, const std::vector<double>& hill_par){
    std::vector<double> force;
    const double Fmax = hill_par[0];
    const double Lopt = hill_par[1];
    const double L0 = hill_par[2];
    const double width = Lopt - L0;
    if(width == 0.0){
        return force;
    }
    const double fv = 1.0; // v=0 and u=1
    const double u = 1.0;
    std::vector<std::vector<double>> lengthValue = get_length_each_muscle(gamma);
    for(const std::vector<double>& lengthStep : lengthValue){
        const double l = std::accumulate(lengthStep.begin(), lengthStep.end(), 0.0);
        double fl = 1.0 - ((l - Lopt) * (l - Lopt)) / (width * width);
        if (fl < 0.0) {
            fl = 0.0;
        }
        force.push_back(Fmax * fl * fv * u);
    }
    return force;
}

void postprocessing::get_hill_total_force_allmuscle(Parm* parm){
    hilltotalforceall.clear();
    const int musclenum = parm->getn_muscles();
    for(int i=0;i<musclenum;i++){
        if(i >= static_cast<int>(hillpassiveforceall.size()) || i >= static_cast<int>(hillactiveforceall.size()) || hillpassiveforceall[i].empty() || hillactiveforceall[i].empty()){
            hilltotalforceall.push_back({});
            continue;
        }
        std::vector<double> total;
        const int stepCount = std::min(hillpassiveforceall[i].size(), hillactiveforceall[i].size());
        for(int j=0;j<stepCount;j++){
            total.push_back(hillpassiveforceall[i][j] + hillactiveforceall[i][j]);
        }
        hilltotalforceall.push_back(total);
    }
}

void postprocessing::get_hill_moment_allmuscle(Parm* parm){
    hillmomentall.clear();
    const int writeMomentJointCount = std::max(1, parm->get_write_moment_joints());
    for(int i=0;i<static_cast<int>(hilltotalforceall.size());i++){
        if(hilltotalforceall[i].empty()){
            hillmomentall.push_back({});
            continue;
        }
        const int momentIndex = i * writeMomentJointCount;
        if(momentIndex >= static_cast<int>(momentarmall.size()) || momentarmall[momentIndex].empty()){
            hillmomentall.push_back({});
            continue;
        }
        std::vector<double> moment;
        const int stepCount = std::min(hilltotalforceall[i].size(), momentarmall[momentIndex].size());
        for(int j=0;j<stepCount;j++){
            moment.push_back(hilltotalforceall[i][j] * momentarmall[momentIndex][j]);
        }
        hillmomentall.push_back(moment);
    }
}

void postprocessing::getphiall(Parm* parm){
    std::vector<body*> allbody=parm->getallbody();
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    std::vector<std::vector<std::vector<double>>> phiallres;
    for(int i=0;i<allmuscle.size();i++){
        for(int j=0;j<parm->getn_bodies();j++){
            std::vector<std::vector<double>> gammaall=phi_shape_all(allmuscle[i]->getgammaall(),allbody[j+1]);
            phiallres.push_back(gammaall);
        }
    }
    phiall=phiallres;
}

std::vector<std::vector<double>> postprocessing::phi_shape_all(const std::vector<std::vector<double>>& gamma, body* Body){
    std::vector<std::vector<double>> gammaall;
    for(int i=0;i<gamma.size();i++){
        std::vector<double> gammaallnode=phi_shape_allnode(gamma[i],Body,i);
        gammaall.push_back(gammaallnode);
    }
    return gammaall;
}

std::vector<double> postprocessing::phi_shape_allnode(const std::vector<double>& gamma, body* Body, int timenum){
    int gammasize=gamma.size()/3;
    std::vector<double> gammaallnode;
    for(int i=0;i<gammasize;i++){
        double gammaonenodere=phi_shape({gamma[3*i],gamma[3*i+1],gamma[3*i+2]}, Body, timenum);
        gammaallnode.push_back(gammaonenodere);
    }
    return gammaallnode;
}

double postprocessing::phi_shape(const std::vector<double>& gamma, body* Body, int timenum){
    return Body->phi_shape(gamma, timenum);
}

void postprocessing::settol(double tolvalue){
    tol=tolvalue;
}

double postprocessing::gettol(){
    return tol;
}

std::vector<std::vector<std::vector<double>>> postprocessing::getphiall(){
    return phiall;
}

std::vector<std::vector<std::vector<double>>> postprocessing::getlengthall(){
    return lengthall;
}

std::vector<std::vector<double>> postprocessing::gettotalforceall(){
    return totalforceall;
}
    
std::vector<std::vector<std::vector<double>>> postprocessing::getforceallnode(){
    return forceallnode;
}

std::vector<std::vector<double>> postprocessing::getmomentarmall(){
    return momentarmall;
}

std::vector<std::vector<std::vector<double>>> postprocessing::getmomentarmnodeall(){
    return momentarmnodeall;
}

std::vector<std::vector<double>> postprocessing::gethillpassiveforceall(){
    return hillpassiveforceall;
}

std::vector<std::vector<double>> postprocessing::gethillactiveforceall(){
    return hillactiveforceall;
}

std::vector<std::vector<double>> postprocessing::gethilltotalforceall(){
    return hilltotalforceall;
}

std::vector<std::vector<double>> postprocessing::gethillmomentall(){
    return hillmomentall;
}
