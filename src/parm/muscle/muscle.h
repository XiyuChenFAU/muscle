/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef MUSCLE_H
#define MUSCLE_H

#include "node.h"

class muscle {
public:
    muscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, int global, const std::vector<double>& gammavalue, const std::vector<double>& etavalue, const std::vector<std::string>& consider_body_list_value);
    muscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, const std::vector<double>& gammavalue, const std::vector<double>& etavalue, const std::vector<std::string>& consider_body_list_value);
    ~muscle();
    void PrintParameters();

    int getnodenum();
    std::string getname();
    // std::vector<double> getrho_o();
    // std::vector<double> getrho_i();
    void setmuscle(const std::vector<body*>& allbody, const std::vector<double>& gamma_o, const std::string& rhoo_bodynamevalue, const std::vector<double>& gamma_i, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, int global, const std::vector<double>& gammavalue, const std::vector<double>& etavalue, const std::vector<std::string>& consider_body_list_value);
    void setmuscle(const std::vector<body*>& allbody, const std::vector<double>& rho_ovalue, const std::string& rhoo_bodynamevalue, const std::vector<double>& rho_ivalue, const std::string& rhoi_bodynamevalue, const std::string& namevalue, int nodenumvalue, const std::vector<double>& gammavalue, const std::vector<double>& etavalue, const std::vector<std::string>& consider_body_list_value);
    std::vector<std::vector<double>> getgammaall();
    std::vector<std::vector<double>> getetaall();
    std::vector<double> getgamma_step(int stepnum);
    std::vector<double> geteta_step(int stepnum);
    std::string getrhoo_bodyname();
    std::vector<double> getrho_o();
    std::string getrhoi_bodyname();
    std::vector<double> getrho_i();
    std::vector<double> getrho_o_position();
    std::vector<double> getrho_i_position();
    body* getrhoo_body();
    body* getrhoi_body();
    std::vector<node*> get_allnodes();
    std::vector<std::string> get_consider_body_list();
    void set_read_muscle_value(int value);
    int get_read_muscle_value();

    void setnodenum(int nodenumvalue);
    void set_node_partition(const std::vector<body*>& allbody, int constraint_local_mode_number, const std::string& constraint_local_select_bodyname, int init_mode_nr, const std::string& init_select_bodyname, int first_step_index);
    void addmuscleparm(const std::vector<double>& muscleparmnew, int local_mode_number);

    void setvia_point_list(const std::vector<body*>& allbody, const std::vector<std::string>& rho_via_point_bodyname, const std::vector<std::vector<double>>& rho_via_value);
    std::vector<node*> getvia_point_list();
    void addvia_point_list(node* viapoint_node);
    void deletevia_point_list();
    void delete_eta_node();

    std::vector<std::vector<double>> interpolation(const std::vector<double>& gamma_o, const std::vector<double>& gamma_i, int nodenum);
    std::vector<double> rearrangeto1D(const std::vector<std::vector<double>>& value);
    std::vector<std::vector<double>> rearrangeto2D(const std::vector<double>& value, int rownum);
    body* findbody(const std::vector<body*>& allbody, const std::string& bodyname);
    std::vector<body*> get_constraint_consider_body(const std::vector<body*>& allbody);
    void setinitialeta_gamma(const std::vector<body*>& allbody);
    void insert_point_node(node* Node_value);

    void print2Dvalue(const std::vector<std::vector<double>>& value);
    void printmuscleinfo();
    void resetforrecalc();
    int getvariablenum(int n_bodies);
    int get_consider_bodynum(int n_bodies);
    void deleteallnodes();


private:
    int nodenum;
    std::string name;
    int read_muscle_value=0;
    int delete_eta=0;
    std::vector<std::string> consider_body_list={};
    std::vector<node*> all_nodes={};
    std::vector<node*> via_point_list={};
};

#endif // MUSCLE_H
