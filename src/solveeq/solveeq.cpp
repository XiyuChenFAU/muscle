/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "solveeq.h"
// #include <omp.h> // new for parallelization

solveeq::solveeq(){
    ipopt=new IPOPT();
    Constraint=new constraint();
    Objective=new objective();
    Initialguess=new initialguess(0,0);
}
    
solveeq::~solveeq(){
    delete ipopt;
    delete Constraint;
    delete Objective;
    delete Initialguess;
}

IPOPT* solveeq::getipopt(){
    return ipopt;
}

void solveeq::setipoptoption(double tolvalue,int max_itervalue,const std::string& linear_solvervalue,int print_levelvalue,const std::string& hessian_approximationvalue){
    ipopt->settol(tolvalue);
    ipopt->setmax_iter(max_itervalue);
    ipopt->setlinear_solver(linear_solvervalue);
    ipopt->setprint_level(print_levelvalue);
    ipopt->sethessian_approximation(hessian_approximationvalue);
}

constraint* solveeq::getConstraint(){
    return Constraint;
}

objective* solveeq::getObjective(){
    return Objective;
}

initialguess* solveeq::getInitialguess(){
    return Initialguess;
}

void solveeq::set_use_p_variable(int value){
    use_p_variable=value;
}

int solveeq::get_use_p_variable(){
    return use_p_variable;
}

void solveeq::solvesignorinirotate(Parm* parm){
    
    std::vector<std::vector<double>> jointnaxisall;
    for(int i=0;i<parm->getn_joints();i++){
        jointnaxisall.push_back(parm->getjointindex(i)->getabsolute_pos().back());
    }
    
    std::vector<muscle*> allmuscle=parm->getallmuscle();
    std::vector<body*> allbody=parm->getallbody();
    int variablenumall_sig=parm->getvariable();
    int variablenumall_via=parm->get_muscle_viapoint_node_num();
    int variablenumall=variablenumall_sig+variablenumall_via;
    Constraint->set_dict_constraint_all({});
    int bodynum=parm->getn_bodies();
    if(all_muscle_together){
        //variable
        MX x = MX::sym("x", variablenumall);
        //p_variable
        int p_var_musclenum=0;
        for(int k=0;k<parm->getn_muscles();k++){
            p_var_musclenum=p_var_musclenum+3*allmuscle[k]->getnodenum();
        }
        int p_var_num=bodynum*12+p_var_musclenum*2;
        MX p_var = MX::sym("p_var", p_var_num);
        //minimum
        MX f = 0;
        int start_index_x=0;
        int start_index_via_x=0;
        int current_muscle_node_index=0;
        for(int i=0;i<parm->getn_muscles();i++){
            int variablenum=allmuscle[i]->getvariablenum(parm->getn_bodies());
            MX x_single= x(Slice(start_index_x, start_index_x + variablenum));
            int variablenum_via=allmuscle[i]->via_point_num();
            MX x_single_via = x(Slice(start_index_via_x + variablenumall_sig, start_index_via_x + variablenumall_sig + variablenum_via));
            std::vector<MX> p_gamma_previous;
            std::vector<MX> p_mass_matrix;
            if(use_p_variable){
                for(int j=0;j<3*allmuscle[i]->getnodenum();j++){
                    p_gamma_previous.push_back(p_var(bodynum*12 + current_muscle_node_index + j));
                    p_mass_matrix.push_back(p_var(bodynum*12 + p_var_musclenum + current_muscle_node_index + j));
                }
                current_muscle_node_index=current_muscle_node_index+allmuscle[i]->getnodenum()*3;
            }
            f = f + Objective->getobjective(parm, x_single,jointnaxisall,i, Constraint->get_local_mode_number(), use_p_variable, p_gamma_previous, p_mass_matrix);
            //constraints.
            std::vector<MX> singlemuscleconstraint=Constraint->constraints(parm,x_single, x_single_via, i, use_p_variable, p_var);
            start_index_x=start_index_x+variablenum;
            start_index_via_x=start_index_via_x+variablenum_via;
        }
        std::vector<MX> allconstraint = Constraint->put_constraints_together();
        MX g = vertcat(allconstraint);
        //set ipopt nlp
        MXDict nlp;
        if(use_p_variable){
            nlp = {{"x", x}, {"p", p_var}, {"f", f}, {"g", g}};
        }  else {
            nlp = {{"x", x}, {"f", f}, {"g", g}};
        }
        Dict opts_dict=ipopt->getipoptparm();
        Function solver = nlpsol("solver", "ipopt", nlp, opts_dict);
        std::map<std::string, DM> arg, res;
        // Set constraint limit
        std::vector<std::vector<double>> lower_upper_limit = Constraint->set_lower_upper_limit();
        arg["lbg"] = lower_upper_limit[0];
        arg["ubg"] = lower_upper_limit[1];
        // Initial guess
        std::vector<double> x0;
        for(int i=0;i<parm->getn_muscles();i++){
            std::vector<double> initial=Initialguess->get_initialguessvalueindex(i);
            x0.insert(x0.end(), initial.begin(), initial.end());
        }
        std::vector<double> initial_via_point_eta=parm->get_all_via_point_eta(-1);
        x0.insert(x0.end(), initial_via_point_eta.begin(), initial_via_point_eta.end());
        if(g_enable_print){
            std::cout<<"x0 size: "<<x0.size() <<std::endl;
        }

        arg["x0"] = x0;
        //p_variable initial
        if(use_p_variable){
            std::vector<double> p_var_init;
            for(int j=0;j<parm->getn_bodies();j++){
                std::vector<double>  q=allbody[j+1]->getbodybasic()->getq().back();
                p_var_init.insert(p_var_init.end(), q.begin(), q.end());
            }
            for(int j=0;j<parm->getn_muscles();j++){
                std::vector<std::vector<double>> gammaall = allmuscle[j]->getgammaall();
                p_var_init.insert(p_var_init.end(), gammaall.back().begin(), gammaall.back().end());
            }
            for(int j=0;j<parm->getn_muscles();j++){
                std::vector<double> mass_matrix = Objective->getmassmatrix(allmuscle[j], jointnaxisall);
                p_var_init.insert(p_var_init.end(), mass_matrix.begin(), mass_matrix.end());
            }
            arg["p"] = p_var_init;
        }

        // Solve the NLP
        res = solver(arg);
        vector<double> solution;
        for (int i = 0; i < x0.size(); i++) {
            solution.push_back(static_cast<double>(res.at("x")(i)));
        }
        //add solution to each muscle
        int start_index=0;
        int start_index_via=0;
        for(int i=0;i<parm->getn_muscles();i++){
            int variablenum=allmuscle[i]->getvariablenum(parm->getn_bodies());
            std::vector<double> singlemusclesolution(solution.begin()+start_index,solution.begin()+start_index+variablenum);
            allmuscle[i]->addmuscleparm(singlemusclesolution, Constraint->get_local_mode_number());
            start_index=start_index+variablenum;
            int variablenum_via=allmuscle[i]->via_point_num();
            std::vector<double> singlemuscle_via_solution(solution.begin()+variablenumall_sig+start_index_via,solution.begin()+variablenumall_sig+start_index_via+variablenum_via);
            allmuscle[i]->update_via_point_eta(singlemuscle_via_solution);
            start_index_via=start_index_via+variablenum_via;
        }
    }
    else{
        for(int i=0;i<parm->getn_muscles();i++){
            if(g_enable_print){
                std::cout<<"calculate muscle: "<<  allmuscle[i]->getname() <<std::endl;
            }
            //variable
            int variablenum_sig=allmuscle[i]->getvariablenum(parm->getn_bodies());
            int variablenum_via=allmuscle[i]->via_point_num();
            int variablenum=variablenum_sig+variablenum_via;
            MX x = MX::sym("x", variablenum);
            MX x_single_sig= x(Slice(0, variablenum_sig));
            MX x_single_via = x(Slice(variablenum_sig, variablenum_sig + variablenum_via));
            //p_variable
            int p_var_num=bodynum*12+3*allmuscle[i]->getnodenum()*2;
            MX p_var = MX::sym("p_var", p_var_num);
            //minimum
            std::vector<MX> p_gamma_previous;
            std::vector<MX> p_mass_matrix;
            if(use_p_variable){
                for(int j=0;j<3*allmuscle[i]->getnodenum();j++){
                    p_gamma_previous.push_back(p_var(bodynum*12 + j));
                    p_mass_matrix.push_back(p_var(bodynum*12 + 3*allmuscle[i]->getnodenum() + j));
                }
            }
            MX f = Objective->getobjective(parm, x,jointnaxisall,i, Constraint->get_local_mode_number(), use_p_variable, p_gamma_previous, p_mass_matrix);
            //constraints
            std::vector<MX> allconstraint=Constraint->constraints(parm,x_single_sig, x_single_via, i, use_p_variable, p_var);
            MX g = vertcat(allconstraint);
            //set ipopt nlp
            MXDict nlp;
            if(use_p_variable){
                nlp = {{"x", x}, {"p", p_var}, {"f", f}, {"g", g}};
            }  else {
                nlp = {{"x", x}, {"f", f}, {"g", g}};
            }
            Dict opts_dict=ipopt->getipoptparm();
            Function solver = nlpsol("solver", "ipopt", nlp, opts_dict);
            std::map<std::string, DM> arg, res;
            // Set constraint limit
            std::vector<std::vector<double>> lower_upper_limit = Constraint->set_lower_upper_limit();
            arg["lbg"] = lower_upper_limit[0];
            arg["ubg"] = lower_upper_limit[1];
            // Initial guess
            std::vector<double> x0;
            std::vector<double> initial=Initialguess->get_initialguessvalueindex(i);
            x0.insert(x0.end(), initial.begin(), initial.end());
            std::vector<double> initial_muscle_via_point_eta=allmuscle[i]->getvia_point_eta(-1);
            x0.insert(x0.end(), initial_muscle_via_point_eta.begin(), initial_muscle_via_point_eta.end());
            if(g_enable_print){
                std::cout<<"x0 size: "<<x0.size() <<std::endl;
            }

            arg["x0"] = x0;
            //p_variable initial
            if(use_p_variable){
                std::vector<double> p_var_init;
                for(int j=0;j<parm->getn_bodies();j++){
                    std::vector<double>  q=allbody[j+1]->getbodybasic()->getq().back();
                    p_var_init.insert(p_var_init.end(), q.begin(), q.end());
                }
                std::vector<std::vector<double>> gammaall = allmuscle[i]->getgammaall();
                p_var_init.insert(p_var_init.end(), gammaall.back().begin(), gammaall.back().end());
            
                std::vector<double> mass_matrix = Objective->getmassmatrix(allmuscle[i], jointnaxisall);
                p_var_init.insert(p_var_init.end(), mass_matrix.begin(), mass_matrix.end());
                
                arg["p"] = p_var_init;
            }
            // Solve the NLP
            res = solver(arg);
            vector<double> solution;
            vector<double> solution_via;
            for (int i = 0; i < x0.size(); i++) {
                if(i<variablenum_sig){
                    solution.push_back(static_cast<double>(res.at("x")(i)));
                } else {
                    solution_via.push_back(static_cast<double>(res.at("x")(i)));
                }
            }
            allmuscle[i]->addmuscleparm(solution, Constraint->get_local_mode_number());
            allmuscle[i]->update_via_point_eta(solution_via);
        }
    }
}

void solveeq::solvesignorinistep(Parm* parm, int stepnum){
    if(stepnum==0){parm->set_node_partition(Constraint->get_local_mode_number(), Constraint->get_local_select_bodyname(), Initialguess->getmode_nr(), Initialguess->getselect_bodyname(), 1);}
    if(g_enable_print){parm->set_body_R_initial();}
    // I want to write a sentence that if Constraint->get_local_mode_number()!=4, then set variable node_partition_before = 1 otherwise node_partition_before=0
    int node_partition_after = (Constraint->get_local_mode_number() == 4 || Initialguess->getmode_nr() == 4) ? 1 : 0;
    if(!node_partition_after){
        parm->set_node_partition(Constraint->get_local_mode_number(), Constraint->get_local_select_bodyname(), Initialguess->getmode_nr(), Initialguess->getselect_bodyname(), 0);
    }
    parm->rotatebodyupdate(stepnum);
    if(g_enable_print){parm->check_body_R();}
    if(node_partition_after){parm->set_node_partition(Constraint->get_local_mode_number(), Constraint->get_local_select_bodyname(), Initialguess->getmode_nr(), Initialguess->getselect_bodyname(), 0);}
    parm->update_all_via_point_gamma();
    if(stepnum==0){
        parm->setallmuscleinitialeta_gamma();
        Initialguess->set_initialguessvalue(parm, 1);
    }else{
        Initialguess->set_initialguessvalue(parm, 0);
    }
    solvesignorinirotate(parm);
}

int solveeq::get_all_muscle_together(){
    return all_muscle_together;
}

void solveeq::set_all_muscle_together(int value){
    all_muscle_together=value;
}

void solveeq::set_local_parameter(int selectedValue_localmode, int selectedValue_mode, const std::string&  selectedValue_body, int selectedValue_cons_mode, const std::string&  selectedValue_cons_body, int check_collision_Value){
    if(selectedValue_localmode){ //use local frame
        Initialguess->setcollision_check(0);
        Initialguess->setmode_nr(-1);
        Initialguess->setselect_bodyname(selectedValue_body);
        Constraint->set_local_mode_number(selectedValue_cons_mode);
        Constraint->set_local_select_bodyname(selectedValue_cons_body);
    } else {
        Initialguess->setcollision_check(check_collision_Value);
        Initialguess->setmode_nr(selectedValue_mode);
        Initialguess->setselect_bodyname(selectedValue_body);
        Constraint->set_local_mode_number(0);
        Constraint->set_local_select_bodyname(selectedValue_cons_body);
    }
}
