/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include "../parm/parm.h"

class postprocessing {
public:
    postprocessing();
    ~postprocessing();
    void do_postprocessingall(Parm* parm);
    void get_momentarmall(Parm* parm);
    std::vector<std::vector<double>> get_momentarm_per_muscle(const std::vector<std::vector<double>>& gamma, joint* Joint);
    std::vector<double> get_momentarm_per_node(const std::vector<double>& gamma, joint* Joint, int step);
    void get_force_allmuscle(Parm* parm);
    std::vector<std::vector<std::vector<double>>> get_force_each_muscle(Parm* parm, int musclenum);
    std::vector<std::vector<double>> get_force_each_muscle_each_time(const std::vector<double>& gamma, const std::vector<std::vector<double>>& phi, double lengthchange);
    void get_length_allmuscle(Parm* parm);
    std::vector<std::vector<double>> get_length_each_muscle(const std::vector<std::vector<double>>& gamma);
    std::vector<double> get_length_each_muscle_each_node(const std::vector<double>& gamma);
    void get_hill_passive_force_allmuscle(Parm* parm);
    std::vector<double> get_hill_passive_force_each_muscle(const std::vector<std::vector<double>>& gamma, const std::vector<double>& hill_par);
    void get_hill_active_force_allmuscle(Parm* parm);
    std::vector<double> get_hill_active_force_each_muscle(const std::vector<std::vector<double>>& gamma, const std::vector<double>& hill_par);
    void get_hill_total_force_allmuscle(Parm* parm);
    void get_hill_moment_allmuscle(Parm* parm);
    void getphiall(Parm* parm);
    std::vector<std::vector<double>> phi_shape_all(const std::vector<std::vector<double>>& gamma, body* Body);
    std::vector<double> phi_shape_allnode(const std::vector<double>& gamma, body* Body, int timenum);
    double phi_shape(const std::vector<double>& gamma, body* Body, int timenum);
    void settol(double tolvalue);
    double gettol();

    std::vector<std::vector<std::vector<double>>> getphiall();
    std::vector<std::vector<std::vector<double>>> getlengthall();
    std::vector<std::vector<double>> gettotalforceall();
    std::vector<std::vector<std::vector<double>>> getforceallnode();
    std::vector<std::vector<double>> getmomentarmall();
    std::vector<std::vector<std::vector<double>>> getmomentarmnodeall();
    std::vector<std::vector<double>> gethillpassiveforceall();
    std::vector<std::vector<double>> gethillactiveforceall();
    std::vector<std::vector<double>> gethilltotalforceall();
    std::vector<std::vector<double>> gethillmomentall();

private:

    double E=440400.0;
    double area=0.00007;
    double tol=0.001;
    std::vector<std::vector<std::vector<double>>> phiall;
    std::vector<std::vector<std::vector<double>>> lengthall;
    std::vector<std::vector<double>> totalforceall;
    std::vector<std::vector<std::vector<double>>> forceallnode;
    std::vector<std::vector<double>> momentarmall;
    std::vector<std::vector<std::vector<double>>> momentarmnodeall;
    std::vector<std::vector<double>> hillpassiveforceall;
    std::vector<std::vector<double>> hillactiveforceall;
    std::vector<std::vector<double>> hilltotalforceall;
    std::vector<std::vector<double>> hillmomentall;

};

#endif // POSTPROCESSING_H
