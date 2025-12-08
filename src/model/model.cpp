/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "model.h"

model::model(const std::string& modelname):
    modelname(modelname)
{
    parm=new Parm();
    Solveeq=new solveeq();
    Postprocessing=new postprocessing();
    //// space fix
    std::vector<double> qall;
    std::vector<double> origin = {0.0, 0.0, 0.0};
    std::vector<double> e1 = {1.0, 0.0, 0.0};
    std::vector<double> e2 = {0.0, 1.0, 0.0};
    std::vector<double> e3 = {0.0, 0.0, 1.0};
    qall=pushback(qall,origin);
    qall=pushback(qall,e1);
    qall=pushback(qall,e2);
    qall=pushback(qall,e3);
    parm->addbody("fix_space",qall);
}

model::~model(){
    delete parm;
    delete Solveeq;
    delete Postprocessing;
}

void model::setmodelname(const std::string& modelnamevalue){
    modelname=modelnamevalue;
}

Parm* model::getparm(){
    return parm;
}

solveeq* model::getSolveeq(){
    return Solveeq;
}

std::string model::getmodelname(){
    return modelname;
}

postprocessing* model::getPostprocessing(){
    return Postprocessing;
}

void model::solve_signorini(){
    int loopnum = parm->get_run_total_step()+1;
    if(parm->get_run_total_step()==0){
        loopnum=0;
    }
    for(int i=0;i<loopnum;i++){
        Solveeq->solvesignorinistep(parm, i);
        if(save_interval!=0){
            if(i%save_interval==0){writejson(1,i+1);}
        }
    }
}

void model::do_postprocessing(double timevalue){
    elapsed_time=timevalue;
    Postprocessing->do_postprocessingall(parm);
}

std::vector<double> model::pushback(std::vector<double>& q, const std::vector<double>& value){
    for(int i=0; i<value.size(); i++){
        q.push_back(value[i]);
    }
    return q;
}

int model::get_save_interval(){
    return save_interval;
}

void model::set_save_interval(int value){
    save_interval=value;
}

double model::get_elapsed_time(){
    return elapsed_time;
}

void model::set_elapsed_time(double timevalue){
    elapsed_time=timevalue;
}

void model::setfolderpath(const std::string& folderpathvalue){
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

std::string model::getfolderpath(){
    return folderpath;
}

void model::writejson(int write_gamma, int currentstepnum){

    //create folder
    std::string folderoutput="";

    folderoutput=folderpath+"output_"+modelname;
    
    if (!std::filesystem::exists(folderoutput)) {
        std::filesystem::create_directory(folderoutput);
        std::cout << "save result to folder " << folderoutput << std::endl;
    }

    Json::Value root;
    root["name"] = modelname;

    Json::Value bodyArray(Json::arrayValue);
    std::vector<body*> allbody=parm->getallbody();
    for (int i=0;i<parm->getn_bodies();i++) {
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
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    for(int i=0;i<parm->getn_muscles();i++){
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
    for (int i=0;i<parm->getn_joints();i++) {

        Json::Value jointObject;
        jointObject["joint_name"] = parm->getjointindex(i)->getname();
        jointObject["rotate_body_name"] = parm->getjointindex(i)->getbodyname();
        jointObject["joint_type_name"] = parm->getjointindex(i)->getjoint_type();
        Json::Value relative_posvalue(Json::arrayValue);
        for (const auto& value : parm->getjointindex(i)->getrelative_pos()) {
            relative_posvalue.append(value);
        }
        jointObject["position_relative_rotate_body"] = relative_posvalue;
        Json::Value axisvectorvalue(Json::arrayValue);
        for (const auto& value : parm->getjointindex(i)->getaxisvector()) {
            axisvectorvalue.append(value);
        }
        jointObject["rotation_axis_relative_rotate_body"] = axisvectorvalue;
        if(parm->getjointindex(i)->getmove_all_body()){
            jointObject["move_all_bodys"] = parm->getjointindex(i)->getmove_all_body();
        }

        Json::Value move_setting_value(Json::arrayValue);
        for (const auto& matrix : parm->getjointindex(i)->get_move_setting()) {
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

        if(parm->getjointindex(i)->getread_from_movement()){
            Json::Value movement_value(Json::arrayValue);
            for (const auto& row : parm->getjointindex(i)->get_movement()) {
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
    ipopt["tol"]=Solveeq->getipopt()->gettol();
    ipopt["max_iter"]=Solveeq->getipopt()->getmax_iter();
    ipopt["linear_solver"]=Solveeq->getipopt()->getlinear_solver();
    ipopt["print_level"]=Solveeq->getipopt()->getprint_level();
    ipopt["hessian_approximation"]=Solveeq->getipopt()->gethessian_approximation();
    root["ipoptsetting"] = ipopt;

    Json::Value objective;
    objective["solvercase"] = Solveeq->getObjective()->getcasenum();
    objective["length_constant"] = Solveeq->getObjective()->getlengthconstant();
    root["objective"]=objective;

    Json::Value initial;
    initial["mode_number"] = Solveeq->getInitialguess()->getmode_nr();
    initial["select_body"] = Solveeq->getInitialguess()->getselect_bodyname();
    initial["collision_check"] = Solveeq->getInitialguess()->getcollision_check();
    root["initial_guess"]=initial;

    Json::Value postprocessing;
    postprocessing["tol"]=Postprocessing->gettol();
    root["postprocessing"] = postprocessing;

    root["save_interval"] = save_interval;
    root["output_path"] = folderpath;

    if(Solveeq->getConstraint()->get_phi_eta_plus()){
        root["use_phi_eta_plus_length"] = 1;
    }

    if(Solveeq->get_all_muscle_together()){
        root["calculate_all_muscle_together"] = 1;
    }

    // change json object to string
    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, root);
    std::string filename = "";
    if(write_gamma){
        filename = folderpath+"output_"+modelname+"/"+modelname+"_step_"+std::to_string(currentstepnum-1)+".json";
    }
    else{
        filename = folderpath+"output_"+modelname+"/"+modelname+".json";
    }

    std::ofstream file(filename);
    file << jsonString;
    file.close();
}
