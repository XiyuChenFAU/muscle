/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "IO.h"
#include <algorithm>
#include <map>
#include <utility>

IO::IO(){
    
}

IO::~IO(){
    
}

void IO::write2DvalueToFile(const std::vector<std::vector<double>>& value, std::ofstream& file, const std::string& name, const std::string& typenamevalue){
    int rownum=1;
    if (file.is_open()) {
        for (int i=0;i<value[0].size();i++) {
            file << name << "\t";
            file << typenamevalue << "\t";
            file << rownum << "\t";
            for (int j=0;j<value.size();j++) {
                if(write_precision){file << std::setprecision(17) << value[j][i] << "\t";} 
                else {file << value[j][i] << "\t";}
            }
            file << "\n";
            rownum++;
        }
    } else {
        std::cerr << "can not open file" << std::endl;
    }
}

void IO::write2DintvalueToFile(const std::vector<std::vector<int>>& value, std::ofstream& file, const std::string& name, const std::string& typenamevalue){
    int rownum=1;
    if (file.is_open()) {
        for (int i=0;i<value[0].size();i++) {
            file << name << "\t";
            file << typenamevalue << "\t";
            file << rownum << "\t";
            for (int j=0;j<value.size();j++) {
                file << value[j][i] << "\t";
            }
            file << "\n";
            rownum++;
        }
    } else {
        std::cerr << "can not open file" << std::endl;
    }
}

void IO::writemusclebodyresultToFileAll(model* Model){

    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<body*> allbody=Model->getparm()->getallbody();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<int> rotation={};
    int num_steps = Model->getparm()->getbodyindex(-1)->getbodybasic()->getq().size()-1;
    for(int i=0;i<num_steps;i++){rotation.push_back(i);}
    
    //writebody
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_body_result.txt";
    std::ofstream file1(filename);
    //write titel
    file1 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
    for(int i=0;i<rotation.size();i++){file1 << rotation[i] << "\t";}
    file1 << "\n";
    //writevalue
    for(int i =0;i<allbody.size();i++){
        write2DvalueToFile(allbody[i]->getbodybasic()->getq(),file1,allbody[i]->getname(),"q");
    }
    file1.close();

    //writemuscle
    filename = folderoutput+"/"+Model->getmodelname()+"_muscle_gamma_result.txt";
    std::ofstream file2(filename);
    //write titel
    file2 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
    for(int i=0;i<rotation.size();i++){file2 << rotation[i] << "\t";}
    file2 << "\n";
    //writevalue
    for(int i =0;i<allmuscle.size();i++){
        write2DvalueToFile(allmuscle[i]->getgammaall(),file2,allmuscle[i]->getname(),"gamma");
    }
    file2.close();

    //writemuscle
    filename = folderoutput+"/"+Model->getmodelname()+"_muscle_eta_result.txt";
    std::ofstream file3(filename);
    //write titel
    file3 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
    for(int i=0;i<rotation.size();i++){file3 << rotation[i] << "\t";}
    file3 << "\n";
    //writevalue

    for(int i =0;i<allmuscle.size();i++){
        write2DvalueToFile(allmuscle[i]->getetaall(),file3,allmuscle[i]->getname(),"eta");
    }
    file3.close();
}

void IO::writeanalyzeresultToFileAll(model* Model){

    writephiToFile(Model);
    writelengthToFile(Model);
    writelengthallToFile(Model);
    writeforcenodeToFile(Model);
    writetotalforceToFile(Model);
    writebody_stateToFile(Model);
    writemomentarmnodeToFile(Model);
    writemomentarmToFile(Model);
    writehillToFile(Model);
    writerunningtimeToFile(Model);
    writerefbodyToFile(Model);
}

void IO::writephiToFile(model* Model){

    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<std::vector<double>>> phiall = Model->getPostprocessing()->getphiall();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<body*> allbody=Model->getparm()->getallbody();
    std::vector<int> rotation={};
    int num_steps = Model->getparm()->getbodyindex(-1)->getbodybasic()->getq().size()-1;
    for(int i=0;i<num_steps;i++){rotation.push_back(i);}
    

    //phiall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_phi_result.txt";
    std::ofstream file1(filename);
    //write titel
    file1 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
    for(int i=0;i<rotation.size();i++){file1 << rotation[i] << "\t";}
    file1 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        for(int j =0;j<Model->getparm()->getn_bodies();j++){
            write2DvalueToFile(phiall[i*Model->getparm()->getn_bodies()+j],file1,allmuscle[i]->getname()+"-"+allbody[j+1]->getname(),"phi");
        }
    }
    file1.close();
}

void IO::writelengthToFile(model* Model){

    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<std::vector<double>>> lengthall = Model->getPostprocessing()->getlengthall();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<int> rotation={};
    int num_steps = Model->getparm()->getbodyindex(-1)->getbodybasic()->getq().size()-1;
    for(int i=0;i<num_steps;i++){rotation.push_back(i);}

    //lengthall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_length_result.txt";
    std::ofstream file2(filename);
    //write titel
    file2 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
    for(int i=0;i<rotation.size();i++){file2 << rotation[i] << "\t";}
    file2 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        write2DvalueToFile(lengthall[i],file2,allmuscle[i]->getname(),"length");
    }
    file2.close();
}

void IO::writelengthallToFile(model* Model){

    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<std::vector<double>>> lengthall = Model->getPostprocessing()->getlengthall();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<int> rotation={};
    int num_steps = Model->getparm()->getbodyindex(-1)->getbodybasic()->getq().size()-1;
    for(int i=0;i<num_steps;i++){rotation.push_back(i);}

    //lengthall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_length_total_result.txt";
    std::ofstream file2(filename);
    //write titel
    file2 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
    for(int i=0;i<rotation.size();i++){file2 << rotation[i] << "\t";}
    file2 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        file2 << allmuscle[i]->getname() << "\t"<<"length"<<"\t"<<" "<<"\t";
        std::vector<double> total_length_per_muscle={};
        for(int j=0;j<lengthall[i].size();j++){
            double total_length=0.0;
            for(int k=0;k<lengthall[i][j].size();k++){
                total_length=total_length+lengthall[i][j][k];
            }
            file2 << total_length << "\t";
        }
        file2 << "\n";
    }
    file2.close();
}

void IO::writeforcenodeToFile(model* Model){

    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<std::vector<double>>> forceallnode = Model->getPostprocessing()->getforceallnode();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<int> rotation={};
    int num_steps = Model->getparm()->getbodyindex(-1)->getbodybasic()->getq().size()-1;
    for(int i=0;i<num_steps;i++){rotation.push_back(i);}

    //forceallnode
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_forcenode_result.txt";
    std::ofstream file3(filename);
    //write titel
    file3 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t";
    for(int i=0;i<rotation.size();i++){file3 << rotation[i] << "\t";}
    file3 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        write2DvalueToFile(forceallnode[i],file3,allmuscle[i]->getname(),"forcenode");
    }
    file3.close();
}

void IO::writetotalforceToFile(model* Model){

    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<double>> totalforceall = Model->getPostprocessing()->gettotalforceall();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<int> rotation={};
    int num_steps = Model->getparm()->getbodyindex(-1)->getbodybasic()->getq().size()-1;
    for(int i=0;i<num_steps;i++){rotation.push_back(i);}

    //totalforceall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_totalforce_result.txt";
    std::ofstream file4(filename);
    //write titel
    file4 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t";
    for(int i=0;i<rotation.size();i++){file4 << rotation[i] << "\t";}
    file4 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        file4 << allmuscle[i]->getname() << "\t";
        file4 << "totalforce" << "\t";
        file4 << 1 << "\t";
        for (int j=0;j<totalforceall[i].size();j++) {
            if(write_precision){file4 << std::setprecision(17) << totalforceall[i][j] << "\t";} 
            else {file4 << totalforceall[i][j] << "\t";}
        }
        file4 << "\n";
    }
    file4.close();
}


void IO::writebody_stateToFile(model* Model){

    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<body*> allbody=Model->getparm()->getallbody();
    std::vector<int> rotation={};
    int num_steps = Model->getparm()->getbodyindex(-1)->getbodybasic()->getq().size()-1;
    for(int i=0;i<num_steps;i++){rotation.push_back(i);}

    //body_state
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_bodystate_result.txt";
    std::ofstream file5(filename);
    //write titel
    file5 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
    for(int i=0;i<rotation.size();i++){file5 << rotation[i] << "\t";}
    file5 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_bodies();i++){
        file5 << allbody[i+1]->getname() << "\t" << allbody[i+1]->getshape()->getshapename()<<"\ta\t";
        for (int j=0;j<allbody[i+1]->getbodybasic()->getaxisangle_ref().size();j++) {
            file5 << allbody[i+1]->getshape()->geta() << "\t";
        }
        file5 << "\n";
        file5 << allbody[i+1]->getname() << "\t" << allbody[i+1]->getshape()->getshapename()<<"\tb\t";
        for (int j=0;j<allbody[i+1]->getbodybasic()->getaxisangle_ref().size();j++) {
            file5 << allbody[i+1]->getshape()->getb() << "\t";
        }
        file5 << "\n";
        file5 << allbody[i+1]->getname() << "\t" << allbody[i+1]->getshape()->getshapename()<<"\tc\t";
        for (int j=0;j<allbody[i+1]->getbodybasic()->getaxisangle_ref().size();j++) {
            file5 << allbody[i+1]->getshape()->getc() << "\t";
        }
        file5 << "\n";
        file5 << allbody[i+1]->getname() << "\t" << allbody[i+1]->getshape()->getshapename()<<"\trotationaxis1\t";
        for (int j=0;j<allbody[i+1]->getbodybasic()->getaxisangle_ref().size();j++) {
            std::vector<std::vector<double>> axisanglevalue=allbody[i+1]->getbodybasic()->getaxisangle_ref();
            file5 << axisanglevalue[j][0] << "\t";
        }
        file5 << "\n";
        file5 << allbody[i+1]->getname() << "\t" << allbody[i+1]->getshape()->getshapename()<<"\trotationaxis2\t";
        for (int j=0;j<allbody[i+1]->getbodybasic()->getaxisangle_ref().size();j++) {
            std::vector<std::vector<double>> axisanglevalue=allbody[i+1]->getbodybasic()->getaxisangle_ref();
            file5 << axisanglevalue[j][1] << "\t";
        }
        file5 << "\n";
        file5 << allbody[i+1]->getname() << "\t" << allbody[i+1]->getshape()->getshapename()<<"\trotationaxis3\t";
        for (int j=0;j<allbody[i+1]->getbodybasic()->getaxisangle_ref().size();j++) {
            std::vector<std::vector<double>> axisanglevalue=allbody[i+1]->getbodybasic()->getaxisangle_ref();
            file5 << axisanglevalue[j][2] << "\t";
        }
        file5 << "\n";
        file5 << allbody[i+1]->getname() << "\t" << allbody[i+1]->getshape()->getshapename()<<"\trotationangle\t";
        for (int j=0;j<allbody[i+1]->getbodybasic()->getaxisangle_ref().size();j++) {
            std::vector<std::vector<double>> axisanglevalue=allbody[i+1]->getbodybasic()->getaxisangle_ref();
            file5 << axisanglevalue[j][3] << "\t";
        }
        file5 << "\n";
        file5 << allbody[i+1]->getname() << "\t" << allbody[i+1]->getshape()->getshapename()<<"\ttranslationaxis1\t";
        for (int j=0;j<allbody[i+1]->getbodybasic()->getq().size();j++) {
            std::vector<std::vector<double>> transationq=allbody[i+1]->getbodybasic()->getq();
            file5 << transationq[j][0] << "\t";
        }
        file5 << "\n";
        file5 << allbody[i+1]->getname() << "\t" << allbody[i+1]->getshape()->getshapename()<<"\ttranslationaxis2\t";
        for (int j=0;j<allbody[i+1]->getbodybasic()->getq().size();j++) {
            std::vector<std::vector<double>> transationq=allbody[i+1]->getbodybasic()->getq();
            file5 << transationq[j][1] << "\t";
        }
        file5 << "\n";
        file5 << allbody[i+1]->getname() << "\t" << allbody[i+1]->getshape()->getshapename()<<"\ttranslationaxis3\t";
        for (int j=0;j<allbody[i+1]->getbodybasic()->getq().size();j++) {
            std::vector<std::vector<double>> transationq=allbody[i+1]->getbodybasic()->getq();
            file5 << transationq[j][2] << "\t";
        }
        file5 << "\n";
    }
    file5.close();
}

void IO::writemomentarmnodeToFile(model* Model){

    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<std::vector<double>>> momentarmnodeall = Model->getPostprocessing()->getmomentarmnodeall();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<joint*> alljoint=Model->getparm()->getalljoint();
    std::vector<int> rotation={};
    int num_steps = Model->getparm()->getbodyindex(-1)->getbodybasic()->getq().size()-1;
    for(int i=0;i<num_steps;i++){rotation.push_back(i);}

    //phiall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_momentarmnode_result.txt";
    std::ofstream file6(filename);
    //write titel
    file6 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t";
    for(int i=0;i<rotation.size();i++){file6 << rotation[i] << "\t";}
    file6 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        int joint_index=0;
        for(int j =0;j<Model->getparm()->getn_joints();j++){
            if(alljoint[j]->getwritemomentarm()){
                write2DvalueToFile(momentarmnodeall[i*(Model->getparm()->get_write_moment_joints())+joint_index],file6,allmuscle[i]->getname()+"-"+alljoint[j]->getname(),"moment_arm_node");
                joint_index++;
            }
        }
    }
    file6.close();
}

void IO::writemomentarmToFile(model* Model){

    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }
    std::vector<std::vector<double>> momentarmall = Model->getPostprocessing()->getmomentarmall();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<joint*> alljoint=Model->getparm()->getalljoint();
    std::vector<int> rotation={};
    int num_steps = Model->getparm()->getbodyindex(-1)->getbodybasic()->getq().size()-1;
    for(int i=0;i<num_steps;i++){rotation.push_back(i);}
    

    //phiall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_momentarm_result.txt";
    std::ofstream file7(filename);
    //write titel
    file7 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t";
    for(int i=0;i<rotation.size();i++){file7 << rotation[i] << "\t";}
    file7 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        int joint_index=0;
        for(int j =0;j<Model->getparm()->getn_joints();j++){
            if(alljoint[j]->getwritemomentarm()){
                file7 << allmuscle[i]->getname()+"-"+alljoint[j]->getname() << "\t"<<"moment_arm"<<"\t"<<1<<"\t";
                for(int k =0;k<momentarmall[i*Model->getparm()->get_write_moment_joints()+joint_index].size();k++){
                    if(write_precision){file7 << std::setprecision(17) << momentarmall[i*Model->getparm()->get_write_moment_joints()+joint_index][k]<<"\t";} 
                    else {file7 << momentarmall[i*Model->getparm()->get_write_moment_joints()+joint_index][k]<<"\t";}
                }
                file7 << "\n";
                joint_index++;
            }
        }
    }
    file7.close();
}

void IO::writehillToFile(model* Model){
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<int> rotation={};
    int num_steps = Model->getparm()->getbodyindex(-1)->getbodybasic()->getq().size()-1;
    for(int i=0;i<num_steps;i++){rotation.push_back(i);}
    std::vector<std::pair<std::string, std::vector<std::vector<double>>>> hillFiles = {
        {"hill_passive_force", Model->getPostprocessing()->gethillpassiveforceall()},
        {"hill_active_force", Model->getPostprocessing()->gethillactiveforceall()},
        {"hill_total_force", Model->getPostprocessing()->gethilltotalforceall()},
        {"hill_moment", Model->getPostprocessing()->gethillmomentall()}
    };

    for(const auto &hillFile : hillFiles){
        bool hasValue = false;
        for(const auto &line : hillFile.second){
            if(!line.empty()){
                hasValue = true;
                break;
            }
        }
        if(!hasValue){
            continue;
        }

        std::string filename = folderoutput+"/"+Model->getmodelname()+"_"+hillFile.first+"_result.txt";
        std::ofstream file(filename);
        file << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
        for(int i=0;i<rotation.size();i++){file << rotation[i] << "\t";}
        file << "\n";

        for(int i=0;i<static_cast<int>(hillFile.second.size()) && i<static_cast<int>(allmuscle.size());i++){
            if(hillFile.second[i].empty()){
                continue;
            }
            file << allmuscle[i]->getname() << "\t" << hillFile.first << "\t" << 1 << "\t";
            for(double value : hillFile.second[i]){
                if(write_precision){file << std::setprecision(17) << value << "\t";}
                else {file << value << "\t";}
            }
            file << "\n";
        }
        file.close();
    }
}

void IO::writerunningtimeToFile(model* Model){
    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::string filename = folderoutput+"/"+Model->getmodelname()+"_runningtime_result.txt";
    std::ofstream file8(filename);
    //write titel
    if(write_precision){file8 << "run_time" << std::setprecision(17) << Model->get_elapsed_time() <<"\n";} 
    else {file8 << "run_time" << Model->get_elapsed_time() <<"\n";}
    file8.close();
}

void IO::writerefbodyToFile(model* Model){
    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<body*> allbody=Model->getparm()->getallbody();
    std::vector<int> rotation={};
    int num_steps = Model->getparm()->getbodyindex(-1)->getbodybasic()->getq().size()-1;
    for(int i=0;i<num_steps;i++){rotation.push_back(i);}

    //lengthall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_ref_body_result.txt";
    std::ofstream file9(filename);
    //write titel
    file9 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
    for(int i=0;i<rotation.size();i++){file9 << rotation[i] << "\t";}
    file9 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        write2DintvalueToFile(matrixtranspose(allmuscle[i]->getrefbody_all(allbody, Model->getSolveeq()->getConstraint()->get_local_mode_number(), Model->getSolveeq()->getInitialguess()->getmode_nr())),file9,allmuscle[i]->getname(),"ref_body");
    }
    file9.close();
}

void IO::writejson(model* Model, int write_gamma, int currentstepnum){

    Model->writejson(write_gamma, currentstepnum);

    /*
    //create folder
    std::string folderoutput="";

    folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    Json::Value root;
    root["name"] = Model->getmodelname();

    Json::Value bodyArray(Json::arrayValue);
    std::vector<body*> allbody=Model->getparm()->getallbody();
    for (int i=0;i<Model->getparm()->getn_bodies();i++) {
        Json::Value bodyObject;
        bodyObject["body_name"] = allbody[i+1]->getname();
        bodyObject["body_parent_name"] = allbody[i+1]->getparent()->getname();
        if(write_gamma){
            std::vector<double> localvalue=allbody[i+1]->get_local_axis_angle_pos(currentstepnum);
            std::vector<double> naxisvalue={localvalue[0],localvalue[1],localvalue[2]};
            Json::Value naxis(Json::arrayValue);
            for (const auto& value : naxisvalue) {
                naxis.append(value);
            }
            bodyObject["rotation_axis_relative_parent_coordinate"] = naxis;
            bodyObject["rotation_angle"] = localvalue[3];
            std::vector<double> rhobodyvalue={localvalue[4],localvalue[5],localvalue[6]};
            Json::Value rhobody(Json::arrayValue);
            for (const auto& value : rhobodyvalue) {
                rhobody.append(value);
            }
            bodyObject["position_relative_parent_coordinate"] = rhobody;
        } else {
            Json::Value naxis(Json::arrayValue);
            for (const auto& value : allbody[i+1]->getbodybasic()->getinitialsetting_naxis()) {
                naxis.append(value);
            }
            bodyObject["rotation_axis_relative_parent_coordinate"] = naxis;
            bodyObject["rotation_angle"] = allbody[i+1]->getbodybasic()->getinitialsetting_angle();
            Json::Value rhobody(Json::arrayValue);
            for (const auto& value : allbody[i+1]->getbodybasic()->getrhobody()) {
                rhobody.append(value);
            }
            bodyObject["position_relative_parent_coordinate"] = rhobody;
        }
        Json::Value shapeObject;
        shapeObject["shape_name"] = allbody[i+1]->getshape()->getshapename();
        shapeObject["a"] = allbody[i+1]->getshape()->geta();
        shapeObject["b"] = allbody[i+1]->getshape()->getb();
        shapeObject["c"] = allbody[i+1]->getshape()->getc();
        shapeObject["length"] = allbody[i+1]->getshape()->getlength();
        shapeObject["radius"] = allbody[i+1]->getshape()->getradius();
        bodyObject["shape"] = shapeObject;
        bodyArray.append(bodyObject);
    }
    root["body"] = bodyArray;

    Json::Value muscleArray(Json::arrayValue);
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    for(int i=0;i<Model->getparm()->getn_muscles();i++){
        Json::Value muscleObject;
        muscleObject["muscle_name"]=allmuscle[i]->getname();
        Json::Value rhoo(Json::arrayValue);
        for (const auto& value : allmuscle[i]->getrho_o()) {
            rhoo.append(value);
        }
        muscleObject["rho_origin"]=rhoo;
        muscleObject["origin_relative_body"]=allmuscle[i]->getrhoo_bodyname();
        Json::Value rhoi(Json::arrayValue);
        for (const auto& value : allmuscle[i]->getrho_i()) {
            rhoi.append(value);
        }
        muscleObject["rho_insertion"]=rhoi;
        muscleObject["insertion_relative_body"]=allmuscle[i]->getrhoi_bodyname();
        muscleObject["node_number"]=allmuscle[i]->getnodenum();
        std::vector<double> hillPar = allmuscle[i]->get_hill_parameter();
        hillPar.resize(3, 0.0);
        Json::Value hillObject;
        hillObject["Fmax"] = hillPar[0];
        hillObject["Lopt"] = hillPar[1];
        hillObject["L0"] = hillPar[2];
        muscleObject["hill"] = hillObject;

        if(write_gamma){
            std::vector<std::vector<double>> gammaall = allmuscle[i]->getgammaall();
            Json::Value gamma(Json::arrayValue);
            for (const auto& value : gammaall[currentstepnum]) {
                gamma.append(value);
            }
            muscleObject["gamma"] = gamma;

            std::vector<std::vector<double>> etaall = allmuscle[i]->getetaall();
            Json::Value eta(Json::arrayValue);
            for (const auto& value : etaall[currentstepnum]) {
                eta.append(value);
            }
            muscleObject["eta"] = eta;
        } 
        muscleArray.append(muscleObject);

    }
    root["muscle"] = muscleArray;

    Json::Value jointArray(Json::arrayValue);
    for (int i=0;i<Model->getparm()->getn_joints();i++) {

        Json::Value jointObject;
        jointObject["joint_name"] = Model->getparm()->getjointindex(i)->getname();
        jointObject["rotate_body_name"] = Model->getparm()->getjointindex(i)->getbodyname();
        jointObject["joint_type_name"] = Model->getparm()->getjointindex(i)->getjoint_type();
        Json::Value relative_posvalue(Json::arrayValue);
        for (const auto& value : Model->getparm()->getjointindex(i)->getrelative_pos()) {
            relative_posvalue.append(value);
        }
        jointObject["position_relative_rotate_body"] = relative_posvalue;
        Json::Value axisvectorvalue(Json::arrayValue);
        for (const auto& value : Model->getparm()->getjointindex(i)->getaxisvector()) {
            axisvectorvalue.append(value);
        }
        jointObject["rotation_axis_relative_rotate_body"] = axisvectorvalue;

        Json::Value move_setting_value(Json::arrayValue);
        for (const auto& matrix : Model->getparm()->getjointindex(i)->get_move_setting()) {
            Json::Value matrixJson(Json::arrayValue);
            for (const auto& row : matrix) {
                Json::Value rowJson(Json::arrayValue);
                for (const auto& value : row) {
                    rowJson.append(value);
                }
                matrixJson.append(rowJson);
            }
            move_setting_value.append(matrixJson);
        }
        jointObject["move_setting"] = move_setting_value;

        if(Model->getparm()->getjointindex(i)->getread_from_movement()){
            Json::Value movement_value(Json::arrayValue);
            for (const auto& row : Model->getparm()->getjointindex(i)->get_movement()) {
                Json::Value rowJson(Json::arrayValue);
                for (const auto& value : row) {
                    rowJson.append(value);
                }
                movement_value.append(rowJson);
            }
            jointObject["movement"] = movement_value;
        }

        jointArray.append(jointObject);
    }
    root["joint"] = jointArray;

    Json::Value ipopt;
    ipopt["tol"]=Model->getSolveeq()->getipopt()->gettol();
    ipopt["max_iter"]=Model->getSolveeq()->getipopt()->getmax_iter();
    ipopt["linear_solver"]=Model->getSolveeq()->getipopt()->getlinear_solver();
    ipopt["print_level"]=Model->getSolveeq()->getipopt()->getprint_level();
    ipopt["hessian_approximation"]=Model->getSolveeq()->getipopt()->gethessian_approximation();
    root["ipoptsetting"] = ipopt;

    Json::Value objective;
    objective["solvercase"] = Model->getSolveeq()->getObjective()->getcasenum();
    objective["length_constant"] = Model->getSolveeq()->getObjective()->getlengthconstant();
    root["objective"]=objective;

    Json::Value initial;
    initial["mode_number"] = Model->getSolveeq()->getInitialguess()->getmode_nr();
    initial["select_body"] = Model->getSolveeq()->getInitialguess()->getselect_bodyname();
    initial["collision_check"] = Model->getSolveeq()->getInitialguess()->getcollision_check();
    root["initial_guess"]=initial;

    Json::Value postprocessing;
    postprocessing["tol"]=Model->getPostprocessing()->gettol();
    root["postprocessing"] = postprocessing;

    root["save_interval"] = Model->get_save_interval();
    root["output_path"] = Model->getfolderpath();

    // change json object to string
    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, root);
    std::string filename = "";
    if(write_gamma){
        filename = Model->getfolderpath()+"output_"+Model->getmodelname()+"/"+Model->getmodelname()+"_step_"+std::to_string(currentstepnum-1)+".json";
    }
    else{
        filename = Model->getfolderpath()+"output_"+Model->getmodelname()+"/"+Model->getmodelname()+".json";
    }

    std::ofstream file(filename);
    file << jsonString;
    file.close();
    */
}

namespace {
std::map<std::string, std::vector<std::vector<double>>> readResultTable(const std::filesystem::path& filename){
    std::map<std::string, std::vector<std::vector<double>>> componentRows;
    std::ifstream file(filename);
    if(!file.is_open()){
        return {};
    }
    std::string line;
    std::getline(file, line);
    while(std::getline(file, line)){
        if(line.empty()){
            continue;
        }
        std::stringstream ss(line);
        std::string name;
        std::string type;
        int rownum=0;
        ss >> name >> type >> rownum;
        if(name.empty()){
            continue;
        }
        std::vector<double> values;
        double value=0.0;
        while(ss >> value){
            values.push_back(value);
        }
        if(!values.empty()){
            componentRows[name].push_back(values);
        }
    }

    std::map<std::string, std::vector<std::vector<double>>> result;
    for(const auto& item : componentRows){
        int componentCount=static_cast<int>(item.second.size());
        int stepCount=0;
        for(const auto& row : item.second){
            stepCount=std::max(stepCount, static_cast<int>(row.size()));
        }
        std::vector<std::vector<double>> history(stepCount, std::vector<double>(componentCount, 0.0));
        for(int component=0; component<componentCount; component++){
            for(int step=0; step<item.second[component].size(); step++){
                history[step][component]=item.second[component][step];
            }
        }
        result[item.first]=history;
    }
    return result;
}

void rebuildJointAbsoluteHistory(model* Model, int historySize){
    if(Model==nullptr || historySize<=0){
        return;
    }
    std::vector<body*> allbody=Model->getparm()->getallbody();
    std::vector<joint*> alljoint=Model->getparm()->getalljoint();
    for(joint* Joint : alljoint){
        Joint->resetforrecalc();
    }
    for(int step=0; step<historySize; step++){
        for(body* Body : allbody){
            std::vector<std::vector<double>> qHistory=Body->getbodybasic()->getq();
            if(step<static_cast<int>(qHistory.size()) && qHistory[step].size()>=12){
                Body->getbodybasic()->setpoistionaxis(qHistory[step]);
            }
        }
        for(joint* Joint : alljoint){
            Joint->absolute_pos_axis_update(step);
        }
    }
    const int lastStep=historySize-1;
    for(body* Body : allbody){
        std::vector<std::vector<double>> qHistory=Body->getbodybasic()->getq();
        if(lastStep<static_cast<int>(qHistory.size()) && qHistory[lastStep].size()>=12){
            Body->getbodybasic()->setpoistionaxis(qHistory[lastStep]);
        }
    }
}
}

model* IO::readmodelwithresultfolder(const std::string& resultfolder){
    std::filesystem::path folder(resultfolder);
    if(folder.empty()){
        return nullptr;
    }
    if(std::filesystem::is_regular_file(folder)){
        folder=folder.parent_path();
    }
    if(!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder)){
        std::cerr << "Result folder does not exist: " << folder << std::endl;
        return nullptr;
    }

    std::string file = folder.filename().string();

    const std::string prefix = "output_";
    if(file.rfind(prefix, 0) == 0){
        file.erase(0, prefix.size());
    }

    file += ".json";

    std::filesystem::path jsonPath = folder / file;

    if(jsonPath.empty()){
        std::cerr << "Failed to find model json in result folder: " << folder << std::endl;
        return nullptr;
    }

    model* Model=readmodel(jsonPath.string());
    if(Model==nullptr){
        return nullptr;
    }
    std::string modelname=Model->getmodelname();
    std::filesystem::path parentFolder=folder.parent_path();
    Model->setfolderpath(parentFolder.empty() ? "." : parentFolder.string());

    int loadedHistorySize=0;
    auto bodyResult=readResultTable(folder/(modelname+"_body_result.txt"));
    for(body* Body : Model->getparm()->getallbody()){
        auto it=bodyResult.find(Body->getname());
        if(it!=bodyResult.end()){
            Body->getbodybasic()->set_q_history(it->second);
            loadedHistorySize=std::max(loadedHistorySize, static_cast<int>(it->second.size()));
        }
    }

    auto gammaResult=readResultTable(folder/(modelname+"_muscle_gamma_result.txt"));
    auto etaResult=readResultTable(folder/(modelname+"_muscle_eta_result.txt"));
    for(muscle* Muscle : Model->getparm()->getallmuscle()){
        auto gammaIt=gammaResult.find(Muscle->getname());
        if(gammaIt!=gammaResult.end()){
            Muscle->set_gamma_history(gammaIt->second);
            loadedHistorySize=std::max(loadedHistorySize, static_cast<int>(gammaIt->second.size()));
        }
        auto etaIt=etaResult.find(Muscle->getname());
        if(etaIt!=etaResult.end()){
            Muscle->set_eta_history(etaIt->second);
        }
    }

    if(loadedHistorySize>0){
        //Model->getparm()->set_run_total_step(std::max(0, loadedHistorySize-2));
        //rebuildJointAbsoluteHistory(Model, loadedHistorySize);
        Model->do_postprocessing(0.0);
    }
    return Model;
}

model* IO::readmodel(const std::string&  jsonfilename){

    std::cout<<jsonfilename<<std::endl;

    std::ifstream file(jsonfilename);
    if (!file.is_open()) {
        std::cerr << "Failed to open model.json" << std::endl;
    }

    Json::CharReaderBuilder readerBuilder;
    Json::Value root;

    JSONCPP_STRING errs;
    if (!Json::parseFromStream(readerBuilder, file, &root, &errs)) {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
    }

    model* Model= new model(root["name"].asString());
    //body
    const Json::Value& bodyArray = root["body"];
    for (const Json::Value& bodyObject : bodyArray) {

        std::string bodyname = bodyObject["body_name"].asString();
        std::string parentname = bodyObject["body_parent_name"].asString();
        std::vector<double> n_axis;
        const Json::Value& n_axisArray = bodyObject["rotation_axis_relative_parent_coordinate"];
        for (const Json::Value& value : n_axisArray) {
            n_axis.push_back(value.asDouble());
        }
        double rotationangle = bodyObject["rotation_angle"].asDouble();
        std::vector<double> rho_body;
        const Json::Value& rho_bodyArray = bodyObject["position_relative_parent_coordinate"];
        for (const Json::Value& value : rho_bodyArray) {
            rho_body.push_back(value.asDouble());
        }
        const Json::Value& Shape = bodyObject["shape"];
        double a = Shape["a"].asDouble();
        double b = Shape["b"].asDouble();
        double c = Shape["c"].asDouble();
        double length = Shape["length"].asDouble();
        double radius = Shape["radius"].asDouble();
        std::string shapename = Shape["shape_name"].asString();
        int global=0;
        if (bodyObject.isMember("global")) {
            global = bodyObject["global"].asInt();
        }
        Model->getparm()->addbody(bodyname,parentname,n_axis,rotationangle,rho_body,a,b,c,length,radius,shapename,global);
    }

    //muscle
    const Json::Value& muscleArray = root["muscle"];
    for (const Json::Value& muscleObject : muscleArray) {
        std::vector<double> rho_o;
        const Json::Value& rho_oArray = muscleObject["rho_origin"];
        for (const Json::Value& value : rho_oArray) {
            rho_o.push_back(value.asDouble());
        }
        std::string rho_obodyname = muscleObject["origin_relative_body"].asString();

        std::vector<double> rho_i;
        const Json::Value& rho_iArray = muscleObject["rho_insertion"];
        for (const Json::Value& value : rho_iArray) {
            rho_i.push_back(value.asDouble());
        }
        std::string rho_ibodyname = muscleObject["insertion_relative_body"].asString();
        std::string musclename = muscleObject["muscle_name"].asString();
        int nodenumber = muscleObject["node_number"].asInt();

        std::vector<std::string> consider_body_list_value={};
        if (muscleObject.isMember("consider_body_list")) {
            const Json::Value& consider_body_listArray = muscleObject["consider_body_list"];
            for (const Json::Value& value : consider_body_listArray) {
                consider_body_list_value.push_back(value.asString());
            }
        }

        std::vector<double> gammavalue={};
        std::vector<double> etavalue={};
        if (muscleObject.isMember("gamma")) {
            const Json::Value& gammaArray = muscleObject["gamma"];
            for (const Json::Value& value : gammaArray) { 
                gammavalue.push_back(value.asDouble());
            }
        }

        if (muscleObject.isMember("eta")) {
            const Json::Value& etaArray = muscleObject["eta"];
            for (const Json::Value& value : etaArray) {
                etavalue.push_back(value.asDouble());
            }
        }

        if (gammavalue.empty() && etavalue.empty()) {
            Model->getparm()->addmuscle(rho_o, rho_obodyname, rho_i, rho_ibodyname, musclename, nodenumber,0, {}, {}, consider_body_list_value);
            Model->getparm()->set_single_read_muscle_value(musclename, 0);
        }
        else{
            Model->getparm()->addmuscle(rho_o, rho_obodyname, rho_i, rho_ibodyname, musclename, nodenumber,0, gammavalue, etavalue, consider_body_list_value);
            int read_muscle_value = 3; // has value for both gamma and eta
            if(gammavalue.empty()){
                read_muscle_value = 1; // only has value for eta
            }
            if(etavalue.empty()){
                read_muscle_value = 2; // only has value for gamma
            }
            Model->getparm()->set_single_read_muscle_value(musclename, read_muscle_value);
        }
        std::vector<double> hillValue = {0.0, 0.0, 0.0};
        if (muscleObject.isMember("hill")) {
            const Json::Value& hillObject = muscleObject["hill"];
            if (hillObject.isMember("Fmax")) {
                hillValue[0] = hillObject["Fmax"].asDouble();
            }
            if (hillObject.isMember("Lopt")) {
                hillValue[1] = hillObject["Lopt"].asDouble();
            }
            if (hillObject.isMember("L0")) {
                hillValue[2] = hillObject["L0"].asDouble();
            }
        }
        Model->getparm()->set_hillpar(musclename, hillValue);
        std::vector<std::string> rho_via_point_bodyname={};
        std::vector<std::vector<double>> rho_via_point_value={};
        std::vector<std::vector<double>> rho_via_point_eta={};
        std::vector<double> via_point_alpha={};
        std::vector<double> via_point_cutoff={};
        if (muscleObject.isMember("viapoint_node")) {
            const Json::Value& viapointArray = muscleObject["viapoint_node"];
            for (const Json::Value& viapoint_value : viapointArray) {
                std::string rho_bodyname = viapoint_value["relative_body"].asString();
                rho_via_point_bodyname.push_back(rho_bodyname);
                std::vector<double> rho_via;
                const Json::Value& rho_viaArray = viapoint_value["rho_via"];
                for (const Json::Value& rho_viaArray_value : rho_viaArray) {
                    rho_via.push_back(rho_viaArray_value.asDouble());
                }
                rho_via_point_value.push_back(rho_via);
                std::vector<double> eta_via;
                if(viapoint_value.isMember("eta_via")){
                    const Json::Value& eta_viaArray = viapoint_value["eta_via"];
                    for (const Json::Value& eta_viaArray_value : eta_viaArray) {
                        eta_via.push_back(eta_viaArray_value.asDouble());
                    }
                } else{
                    eta_via.push_back(0.0); 
                }

                rho_via_point_eta.push_back(eta_via);
                if(viapoint_value.isMember("via_alpha")){
                    via_point_alpha.push_back(viapoint_value["via_alpha"].asDouble());
                } else{
                    via_point_alpha.push_back(100.0);
                }

                if(viapoint_value.isMember("via_cutoff")){
                    via_point_cutoff.push_back(viapoint_value["via_cutoff"].asDouble());
                } else{
                    via_point_cutoff.push_back(0.05);
                }
            }
            Model->getparm()->set_muscle_viapoint_node(musclename, rho_via_point_bodyname, rho_via_point_value, rho_via_point_eta, via_point_alpha, via_point_cutoff);
        }
    }
    //joint
    const Json::Value& jointArray = root["joint"];
    for (const Json::Value& jointObject : jointArray) {
        std::string joint_name_value = jointObject["joint_name"].asString();
        std::string rotate_body_name_joint = jointObject["rotate_body_name"].asString();
        std::string joint_type_name_value = jointObject["joint_type_name"].asString();
        std::vector<double> relative_posvalue;
        const Json::Value& relative_posvalueArray = jointObject["position_relative_rotate_body"];
        for (const Json::Value& value : relative_posvalueArray) {
            relative_posvalue.push_back(value.asDouble());
        }
        std::vector<double> rotation_axis_jointvalue;
        const Json::Value& rotation_axis_jointArray = jointObject["rotation_axis_relative_rotate_body"];
        for (const Json::Value& value : rotation_axis_jointArray) {
            rotation_axis_jointvalue.push_back(value.asDouble());
        }

        std::vector<std::vector<double>> movement_value={};
        if (jointObject.isMember("movement")) {
            const Json::Value& movement_valueArray = jointObject["movement"];
            for (const Json::Value& subArray : movement_valueArray) {
                if (!subArray.isArray()) continue; 
                std::vector<double> temp;
                for (const Json::Value& val : subArray) {
                    temp.push_back(val.asDouble());
                }
                movement_value.push_back(temp);
            }
        }

        int move_all_body_value = 0;
        if (jointObject.isMember("move_all_bodys")) {
            move_all_body_value = jointObject["move_all_bodys"].asInt();
        }

        std::vector<std::vector<std::vector<double>>> move_setting_value;
        const Json::Value& move_setting_valueArray = jointObject["move_setting"];
        for (const Json::Value& subArray1 : move_setting_valueArray) {
            if (!subArray1.isArray()) continue;
            std::vector<std::vector<double>> temp2D;
            for (const Json::Value& subArray2 : subArray1) {
                if (!subArray2.isArray()) continue;
                std::vector<double> temp;
                for (const Json::Value& val : subArray2) {
                    temp.push_back(val.asDouble());
                }
                temp2D.push_back(temp);
            }
            move_setting_value.push_back(temp2D);
        }

        // Call addjoint with the correct parameters
        Model->getparm()->addjoint(joint_name_value, rotate_body_name_joint, joint_type_name_value,relative_posvalue, rotation_axis_jointvalue, move_setting_value, movement_value, move_all_body_value);
        
        //Model->getparm()->addjoint(joint_name_value, rotate_body_name_joint, joint_type_name_value, relative_posvalue, rotation_axis_jointvalue, initial_rotation_anglevalue, rotation_anglevalue, initial_translationvalue, translationvalue);
    }

    //setipopt
    const Json::Value& ipopt = root["ipoptsetting"];
    double tolvalue = ipopt["tol"].asDouble();
    int max_itervalue = ipopt["max_iter"].asInt();
    std::string linear_solvervalue = ipopt["linear_solver"].asString();
    int print_levelvalue= ipopt["print_level"].asInt();
    std::string hessian_approximationvalue = ipopt["hessian_approximation"].asString();
    Model->getSolveeq()->setipoptoption(tolvalue,max_itervalue,linear_solvervalue,print_levelvalue,hessian_approximationvalue);

    const Json::Value& saveintervalvalue = root["save_interval"];
    int saveinterval=saveintervalvalue.asInt();
    Model->set_save_interval(saveinterval);

    const Json::Value& obj = root["objective"];
    int solvercase=obj["solvercase"].asInt();
    Model->getSolveeq()->getObjective()->setcasenum(solvercase);
    double length_constant=obj["length_constant"].asDouble();
    Model->getSolveeq()->getObjective()->setlengthconstant(length_constant);


    if (root.isMember("use_p_variable")) {
        int use_p_variable = root["use_p_variable"].asInt();
        Model->getSolveeq()->set_use_p_variable(use_p_variable);
    } else{ //for milimeter cases!!!
        Model->getSolveeq()->set_use_p_variable(0);
    }

    //initial guess setting
    const Json::Value& initial = root["initial_guess"];
    int mode_initial=initial["mode_number"].asInt();
    Model->getSolveeq()->getInitialguess()->setmode_nr(mode_initial);
    int collision_check_initial=initial["collision_check"].asInt();
    Model->getSolveeq()->getInitialguess()->setcollision_check(collision_check_initial);
    std::string select_body_name = initial["select_body"].asString();
    Model->getSolveeq()->getInitialguess()->setselect_bodyname(select_body_name);

    //constraint setting
    const Json::Value& constraint = root["constraint"];
    if (constraint.isMember("use_phi_eta_plus_length")) {
        int use_phi_eta_plus = constraint["use_phi_eta_plus_length"].asInt();
        Model->getSolveeq()->getConstraint()->set_phi_eta_plus(use_phi_eta_plus);
    } else{ //for milimeter cases!!!
        Model->getSolveeq()->getConstraint()->set_phi_eta_plus(0);
    }
    if (constraint.isMember("use_phi_eta_inequality_constraint")) {
        int use_phi_eta_inequality = constraint["use_phi_eta_inequality_constraint"].asInt();
        Model->getSolveeq()->getConstraint()->set_phi_eta_inequality(use_phi_eta_inequality);
    } else{ //increase stability
        Model->getSolveeq()->getConstraint()->set_phi_eta_inequality(0);
    }

    std::string local_select_bodyname=constraint["local_select_bodyname"].asString();
    Model->getSolveeq()->getConstraint()->set_local_select_bodyname(local_select_bodyname);
    int local_mode_number=constraint["local_mode_number"].asInt();
    Model->getSolveeq()->getConstraint()->set_local_mode_number(local_mode_number);
    if(local_mode_number>0){Model->getSolveeq()->getInitialguess()->setmode_nr(-1);}

    if (constraint.isMember("calculate_all_muscle_together")) {
        int all_muscle_together_value = constraint["calculate_all_muscle_together"].asInt();
        Model->getSolveeq()->set_all_muscle_together(all_muscle_together_value);
    } else{ //for milimeter cases!!!
        Model->getSolveeq()->set_all_muscle_together(0);
    }

    //postprocessing
    const Json::Value& postprocessingvalue = root["postprocessing"];
    double tol = postprocessingvalue["tol"].asDouble();
    Model->getPostprocessing()->settol(tol);
    
    //folder path
    if (root.isMember("output_path")) {
        std::string output_path_name = root["output_path"].asString();
        Model->setfolderpath(output_path_name);
    } else {
        std::filesystem::path file_path(jsonfilename);
        std::filesystem::path output_folder_path = file_path.parent_path();
        Model->setfolderpath(output_folder_path.string());
    }
    std::cout<<Model->getfolderpath()<<std::endl;
    return Model;
}
