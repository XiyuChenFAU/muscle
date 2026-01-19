import json
import glob
import os
import numpy as np

# 批量处理所有 .json 文件
json_files = glob.glob("*_static.json")  # 若要递归子目录，可改成：glob.glob("**/*.json", recursive=True)

motion_list=["static","abduction", "flexion","sway_backforth","sway_leftright","turning","walking","walking_side"]
nstep=61

for motion in range(len(motion_list)):
    for file_path in json_files:
        with open(file_path, 'r', encoding='utf-8') as f:
            data = json.load(f)

        # 修改 objective.solvercase = 0
        print(file_path)
        data["joint"] = []
        move_setting=[[[0.0,160.0,40.0,0.0]],[],[]]
        rotation_angle=135.0
        movement1=[0.0]
        for i in range(nstep-1):
            if i < (nstep-1)/4:
                delta = rotation_angle/((nstep-1)/4.0)
            elif i < (nstep-1)/2:
                delta = -1 * rotation_angle/((nstep-1)/4.0)
            elif i < 3*(nstep-1)/4:
                delta = rotation_angle/((nstep-1)/4.0)
            else:
                delta = -1 * rotation_angle/((nstep-1)/4.0)
            movement1.append(movement1[-1]+delta)
        movement=[movement1]
        joint1={"joint_name": "joint_elbow", "joint_type_name": "revolute joint","move_setting":move_setting, "movement": movement, "position_relative_rotate_body": [0.0,0.0,0.0],"rotate_body_name": "elbow","rotation_axis_relative_rotate_body": [0.0,1.0,0.0]}
        data["joint"].append(joint1)

        # local setting
        data["constraint"]["local_mode_number"]=0
        data["constraint"]["local_select_bodyname"]="upperarm"
        data["initial_guess"]["mode_number"]=0

        #torso motion
        if not motion==0:
            joint2={"joint_name": "joint_torso", "position_relative_rotate_body": [-73.03582701,70.20097375,-14.32068088]}
            if motion==1: # abduction
                amplitude = 145.0
                delta_torso = 2.0*amplitude/(nstep-1)
                movement2=[0.0]
                for i in range(nstep-1):
                    if i < (nstep-1)/2:
                        movement2.append(movement2[-1]+delta_torso)
                    else:
                        movement2.append(movement2[-1]-delta_torso)
                joint2["joint_type_name"] = "revolute joint"
                joint2["move_setting"] = [[[0.0,40.0,nstep-1,0.0]],[],[]]
                joint2["rotate_body_name"] = "fix_space"
                joint2["movement"] = [movement2]
                joint2["rotation_axis_relative_rotate_body"] = [0.50000237, 0.09139731, 0.86118898]
            elif motion==2: # flexion
                amplitude = 90.0
                delta_torso = 2.0*amplitude/(nstep-1)
                movement2=[0.0]
                for i in range(nstep-1):
                    if i < (nstep-1)/2:
                        movement2.append(movement2[-1]+delta_torso)
                    else:
                        movement2.append(movement2[-1]-delta_torso)
                joint2["joint_type_name"] = "revolute joint"
                joint2["move_setting"] = [[[0.0,40.0,nstep-1,0.0]],[],[]]
                joint2["rotate_body_name"] = "fix_space"
                joint2["movement"] = [movement2]
                joint2["rotation_axis_relative_rotate_body"] = [-7.14353077e-06, -9.94416177e-01, 1.05540231e-01]
            elif motion==3: # sway_backforth
                amplitude = 30.0
                delta_torso = 2.0*amplitude/(nstep-1)
                movement2=[0.0]
                for i in range(nstep-1):
                    if i < (nstep-1)/2:
                        movement2.append(movement2[-1]+delta_torso)
                    else:
                        movement2.append(movement2[-1]-delta_torso)
                joint2["joint_type_name"] = "revolute joint"
                joint2["move_setting"] = [[[0.0,40.0,nstep-1,0.0]],[],[]]
                joint2["rotate_body_name"] = "fix_space"
                joint2["movement"] = [movement2]
                joint2["rotation_axis_relative_rotate_body"] = [-7.14353077e-06, -9.94416177e-01, 1.05540231e-01]
            elif motion==4: # sway_leftright
                amplitude = 40.0
                delta_torso = 2.0*amplitude/(nstep-1)
                movement2=[0.0]
                for i in range(nstep-1):
                    if i < (nstep-1)/2:
                        movement2.append(movement2[-1]+delta_torso)
                    else:
                        movement2.append(movement2[-1]-delta_torso)
                joint2["joint_type_name"] = "revolute joint"
                joint2["move_setting"] = [[[0.0,40.0,nstep-1,0.0]],[],[]]
                joint2["rotate_body_name"] = "fix_space"
                joint2["movement"] = [movement2]
                joint2["rotation_axis_relative_rotate_body"] = [0.50000237, 0.09139731, 0.86118898]
            elif motion==5: # turning
                amplitude = 270.0
                delta_torso = 2.0*amplitude/(nstep-1)
                movement2=[0.0]
                for i in range(nstep-1):
                    if i < (nstep-1)/2:
                        movement2.append(movement2[-1]+delta_torso)
                    else:
                        movement2.append(movement2[-1]-delta_torso)
                joint2["joint_type_name"] = "revolute joint"
                joint2["move_setting"] = [[[0.0,40.0,nstep-1,0.0]],[],[]]
                joint2["rotate_body_name"] = "fix_space"
                joint2["movement"] = [movement2]
                joint2["rotation_axis_relative_rotate_body"] = [0.86602529, -0.05277627, -0.49721005]
            elif motion==6: # walking
                amplitude = 1000.0
                delta_torso = 2.0*amplitude/(nstep-1)
                delta_axis=np.array([0.50000237, 0.09139731, 0.86118898])*delta_torso
                movement2=[[0.0],[0.0],[0.0]]
                for i in range(nstep-1):
                    if i < (nstep-1)/2:
                        movement2[0].append(movement2[0][-1]-delta_axis[0])
                        movement2[1].append(movement2[1][-1]-delta_axis[1])
                        movement2[2].append(movement2[2][-1]-delta_axis[2])
                    else:
                        movement2[0].append(movement2[0][-1]+delta_axis[0])
                        movement2[1].append(movement2[0][-1]+delta_axis[0])
                        movement2[2].append(movement2[0][-1]+delta_axis[0])
                joint2["joint_type_name"] = "translate joint"
                joint2["move_setting"] = [[[0.0,1000.0,nstep-1,0.0]],[[0.0,0.0,nstep-1,0.0]],[[0.0,0.0,nstep-1,0.0]]]
                joint2["rotate_body_name"] = "fix_space"
                joint2["movement"] = movement2
                joint2["rotation_axis_relative_rotate_body"] = [0.0,0.0,0.0]
                joint2["move_all_bodys"] = 1
            elif motion==7: # walking side
                amplitude = 500.0
                delta_torso = 2.0*amplitude/(nstep-1)
                delta_axis=np.array([-7.14353077e-06, -9.94416177e-01, 1.05540231e-01])*delta_torso
                movement2=[[0.0],[0.0],[0.0]]
                for i in range(nstep-1):
                    if i < (nstep-1)/2:
                        movement2[0].append(movement2[0][-1]-delta_axis[0])
                        movement2[1].append(movement2[1][-1]-delta_axis[1])
                        movement2[2].append(movement2[2][-1]-delta_axis[2])
                    else:
                        movement2[0].append(movement2[0][-1]+delta_axis[0])
                        movement2[1].append(movement2[0][-1]+delta_axis[0])
                        movement2[2].append(movement2[0][-1]+delta_axis[0])
                joint2["joint_type_name"] = "translate joint"
                joint2["move_setting"] = [[[0.0,0.0,nstep-1,0.0]],[[0.0,500.0,nstep-1,0.0]],[[0.0,0.0,nstep-1,0.0]]]
                joint2["rotate_body_name"] = "fix_space"
                joint2["movement"] = movement2
                joint2["rotation_axis_relative_rotate_body"] = [0.0,0.0,0.0]
                joint2["move_all_bodys"] = 1
            data["joint"].append(joint2)
        data["name"] = data["name"].split("static")[0]+motion_list[motion]+"_stepnum_"+ str(int((nstep-1)/4))


        # 写回原文件
        with open(file_path.split("static.json")[0]+motion_list[motion]+"_stepnum_"+ str(int((nstep-1)/4)) +".json", 'w', encoding='utf-8') as f1:
            json.dump(data, f1, indent=4, ensure_ascii=False)

        print(f"{file_path} updated.")

        data["name"] = data["name"]+"_local"
        data["constraint"]["local_mode_number"] = 1

        with open(file_path.split("static.json")[0]+motion_list[motion]+"_stepnum_"+ str(int((nstep-1)/4)) +"_local.json", 'w', encoding='utf-8') as f1:
            json.dump(data, f1, indent=4, ensure_ascii=False)

        print(f"{file_path} updated.")
