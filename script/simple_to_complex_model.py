import numpy as np

simple_pos=np.array([[0,0,0]])
simple_axis=np.array([[0.5,0,0.8660254037844386],[0,1,0],[-0.8660254037844386,0,0.5]])
complex_pos=np.array([[-73.035827014784957,70.200973754634219,-14.320680879202911]])
complex_axis=np.array([[1.000001085408982,-7.1435307672229609e-06,-2.1085965897643666e-06],[-6.9363393618571134e-06,-0.99441617725033205,-0.10554052269613706],[-2.0429142966182923e-06,0.10554023061163195,-0.99441656094173325]])
x_axis=np.array([[1,0,0]])
y_axis=np.array([[0,1,0]])
z_axis=np.array([[0,0,1]])


print(complex_pos)
axis_new_x=complex_axis.dot(simple_axis.dot(np.transpose(x_axis)))
axis_new_y=complex_axis.dot(simple_axis.dot(np.transpose(y_axis)))
axis_new_z=complex_axis.dot(simple_axis.dot(np.transpose(z_axis)))
print(axis_new_x)
print(axis_new_y)
print(axis_new_z)
print(np.transpose(axis_new_x).dot(axis_new_x))
print(np.transpose(axis_new_y).dot(axis_new_y))
print(np.transpose(axis_new_z).dot(axis_new_z))

