#include "postprocessing.h"

postprocessing::postprocessing(){

}

postprocessing::~postprocessing(){
    
}

void postprocessing::do_postprocessingall(Parm* parm){
    get_force_allmuscle(parm);
    get_momentarmall(parm);
}

void postprocessing::get_momentarmall(Parm* parm){
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    std::vector<joint*> alljoint=parm->getalljoint();
    std::vector<std::vector<std::vector<double>>> momentarmallnoderes;
    std::vector<std::vector<double>> momentarmallres;
    for(int i=0;i<allmuscle.size();i++){
        for(int j=0;j<alljoint.size();j++){
            std::vector<std::vector<double>> gammaall=get_momentarm_per_muscle(allmuscle[i]->getgammaall(),alljoint[j]);
            momentarmallnoderes.push_back(gammaall);
            std::vector<double> moment_re;
            for(int k=0;k<gammaall.size();k++){
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
    momentarmall=momentarmallres;
    momentarmnodeall=momentarmallnoderes;
}

std::vector<std::vector<double>> postprocessing::get_momentarm_per_muscle(const std::vector<std::vector<double>>& gamma, joint* Joint){
    std::vector<std::vector<double>> gammaall;
    for(int i=0;i<gamma.size();i++){
        std::vector<double> gammaallnode=get_momentarm_per_node(gamma[i],Joint);
        gammaall.push_back(gammaallnode);
    }
    return gammaall;
}

std::vector<double> postprocessing::get_momentarm_per_node(const std::vector<double>& gamma, joint* Joint){
    int gammasize=gamma.size()/3;
    std::vector<double> gammaallnode;
    if(Joint->getjoint_type() =="revolute joint"){
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
            std::vector<double> forcedir_unit=vector3timeconstant(forcedir,1.0/forcedir_value);
            std::vector<double> rotationangle=Joint->getrotationangle();
            std::vector<double> initialrotationangle=Joint->getinitialrotationangle();
            double angle_vhange_value=(rotationangle[0]-initialrotationangle[0])/fabs((rotationangle[0]-initialrotationangle[0]));
            double axis_value=angle_vhange_value*std::sqrt(vectortime1(Joint->getabsolute_axisvector(),Joint->getabsolute_axisvector()));
            std::vector<double> axis_unit=vector3timeconstant(Joint->getabsolute_axisvector(),1.0/axis_value);

            std::vector<double> r_cross_F=crossProduct(vector3minus({gamma[3*i],gamma[3*i+1],gamma[3*i+2]}, Joint->getabsolute_pos()),forcedir_unit);
            double gamma_one_node_res=vectortime1(r_cross_F,axis_unit);
            gammaallnode.push_back(gamma_one_node_res);
        }
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
    std::vector<std::vector<double>>  qall=Body->getbodybasic()->getq();
    std::vector<double>q = qall[timenum];
    std::vector<double> x;
    for (int i = 0; i < 3; i++) {
        x.push_back(gamma[i] - q[i]);
    }
    std::vector<std::vector<double>> axis;
    for(int i=0; i<3; i++){
        std::vector<double> axis1;
        for(int j=0; j<3; j++){
            axis1.push_back(q[3+3*i+j]);
        }
        axis.push_back(axis1);
    }

    std::vector<double> positionlocal=matrix33time31tog(axis,x);

    double phi=0;
    if(Body->getshape()->getshapename()=="ellipsoid"){
        phi = phi_ellipsoid(positionlocal,Body);
    }
    if(Body->getshape()->getshapename()=="cylinder"){
        phi = phi_ellipsoid(positionlocal,Body);
    }
    return phi;
}

double postprocessing::phi_ellipsoid(const std::vector<double>& positionlocal, body* Body){
    double phi = positionlocal[0] * positionlocal[0] / (Body->getshape()->geta()*Body->getshape()->geta()) +
        positionlocal[1] * positionlocal[1] / (Body->getshape()->getb()*Body->getshape()->getb()) +
        positionlocal[2] * positionlocal[2]  / (Body->getshape()->getc()*Body->getshape()->getc()) - 1;
    return phi;
}

double postprocessing::phi_cylinder(const std::vector<double>& positionlocal, body* Body){
    if(positionlocal[2]>= -1.0*Body->getshape()->getc() && positionlocal[2] <= Body->getshape()->getc()){
        double phi = positionlocal[0] * positionlocal[0] / (Body->getshape()->geta()*Body->getshape()->geta()) +
        positionlocal[1] * positionlocal[1] / (Body->getshape()->getb()*Body->getshape()->getb()) - 1;
        return phi;
    }
    else{
        if(positionlocal[2]< -1.0*Body->getshape()->getc() >= -1.0*Body->getshape()->getc()){
            return  -1.0*Body->getshape()->getc()-positionlocal[2];
        }
        else{
            return positionlocal[2]-Body->getshape()->getc();
        }
    }
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
