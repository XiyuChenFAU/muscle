import matplotlib.pyplot as plt
import numpy as np
import sys
from matplotlib.lines import Line2D
import os
import glob

def readarrangedata(filename):
    f=open(filename,"r")
    titleline=0
    rotationangle,data,musclename=[],[],[]
    for i in f:
        if titleline==0:
            line=i.strip().split("\t")
            titleline=1
            for j in range(4,len(line)):
                rotationangle.append(float(line[j]))
        else:
            line=i.strip().split("\t")
            data1=[]
            for j in range(4,len(line)):
                data1.append(1000*float(line[j]))
            data.append(data1)
            musclename.append(line[0].split("-")[0])
    f.close()
    return [rotationangle,data,musclename]
    
def readdatafromliterature(filename):
    f=open(filename,"r")
    rotationangle,data=[],[]
    for i in f:
        line=i.strip().split()
        rotationangle.append(float(line[0].replace(",",".")))
        data.append(-1*float(line[-1].replace(",",".")))
    f.close()
    return [rotationangle,data]
    
def find_files_with_keyword(folder_path, keyword):
    csv_files = glob.glob(os.path.join(folder_path, '*.csv'))
    files_with_test_keyword = [file for file in csv_files if keyword in os.path.basename(file)]
    return files_with_test_keyword

argv=sys.argv
alldata=readarrangedata(argv[1]+"_momentarm_result.txt")
columnnumber=5
rownumber=2
fig, axes = plt.subplots(nrows=rownumber, ncols=columnnumber, figsize=(12, 5))
literatures=[]
colors=["black","red","orange","purple","green","cyan"]
for i in range(9):
    literaturefilename=find_files_with_keyword("../../moment_arm_literature_result/2018_Hik_Fig.8/", alldata[2][i].split("_")[0])
    ax = axes[i // columnnumber, i % columnnumber]
    ax.plot(alldata[0], alldata[1][i], color=colors[0])
    ax.set_title(alldata[2][i])
    ax.set_ylim(-30, 30)
    ax.set_xlim(-110, 110)
    if i%columnnumber==0:
        ax.set_ylabel("Muscle\nMoment arm (mm)")
    ax.set_xlabel("Axial rotation angle (deg)")
    
    
    for j in literaturefilename:
        index=0
        file=j.split("/")[-1]
        labelname=file.split("_")[-2]+file.split("_")[-1].split(".")[0]
        if labelname in literatures:
            index=literatures.index(labelname)
        else:
            index=len(literatures)
            literatures.append(labelname)
            axes[-1, -1].plot([], [], label=labelname, color=colors[(index+1)%6])
        dataliterature=readdatafromliterature(j)
        ax.plot(dataliterature[0], dataliterature[1], color=colors[index])

print(literatures)
axes[-1, -1].plot([], [], label="Geodesic model", color=colors[0])
axes[-1,-1].legend()
axes[-1,-1].set_axis_off()

plt.tight_layout()
plt.show()
