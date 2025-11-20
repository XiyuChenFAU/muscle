/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "IO.h"

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
    std::vector<int> rotation=Model->getparm()->getallstep();
    
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
    writeforcenodeToFile(Model);
    writetotalforceToFile(Model);
    writebody_stateToFile(Model);
    writemomentarmnodeToFile(Model);
    writemomentarmToFile(Model);
    writerunningtimeToFile(Model);
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
    std::vector<int> rotation=Model->getparm()->getallstep();
    

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
    std::vector<int> rotation=Model->getparm()->getallstep();

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

void IO::writeforcenodeToFile(model* Model){

    //create folder
    std::string folderoutput=Model->getfolderpath()+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<std::vector<double>>> forceallnode = Model->getPostprocessing()->getforceallnode();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<int> rotation=Model->getparm()->getallstep();

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
    std::vector<int> rotation=Model->getparm()->getallstep();

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
            file4 << totalforceall[i][j] << "\t";
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
    std::vector<int> rotation=Model->getparm()->getallstep();

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
    std::vector<int> rotation=Model->getparm()->getallstep();

    //phiall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_momentarmnode_result.txt";
    std::ofstream file6(filename);
    //write titel
    file6 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t";
    for(int i=0;i<rotation.size();i++){file6 << rotation[i] << "\t";}
    file6 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        for(int j =0;j<Model->getparm()->getn_joints();j++){
            if(alljoint[j]->getwritemomentarm()){
                write2DvalueToFile(momentarmnodeall[i*(Model->getparm()->get_write_moment_joints())+j],file6,allmuscle[i]->getname()+"-"+alljoint[j]->getname(),"moment_arm_node");
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
    std::vector<int> rotation=Model->getparm()->getallstep();
    

    //phiall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_momentarm_result.txt";
    std::ofstream file7(filename);
    //write titel
    file7 << "rotation_angle" << "\t"<<" "<<"\t"<<" "<<"\t";
    for(int i=0;i<rotation.size();i++){file7 << rotation[i] << "\t";}
    file7 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        for(int j =0;j<Model->getparm()->getn_joints();j++){
            if(alljoint[j]->getwritemomentarm()){
                file7 << allmuscle[i]->getname()+"-"+alljoint[j]->getname() << "\t"<<"moment_arm"<<"\t"<<1<<"\t";
                for(int k =0;k<momentarmall[i*Model->getparm()->get_write_moment_joints()+j].size();k++){
                    file7 << momentarmall[i*Model->getparm()->get_write_moment_joints()+j][k]<<"\t";
                }
                file7 << "\n";
            }
        }
    }
    file7.close();
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
    file8 << "run_time" << Model->get_elapsed_time() <<"\n";
    file8.close();
}

void IO::writejson(model* Model, int write_gamma, int currentstepnum){

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

        Json::Value movement_value(Json::arrayValue);
        for (const auto& row : Model->getparm()->getjointindex(i)->get_movement()) {
            Json::Value rowJson(Json::arrayValue);
            for (const auto& value : row) {
                rowJson.append(value);
            }
            movement_value.append(rowJson);
        }
        jointObject["movement"] = movement_value;

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
        Model->getparm()->addbody(bodyname,parentname,n_axis,rotationangle,rho_body,a,b,c,length,radius,shapename,0);
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
            Model->getparm()->addmuscle(rho_o, rho_obodyname, rho_i, rho_ibodyname, musclename, nodenumber,0);
            Model->getparm()->set_single_read_muscle_value(musclename, 0);
        }
        else{
            Model->getparm()->addmuscle(rho_o, rho_obodyname, rho_i, rho_ibodyname, musclename, nodenumber,0, gammavalue, etavalue);
            int read_muscle_value = 3; // has value for both gamma and eta
            if(gammavalue.empty()){
                read_muscle_value = 1; // only has value for eta
            }
            if(etavalue.empty()){
                read_muscle_value = 2; // only has value for gamma
            }
            Model->getparm()->set_single_read_muscle_value(musclename, read_muscle_value);
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
        Model->getparm()->addjoint(joint_name_value, rotate_body_name_joint, joint_type_name_value,relative_posvalue, rotation_axis_jointvalue, move_setting_value, movement_value);

        
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

    //initial guess setting
    const Json::Value& initial = root["initial_guess"];
    int mode_initial=initial["mode_number"].asInt();
    Model->getSolveeq()->getInitialguess()->setmode_nr(mode_initial);
    int collision_check_initial=initial["collision_check"].asInt();
    Model->getSolveeq()->getInitialguess()->setcollision_check(collision_check_initial);
    std::string select_body_name = initial["select_body"].asString();
    Model->getSolveeq()->getInitialguess()->setselect_bodyname(select_body_name);

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