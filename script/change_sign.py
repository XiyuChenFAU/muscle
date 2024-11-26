import pandas as pd

name='shoulder_ellipsoid_0_150_scapular_plane_length_1000_node_30'

change_sign=["Infraspinatus_I-joint_head", "Infraspinatus_S-joint_head", "TeresMinor-joint_head", "TeresMajor-joint_head"]
df = pd.read_csv(name+'_momentarm_result_origin.txt', delimiter='\t')
df = df.dropna(axis=1, how='any')
for q in change_sign:
    mask = df.iloc[:, 0].astype(str).str.contains(q, case=False, na=False)
    for idx in df[mask].index:
        df.iloc[idx, 3:] = df.iloc[idx, 3:] * -1.0

df.to_csv(name+'_momentarm_result.txt', sep='\t', index=False)

df = pd.read_csv(name+'_momentarmnode_result_origin.txt', delimiter='\t')
df = df.dropna(axis=1, how='any')
for q in change_sign:
    mask = df.iloc[:, 0].astype(str).str.contains(q, case=False, na=False)
    for idx in df[mask].index:
        df.iloc[idx, 3:] = df.iloc[idx, 3:] * -1.0

df.to_csv(name+'_momentarmnode_result.txt', sep='\t', index=False)
