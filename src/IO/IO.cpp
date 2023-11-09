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
    std::vector<double> rotation=Model->getSolveeq()->getrotationmodel();
    
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
    std::vector<double> rotation=Model->getSolveeq()->getrotationmodel();
    

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
    std::vector<double> rotation=Model->getSolveeq()->getrotationmodel();

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
    std::vector<double> rotation=Model->getSolveeq()->getrotationmodel();

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
    std::vector<double> rotation=Model->getSolveeq()->getrotationmodel();

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
    std::vector<double> rotation=Model->getSolveeq()->getrotationmodel();

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

void IO::writejson(model* Model){

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


    Json::Value ipopt;
    ipopt["tol"]=Model->getSolveeq()->getipopt()->gettol();
    ipopt["max_iter"]=Model->getSolveeq()->getipopt()->getmax_iter();
    ipopt["linear_solver"]=Model->getSolveeq()->getipopt()->getlinear_solver();
    ipopt["print_level"]=Model->getSolveeq()->getipopt()->getprint_level();
    ipopt["hessian_approximation"]=Model->getSolveeq()->getipopt()->gethessian_approximation();
    root["ipoptsetting"] = ipopt;

    Json::Value solvejoint;
    solvejoint["rotate_body"]=Model->getSolveeq()->getrotatebody();
    Json::Value axis(Json::arrayValue);
    for (const auto& value : Model->getSolveeq()->getnaxis()) {
        axis.append(value);
    }
    solvejoint["rotate_axis"]=axis;
    solvejoint["rotationangle"]=Model->getSolveeq()->getrotationangle();
    solvejoint["stepnum"]=Model->getSolveeq()->getstepnum();
    root["joint"] = solvejoint;

    root["solver"] = Model->getSolveeq()->getsolversetting();

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
        Model->getparm()->addbody(bodyname,parentname,n_axis,rotationangle,rho_body,a,b,c,length,radius,shapename);
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
        Model->getparm()->addmuscle(rho_o, rho_obodyname, rho_i, rho_ibodyname, musclename, nodenumber);
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
    const Json::Value& solveproblem = root["joint"];
    std::string rotatebody=solveproblem["rotate_body"].asString();
    std::vector<double> naxis_solve;
    const Json::Value& naxis_solveArray = solveproblem["rotate_axis"];
    for (const Json::Value& value : naxis_solveArray) {
        naxis_solve.push_back(value.asDouble());
    }
    double rotationangle=solveproblem["rotationangle"].asDouble();
    int stepnum=solveproblem["stepnum"].asInt();
    Model->getSolveeq()->setproblemtosolve(rotatebody,naxis_solve,rotationangle,stepnum);
    const Json::Value& solver = root["solver"];
    int solvervalue=solver.asInt();
    Model->getSolveeq()->setsolversetting(solvervalue);

    //postprocessing
    const Json::Value& postprocessingvalue = root["postprocessing"];
    double tol = postprocessingvalue["tol"].asDouble();
    Model->getPostprocessing()->settol(tol);

    return Model;
}