import os
import numpy as np
import pandas as pd
import glob
import sys

def global_to_local(node, q):
    local_node=[node[0]-q[0], node[1]-q[1], node[2]-q[2]]
    x = local_node[0]*q[3] + local_node[1]*q[4] + local_node[2]*q[5]
    y = local_node[0]*q[6] + local_node[1]*q[7] + local_node[2]*q[8]
    z = local_node[0]*q[9] + local_node[1]*q[10] + local_node[2]*q[11]
    return [x, y, z]

def local_to_global(node, q):
    x = node[0]*q[3] + node[1]*q[6] + node[2]*q[9]
    y = node[0]*q[4] + node[1]*q[7] + node[2]*q[10]
    z = node[0]*q[5] + node[1]*q[8] + node[2]*q[11]
    return [x + q[0], y + q[1], z + q[2]]

def phi_ellipsoid(local_node, bodyvalue):
    return (local_node[0]**2)/(bodyvalue[0]**2) + (local_node[1]**2)/(bodyvalue[1]**2) + (local_node[2]**2)/(bodyvalue[2]**2)-1

def change_orientation(node, q_old, oldbody_value, q_new, newbody_value):
    local_node=global_to_local(node, q_old)
    phi_old = phi_ellipsoid(local_node, oldbody_value)
    phi_new = phi_ellipsoid(local_node, newbody_value)
    x_time_value = 0
    if not phi_new +1.0==0:
        x_time_value = np.sqrt(abs((phi_old+1)/(phi_new+1)))
    new_local_node = [x_time_value * local_node[0], x_time_value * local_node[1], x_time_value * local_node[2]]
    new_global_node = local_to_global(new_local_node, q_new)
    return new_local_node, new_global_node

def change_orientation_two_body_case(node, q_old_ini, q_old, oldbody_value, nodenew, qnew_ini, q_new, newbody_value, time_t):
    local_node_old=global_to_local(local_to_global(node, q_old_ini), q_old)
    phi_old = phi_ellipsoid(local_node_old, oldbody_value)

    local_node_new=global_to_local(local_to_global([nodenew[0]*time_t, nodenew[1]*time_t, nodenew[2]*time_t], qnew_ini), q_new)
    phi_new = phi_ellipsoid(local_node_new, newbody_value)
    return phi_new-phi_old


def change_orientation_two_body_case_self_penetration(nodenew, qnew_ini, q_new, newbody_value, time_t):
    local_node_new=global_to_local(local_to_global([nodenew[0]*time_t, nodenew[1]*time_t, nodenew[2]*time_t], qnew_ini), q_new)
    phi_new_initial = phi_ellipsoid([nodenew[0]*time_t, nodenew[1]*time_t, nodenew[2]*time_t], newbody_value)
    phi_new = phi_ellipsoid(local_node_new, newbody_value)
    if phi_new_initial<0:
        phi_new=1000000
    if phi_new<0:
        phi_new=10000
    return phi_new

q_old_ini=[-0.0652797, 	0.0607938,	0.159101,	0.170033,	-0.965324,	-0.198089,	0.276023,	-0.146315,	0.949949,	-0.945992,	-0.216199,	0.241573]
q_old=[-0.0711611, 0.0679275, 0.0276008, 1, -3.18895e-06, -3.82474e-06, -3.3336e-06, -0.994415, -0.105541, -3.62956e-06, 0.105541, -0.994415]
oldbody_value=[0.015, 0.015, 0.1425]

q_new_ini=[-0.0652797, 	0.0607938,	0.159101,	0.170033,	-0.965324,	-0.198089,	0.276023,	-0.146315,	0.949949,	-0.945992,	-0.216199,	0.241573]
q_new=[-0.0730361, 0.0702017, -0.0143209, 1, -2.9012e-06, -3.95377e-06, -2.74621e-06, -0.994415, -0.105541, -4.00139e-06, 0.105541, -0.994415]

newbody_value=[0.045, 0.045, 0.17385]


node_TeresMinor=[-0.0033096900000000001, -0.015683099999999998, 0.017922400000000002]
result_teresminor=[]
test_range_teresminor=range(0,3000)
for i in test_range_teresminor:
    result_teresminor.append(change_orientation_two_body_case(node_TeresMinor, q_old_ini, q_old, oldbody_value, node_TeresMinor, q_new_ini, q_new, newbody_value, 1+i/1000))
min_index_teresminor=np.argmin(np.abs(np.array(result_teresminor)))
print(min_index_teresminor)
print(result_teresminor[min_index_teresminor])
node_TeresMinor_new=[node_TeresMinor[0]*(1+test_range_teresminor[min_index_teresminor]/1000), node_TeresMinor[1]*(1+test_range_teresminor[min_index_teresminor]/1000), node_TeresMinor[2]*(1+test_range_teresminor[min_index_teresminor]/1000)]
print(node_TeresMinor_new)

node_TeresMajor=[0.0033711600000000002, -0.048582199999999999, -0.015198700000000001]
result_TeresMajor=[]
test_range_TeresMajor=range(-2000,1000)
for i in test_range_TeresMajor:
    result_TeresMajor.append(change_orientation_two_body_case_self_penetration(node_TeresMajor, q_new_ini, q_new, newbody_value, 1+i/1000))
min_index_TeresMajor=np.argmin(np.abs(np.array(result_TeresMajor)))
print(min_index_TeresMajor)
print(result_TeresMajor[min_index_TeresMajor])
node_TeresMajor_new=[node_TeresMajor[0]*(1+test_range_TeresMajor[min_index_TeresMajor]/1000), node_TeresMajor[1]*(1+test_range_TeresMajor[min_index_TeresMajor]/1000), node_TeresMajor[2]*(1+test_range_TeresMajor[min_index_TeresMajor]/1000)]
print(node_TeresMajor_new)
