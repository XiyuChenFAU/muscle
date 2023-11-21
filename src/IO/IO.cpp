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
    std::string folderoutput=folderpath+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<body*> allbody=Model->getparm()->getallbody();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<double> rotation;
    if(Model->getparm()->getn_joints()>0){rotation=Model->getparm()->getjointindex(0)->getallrotationangle();}
    
    //writebody
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_body_result.txt";
    std::ofstream file1(filename);
    //write titel
    file1 << "rotation angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
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
    file2 << "rotation angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
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
    file3 << "rotation angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
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
}

void IO::writephiToFile(model* Model){

    //create folder
    std::string folderoutput=folderpath+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<std::vector<double>>> phiall = Model->getPostprocessing()->getphiall();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<body*> allbody=Model->getparm()->getallbody();
    std::vector<double> rotation;
    if(Model->getparm()->getn_joints()>0){rotation=Model->getparm()->getjointindex(0)->getallrotationangle();}
    

    //phiall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_phi_result.txt";
    std::ofstream file1(filename);
    //write titel
    file1 << "rotation angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
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
    std::string folderoutput=folderpath+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<std::vector<double>>> lengthall = Model->getPostprocessing()->getlengthall();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<double> rotation;
    if(Model->getparm()->getn_joints()>0){rotation=Model->getparm()->getjointindex(0)->getallrotationangle();}

    //lengthall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_length_result.txt";
    std::ofstream file2(filename);
    //write titel
    file2 << "rotation angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
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
    std::string folderoutput=folderpath+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<std::vector<double>>> forceallnode = Model->getPostprocessing()->getforceallnode();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<double> rotation;
    if(Model->getparm()->getn_joints()>0){rotation=Model->getparm()->getjointindex(0)->getallrotationangle();}

    //forceallnode
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_forcenode_result.txt";
    std::ofstream file3(filename);
    //write titel
    file3 << "rotation angle" << "\t"<<" "<<"\t"<<" "<<"\t";
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
    std::string folderoutput=folderpath+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<double>> totalforceall = Model->getPostprocessing()->gettotalforceall();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<double> rotation;
    if(Model->getparm()->getn_joints()>0){rotation=Model->getparm()->getjointindex(0)->getallrotationangle();}

    //totalforceall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_totalforce_result.txt";
    std::ofstream file4(filename);
    //write titel
    file4 << "rotation angle" << "\t"<<" "<<"\t"<<" "<<"\t";
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
    std::string folderoutput=folderpath+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<body*> allbody=Model->getparm()->getallbody();
    std::vector<double> rotation;
    if(Model->getparm()->getn_joints()>0){rotation=Model->getparm()->getjointindex(0)->getallrotationangle();}

    //body_state
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_bodystate_result.txt";
    std::ofstream file5(filename);
    //write titel
    file5 << "rotation angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
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
    std::string folderoutput=folderpath+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<std::vector<double>>> momentarmnodeall = Model->getPostprocessing()->getmomentarmnodeall();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<joint*> alljoint=Model->getparm()->getalljoint();
    std::vector<double> rotation;
    if(Model->getparm()->getn_joints()>0){rotation=Model->getparm()->getjointindex(0)->getallrotationangle();}
    

    //phiall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_moment_arm_node_result.txt";
    std::ofstream file6(filename);
    //write titel
    file6 << "rotation angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
    for(int i=0;i<rotation.size();i++){file6 << rotation[i] << "\t";}
    file6 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        for(int j =0;j<Model->getparm()->getn_joints();j++){
            write2DvalueToFile(momentarmnodeall[i*Model->getparm()->getn_joints()+j],file6,allmuscle[i]->getname()+"-"+alljoint[j]->getname(),"moment_arm_node");
        }
    }
    file6.close();
}

void IO::writemomentarmToFile(model* Model){

    //create folder
    std::string folderoutput=folderpath+"output_"+Model->getmodelname();
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    std::vector<std::vector<double>> momentarmall = Model->getPostprocessing()->getmomentarmall();
    std::vector<muscle*> allmuscle=Model->getparm()->getallmuscle();
    std::vector<joint*> alljoint=Model->getparm()->getalljoint();
    std::vector<double> rotation;
    if(Model->getparm()->getn_joints()>0){rotation=Model->getparm()->getjointindex(0)->getallrotationangle();}
    

    //phiall
    std::string filename = folderoutput+"/"+Model->getmodelname()+"_moment_arm_result.txt";
    std::ofstream file7(filename);
    //write titel
    file7 << "rotation angle" << "\t"<<" "<<"\t"<<" "<<"\t"<<"initial"<<"\t";
    for(int i=0;i<rotation.size();i++){file7 << rotation[i] << "\t";}
    file7 << "\n";
    //writevalue
    for(int i =0;i<Model->getparm()->getn_muscles();i++){
        for(int j =0;j<Model->getparm()->getn_joints();j++){
            file7 << allmuscle[i]->getname()+"-"+alljoint[j]->getname() << "\t"<<"moment_arm"<<"\t"<<1<<"\t";
            for(int k =0;k<momentarmall[i*Model->getparm()->getn_joints()+j].size();k++){
                file7 << momentarmall[i*Model->getparm()->getn_joints()+j][k]<<"\t";
            }
            file7 << "\n";
        }
    }
    file7.close();
}

void IO::writejson(model* Model){

    //create folder
    std::string folderoutput=folderpath+"output_"+Model->getmodelname();
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

        Json::Value initial_rotationanglevalue(Json::arrayValue);
        for (const auto& value : Model->getparm()->getjointindex(i)->getinitialrotationangle()) {
            initial_rotationanglevalue.append(value);
        }
        jointObject["initial_rotation_angle"] = initial_rotationanglevalue;

        Json::Value rotationanglevalue(Json::arrayValue);
        for (const auto& value : Model->getparm()->getjointindex(i)->getrotationangle()) {
            rotationanglevalue.append(value);
        }
        jointObject["rotation_angle"] = rotationanglevalue;
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


    root["stepnum"]=Model->getSolveeq()->getstepnum();
    root["solvercase"] = Model->getSolveeq()->getObjective()->getcasenum();

    Json::Value postprocessing;
    postprocessing["tol"]=Model->getPostprocessing()->gettol();
    root["postprocessing"] = postprocessing;

    // change json object to string
    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, root);
    std::string filename = folderpath+"output_"+Model->getmodelname()+"/"+Model->getmodelname()+".json";
    std::ofstream file(filename);
    file << jsonString;
    file.close();
}

void IO::setfolderpath(const std::string& folderpathvalue){
    folderpath=folderpathvalue;
    if (!folderpath.empty()) {
        for (char &c : folderpath) {
            if (c == '\\') {
                c = '/';
            }
        }

        if (folderpath.back() != '/') {
            folderpath += '/';
        }
    }
}

std::string IO::getfolderpath(){
    return folderpath;
}

model* IO::readmodel(const std::string&  jsonfilename){

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
        Model->getparm()->addmuscle(rho_o, rho_obodyname, rho_i, rho_ibodyname, musclename, nodenumber,0);
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
        std::vector<double> initial_rotation_anglevalue;
        const Json::Value& initial_rotation_angleArray = jointObject["initial_rotation_angle"];
        for (const Json::Value& value : initial_rotation_angleArray) {
            initial_rotation_anglevalue.push_back(value.asDouble());
        }
        std::vector<double> rotation_anglevalue;
        const Json::Value& rotation_angleArray = jointObject["rotation_angle"];
        for (const Json::Value& value : rotation_angleArray) {
            rotation_anglevalue.push_back(value.asDouble());
        }
        Model->getparm()->addjoint(joint_name_value, rotate_body_name_joint, joint_type_name_value, relative_posvalue, rotation_axis_jointvalue, initial_rotation_anglevalue, rotation_anglevalue);
    }

    //setipopt
    const Json::Value& ipopt = root["ipoptsetting"];
    double tolvalue = ipopt["tol"].asDouble();
    int max_itervalue = ipopt["max_iter"].asInt();
    std::string linear_solvervalue = ipopt["linear_solver"].asString();
    int print_levelvalue= ipopt["print_level"].asInt();
    std::string hessian_approximationvalue = ipopt["hessian_approximation"].asString();
    Model->getSolveeq()->setipoptoption(tolvalue,max_itervalue,linear_solvervalue,print_levelvalue,hessian_approximationvalue);

    //solve_problem
    const Json::Value& stepnumvalue = root["stepnum"];
    int stepnum=stepnumvalue.asInt();
    Model->getSolveeq()->setstepnum(stepnum);

    const Json::Value& solver = root["solvercase"];
    int solvercase=solver.asInt();
    Model->getSolveeq()->getObjective()->setcasenum(solvercase);

    //postprocessing
    const Json::Value& postprocessingvalue = root["postprocessing"];
    double tol = postprocessingvalue["tol"].asDouble();
    Model->getPostprocessing()->settol(tol);
    return Model;
}