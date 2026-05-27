import numpy as np

shoulder_upper=np.array([15.0,15.0,142.5])

Johann_upper=np.array([45.0,45.0,173.85])
Johann_elbow=np.array([45.0,43.0,37.0])
Johann_lower=np.array([38.0,38.0,142.0])

tri_origin=np.array([0.0,45.0,-121.695])
tri_insertion=np.array([0.0,45.6,-127.8])

bi_origin=np.array([0.0,-45.0,-139.08])
bi_insertion=np.array([38.0,0.0,-85.2])

for i in range(3):
    ratio = shoulder_upper[i]/Johann_upper[i]
    Johann_upper[i] = ratio*Johann_upper[i]
    Johann_elbow[i] = ratio*Johann_elbow[i]
    Johann_lower[i] = ratio*Johann_lower[i]
    tri_origin[i] = ratio*tri_origin[i]
    tri_insertion[i] = ratio*tri_insertion[i]
    bi_origin[i] = ratio*bi_origin[i]
    bi_insertion[i] = ratio*bi_insertion[i]

print(Johann_upper)
print(Johann_elbow)
print(Johann_lower)
print(tri_origin)
print(tri_insertion)
print(bi_origin)
print(bi_insertion)

