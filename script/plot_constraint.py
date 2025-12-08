from platform import node
import numpy as np
import matplotlib.pyplot as plt
import os
import sys
import pandas as pd

def list_minus(x1,x2):
    return [x1[i] - x2[i] for i in range(len(x1))]

def list_plus(x1,x2):
    return [x1[i] + x2[i] for i in range(len(x1))]

def list_time(x1,x2):
    return [x1[i] * x2[i] for i in range(len(x1))]

def list_divide(x1,x2):
    return [x1[i] / x2[i] for i in range(len(x1))]

def Jacobian_constraint_ellipsoid_node(node, q, body_value):
    x=list_minus(node, q[:3])
    axis = [
        [q[3], q[4], q[5]],
        [q[6], q[7], q[8]],
        [q[9], q[10], q[11]]
    ]
    a = body_value[0]
    b = body_value[1]
    c = body_value[2]

    a_sq = a * a
    b_sq = b * b
    c_sq = c * c

    G = [0.0, 0.0, 0.0]

    for i in range(3):
        dot0 = x[0] * axis[0][0] + x[1] * axis[0][1] + x[2] * axis[0][2]
        dot1 = x[0] * axis[1][0] + x[1] * axis[1][1] + x[2] * axis[1][2]
        dot2 = x[0] * axis[2][0] + x[1] * axis[2][1] + x[2] * axis[2][2]

        G[i] = 2.0 * (
            (dot0 / a_sq) * axis[0][i] +
            (dot1 / b_sq) * axis[1][i] +
            (dot2 / c_sq) * axis[2][i]
        )
    return G

def Jacobian_constraint_ellipsoid_node_all(node, q, body_value, bodyshape):
    Jacobian_all=[]
    for i in range(len(q)):
        if bodyshape[i]=="ellipsoid":
            Jacobian_all.append(Jacobian_constraint_ellipsoid_node(node, q[i], body_value[i]))
    return Jacobian_all

def constraint_ellipsoid_node(node, q, body_value):
    x=list_minus(node, q[:3])
    axis = [
        [q[3], q[4], q[5]],
        [q[6], q[7], q[8]],
        [q[9], q[10], q[11]]
    ]
    a = body_value[0]
    b = body_value[1]
    c = body_value[2]

    a_sq = a * a
    b_sq = b * b
    c_sq = c * c

    dot0 = x[0] * axis[0][0] + x[1] * axis[0][1] + x[2] * axis[0][2]
    dot1 = x[0] * axis[1][0] + x[1] * axis[1][1] + x[2] * axis[1][2]
    dot2 = x[0] * axis[2][0] + x[1] * axis[2][1] + x[2] * axis[2][2]

    constraint_value = (dot0 * dot0) / a_sq + (dot1 * dot1) / b_sq + (dot2 * dot2) / c_sq - 1.0
    return constraint_value

def constraint_ellipsoid_node_all(node, q, body_value, bodyshape):
    constraint_all=[]
    for i in range(len(q)):
        if bodyshape[i]=="ellipsoid":
            constraint_all.append(constraint_ellipsoid_node(node, q[i], body_value[i]))
    return constraint_all

def geodesic_constraint(gamma, q, body_value, shapename, eta):
    K_value = int(len(gamma) / 3) - 1
    body_num=len(q)
    all_value = [] #geodesic (node_1_x,node_1_x,node_1_x, node_2_x,node_2_y,node_2_z,...)
    for i in range(1,K_value):
        all_value_per_node=[]
        Jacobian_all = Jacobian_constraint_ellipsoid_node_all(gamma[i*3:(i+1)*3], q, body_value, shapename) #all_body_q, all_body_value(a,b,c), all_body_shape
        for j in range(3):
            geodesic = (2 * gamma[i*3+j] - gamma[(i-1)*3+j] - gamma[(i+1)*3+j]) * K_value
            time_eta_value = 0
            for k in range(len(Jacobian_all)):
                time_eta_value = time_eta_value + eta[(i-1)*body_num+k] * Jacobian_all[k][j]  #+ eta * sum_over_bodies(Jacobian)
            constraint_geodesic_value = geodesic - time_eta_value/K_value
            all_value_per_node.append(constraint_geodesic_value)
        all_value.append(all_value_per_node)
    return all_value

def constraint_time_eta(gamma, q, body_value, shapename, eta):
    K_value = int(len(gamma) / 3) - 1
    body_num=len(q)
    all_value = [] #geodesic (node_1_x,node_1_x,node_1_x, node_2_x,node_2_y,node_2_z,...)
    for i in range(1,K_value):
        all_value_per_node=[]
        constraint_all = constraint_ellipsoid_node_all(gamma[i*3:(i+1)*3], q, body_value, shapename) #all_body_q, all_body_value(a,b,c), all_body_shape
        for j in range(len(constraint_all)):
            all_value_per_node.append(eta[(i-1)*body_num+j]*constraint_all[j])
        all_value.append(all_value_per_node)
    return all_value

def constraint_shape_and_eta(gamma, q, body_value, shapename, eta):
    K_value = int(len(gamma) / 3) - 1
    body_num=len(q)
    all_value_phi, all_value_eta = [], [] #geodesic (node_1_x,node_1_x,node_1_x, node_2_x,node_2_y,node_2_z,...)
    for i in range(1,K_value):
        all_value_per_node_phi, all_value_per_node_eta = [], []
        constraint_all = constraint_ellipsoid_node_all(gamma[i*3:(i+1)*3], q, body_value, shapename) #all_body_q, all_body_value(a,b,c), all_body_shape
        for j in range(len(constraint_all)):
            all_value_per_node_phi.append(constraint_all[j])
            all_value_per_node_eta.append(eta[(i-1)*body_num+j])
        all_value_phi.append(all_value_per_node_phi)
        all_value_eta.append(all_value_per_node_eta)
    return all_value_phi, all_value_eta

def read_data(modelname, select_col_body, select_col_muscle):
    df_bodystate = pd.read_csv(modelname+'_bodystate_result.txt', sep='\t')
    df_q = pd.read_csv(modelname+'_body_result.txt', sep='\t')
    df_gamma = pd.read_csv(modelname+'_muscle_gamma_result.txt', sep='\t')
    df_eta = pd.read_csv(modelname+'_muscle_eta_result.txt', sep='\t')
    shapename, body_value, q, bodyname = [], [], [], []
    for body in df_bodystate.iloc[:,0].unique():
        df_bodystate_single=df_bodystate[df_bodystate.iloc[:,0]==body]
        shapename.append(str(df_bodystate_single.iloc[0,1]))
        body_a=df_bodystate_single[df_bodystate_single[" .1"]=="a"].iloc[:,3+select_col_body].values.tolist()[0]
        body_b=df_bodystate_single[df_bodystate_single[" .1"]=="b"].iloc[:,3+select_col_body].values.tolist()[0]
        body_c=df_bodystate_single[df_bodystate_single[" .1"]=="c"].iloc[:,3+select_col_body].values.tolist()[0]
        body_value_single=[body_a, body_b, body_c]
        df_q_single=df_q[df_q.iloc[:,0]==body]
        qsingle=df_q_single.iloc[:,3+select_col_body].values.tolist()
        q.append(qsingle)
        bodyname.append(body)
        body_value.append(body_value_single)
    gamma_value_all, eta_value_all, musclename = [], [], []
    for muscle in df_gamma.iloc[:,0].unique():
        musclename.append(muscle)
        df_gamma_single=df_gamma[df_gamma.iloc[:,0]==muscle]
        gamma_value=df_gamma_single.iloc[:,3+select_col_muscle].values.tolist()
        df_eta_single=df_eta[df_eta.iloc[:,0]==muscle]
        eta_value=df_eta_single.iloc[:,3+select_col_muscle].values.tolist()
        gamma_value_all.append(gamma_value)
        eta_value_all.append(eta_value)
    return shapename, body_value, q, bodyname,gamma_value_all, eta_value_all, musclename



def plot_constraints(modelname, select_col_body, select_col_muscle, output_dir = "constraint_plots"):
    shapename, body_value, q, bodyname, gamma_value, eta_value, musclename = read_data(modelname, select_col_body, select_col_muscle)
    
    for i in range(len(gamma_value)):
        geodesic_value = geodesic_constraint(gamma_value[i], q, body_value, shapename, eta_value[i])
        geodesic_rearrange = np.array(geodesic_value).transpose()
        geodesic_label = ["x", "y", "z"]
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        fig = plt.figure()  
        for j in range(len(geodesic_rearrange)):
            plt.plot(range(1, geodesic_rearrange[j].shape[0] + 1), geodesic_rearrange[j], 
                    label=geodesic_label[j], linewidth=2)
        
        plt.title(f'geodesic constraint for muscle {musclename[i]}', fontsize=14)
        plt.xlabel('node index', fontsize=12)
        plt.ylabel('geodesic constraint value', fontsize=12)
        plt.legend(fontsize=10)
        filename = f'{output_dir}/muscle_{musclename[i]}_geodesic_constraint'
        plt.savefig(f'{filename}.png', dpi=300, bbox_inches='tight')
        plt.close(fig)
        
        constraint_time_eta_value = constraint_time_eta(gamma_value[i], q, body_value, shapename, eta_value[i])
        constraint_time_eta_value_rearrange = np.array(constraint_time_eta_value).transpose()
        fig = plt.figure()  
        for j in range(len(constraint_time_eta_value_rearrange)):
            plt.plot(range(1, constraint_time_eta_value_rearrange[j].shape[0] + 1), constraint_time_eta_value_rearrange[j], 
                    label=bodyname[j], linewidth=2)
        
        plt.title(f'phi*eta constraint for muscle {musclename[i]}', fontsize=14)
        plt.xlabel('node index', fontsize=12)
        plt.ylabel('phi*eta constraint value', fontsize=12)
        plt.legend(fontsize=10)
        filename = f'{output_dir}/muscle_{musclename[i]}_phi_eta_constraint'
        plt.savefig(f'{filename}.png', dpi=300, bbox_inches='tight')
        plt.close(fig)

        phi_value, eta_value_plot = constraint_shape_and_eta(gamma_value[i], q, body_value, shapename, eta_value[i])
        phi_value_rearrange = np.array(phi_value).transpose()
        fig = plt.figure()  
        for j in range(len(phi_value_rearrange)):
            plt.plot(range(1, phi_value_rearrange[j].shape[0] + 1), phi_value_rearrange[j], 
                    label=bodyname[j], linewidth=2)
        plt.title(f'phi constraint for muscle {musclename[i]}', fontsize=14)
        plt.xlabel('node index', fontsize=12)
        plt.ylabel('phi constraint value', fontsize=12)
        plt.legend(fontsize=10)
        filename = f'{output_dir}/muscle_{musclename[i]}_phi_constraint'
        plt.savefig(f'{filename}.png', dpi=300, bbox_inches='tight')
        plt.close(fig)

        eta_value_rearrange = np.array(eta_value_plot).transpose()
        fig = plt.figure()  
        for j in range(len(eta_value_rearrange)):
            plt.plot(range(1, eta_value_rearrange[j].shape[0] + 1), eta_value_rearrange[j], 
                    label=bodyname[j], linewidth=2)
        plt.title(f'eta constraint for muscle {musclename[i]}', fontsize=14)
        plt.xlabel('node index', fontsize=12)
        plt.ylabel('eta constraint value', fontsize=12)
        plt.legend(fontsize=10)
        filename = f'{output_dir}/muscle_{musclename[i]}_eta_constraint'
        plt.savefig(f'{filename}.png', dpi=300, bbox_inches='tight')
        plt.close(fig)


modelname_value="elbow_model_static"
select_col_body_value=0
select_col_muscle_value=0
plot_constraints(modelname_value, select_col_body_value, select_col_muscle_value)