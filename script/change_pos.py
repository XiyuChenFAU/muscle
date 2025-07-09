import pandas as pd
import numpy as np

"""
axis 10T=[[d1]
          [d2]
          [d3]]
"""

wrap_scapula_blade_q=[0.00274258, 0.111457, 0.116205, 0.780718, 0.283358, 0.556945, -0.465345, 0.858484, 0.215544, -0.417052, -0.42745, 0.802093]
upperarm_q=[-0.0711612, 0.0679275, 0.0276005, 1.0, -2.9012e-06, -3.95377e-06, -2.74621e-06, -0.994415, -0.105541, -4.00139e-06, 0.105541, -0.994415]

triceps_local_upperarm=np.array([-0.0369, 0, -0.1])
biceps_local_upperarm=np.array([0.0369, 0, -0.114])
triceps_global_upperarm=np.array([-0.108061, 0.0573735, 0.127042])
biceps_global_upperarm=np.array([-0.0342607, 0.0558957, 0.140964])

def q_pos_axis(q_value):
    position=np.array([q_value[0],q_value[1],q_value[2]])
    axis=np.array([
            [q_value[3],q_value[4],q_value[5]],
            [q_value[6],q_value[7],q_value[8]], 
            [q_value[9],q_value[10],q_value[11]]
    ])
    return position,axis

def local_to_global(position, axis, value):
    return axis.T @ value+ position

def global_to_local(position, axis, value):
    return axis @ (value - position)

upperarm_pos, upperarm_axis = q_pos_axis(upperarm_q)
wrap_scapula_blade_pos, wrap_scapula_blade_axis = q_pos_axis(wrap_scapula_blade_q)
triceps_wrap_scapula_blade = global_to_local(wrap_scapula_blade_pos, wrap_scapula_blade_axis, local_to_global(upperarm_pos, upperarm_axis, triceps_local_upperarm))
biceps_wrap_scapula_blade = global_to_local(wrap_scapula_blade_pos, wrap_scapula_blade_axis, local_to_global(upperarm_pos, upperarm_axis, biceps_local_upperarm))
print("Triceps local position:", triceps_wrap_scapula_blade)
print("Biceps local position:", biceps_wrap_scapula_blade)
print("Triceps global position:", local_to_global(upperarm_pos, upperarm_axis, triceps_local_upperarm))
print("Biceps global position:", local_to_global(upperarm_pos, upperarm_axis, biceps_local_upperarm))

