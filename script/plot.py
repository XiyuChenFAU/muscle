import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm

# extract first line and first row from excel
def extractxyaxis(lines, initial):
    lock = 0
    muscles = []
    angles = []
    nodenum = []
    for i in range(len(lines)):
        row = lines[i].strip().split('\t')
        if (row[0] not in muscles) and (row[0] != 'rotation angle'):
            muscles.append(row[0])  # find all muscles
            lock = 1
        if lock == 1:
            if row[0] == muscles[0]:
                nodenum.append(row[2])
        if i==0:   # extract all angles
            for j in range(len(row)):
                if initial==1 and j>3:
                    angles.append(float(row[j]))
                elif initial==0 and j>2:
                    angles.append(float(row[j]))
    return muscles, angles, nodenum

# arrange raw data to a 2d or 3d list 
def arrangetolist(lines, muscles, filename, inital):
    allmuscledata = []
    if 'totalforce' not in filename:
        for i in range(len(muscles)):
            muscledata = []
            for line in lines:
                row = line.strip().split('\t')
                if row[0] == muscles[i]:
                    rowdata = []
                    for j in range(len(row)-3-inital):
                            rowdata.append(float(row[j+3+inital]))                        
                    muscledata.append(rowdata)
            allmuscledata.append(muscledata)
    else:
        for line in lines:
            row = line.strip().split('\t')
            if row[0] != 'rotation angle':
                muscledata = []
                for j in range(len(row)-3-inital):
                    muscledata.append(float(row[j+3+inital]))
                allmuscledata.append(muscledata)

    return allmuscledata

def pcolorfig(x, y, data2d, ylabelname, title, prefixname):
    plt.figure()
    plt.pcolormesh(x, y, data2d, cmap='viridis')
    plt.colorbar()
    plt.xticks(x, rotation=45)
    plt.yticks(y)
    plt.xlabel('angle')
    plt.ylabel(ylabelname)
    plt.title(prefixname +title)
    figname = prefixname + title + '.png'
    plt.savefig(figname)

def normalfig(data, linename, x, ylabelname, title):
    plt.figure()
    for i in range(len(data)):
        if 'length' in title:
            sum = np.sum(np.array(data[i]), axis=0)
            plt.plot(x, sum, label=linename[i])
        else:
            plt.plot(x, data[i], label=linename[i])

    plt.xticks(x, rotation=45)
    plt.xlabel('angle')
    plt.ylabel(ylabelname)
    plt.legend()
    figurename = title + '.png'
    plt.savefig(figurename)   

def gatherdataandplot(data, another):
    allpenatration = []
    for m in range(len(data[0])):
        oneline = []
        for n in range(len(data[0][0])):
            values = []
            for p in range(len(another)):
                values.append(data[p][m][n])
            if np.min(np.array(values))<=0:
                penatration = np.min(np.array(values))
            else:
                penatration = 0
            oneline.append(penatration)
        allpenatration.append(oneline)
    return allpenatration
    

def penatrationplot(data, x, y, musclepair):
    # find all muscles and tissues first
    muscles = []
    tissues = []
    for i in range(len(musclepair)):
        name = musclepair[i].strip().split('-')
        if name[0] not in muscles:
            muscles.append(name[0])
        if name[1] not in tissues:
            tissues.append(name[1])
    
    # figure accoding to muscle type
    for i in range(len(muscles)):
        onemuscledata = []
        for j in range(len(musclepair)):
            if muscles[i] in musclepair[j]:
                onemuscledata.append(data[j])
        allpenatration = gatherdataandplot(onemuscledata, tissues)
        pcolorfig(x, y, allpenatration, 'node number', muscles[i], 'penetration_')

    # figure according to tissue type
    for i in range(len(tissues)):
        onetissuedata = []
        for j in range(len(musclepair)):
            if tissues[i] in musclepair[j]:
                onetissuedata.append(data[j])
        allpenatration = gatherdataandplot(onetissuedata, muscles)
        pcolorfig(x, y, allpenatration, 'node number', tissues[i], 'penetration_')
        
        

prefix = 'elbow_model_'   
types = ['length', 'forcenode', 'phi', 'totalforce']

for i in range(len(types)):
    filename = prefix + types[i] +'_result.txt'
    with open(filename, 'r') as file:
        lines = file.readlines()

    
    if ('forcenode' in filename) or ('totalforce' in filename):
        muscles, angles, nodenum = extractxyaxis(lines, 0)
        allmuscledata = arrangetolist(lines, muscles, filename, 0)

        # plot
        if 'forcenode' in filename:
            for i in range(len(allmuscledata)):
                pcolorfig(angles, nodenum, np.array(allmuscledata[i]), 'node number', muscles[i], 'forcenode_')
        elif 'totalforce' in filename:
            normalfig(allmuscledata, muscles, angles, 'total force', 'totalforce')   
    else:
        muscles, angles, nodenum = extractxyaxis(lines, 1)
        allmuscledata = arrangetolist(lines, muscles, filename, 1)

        # plot
        if 'length' in filename:
            normalfig(allmuscledata, muscles, angles, 'total deformation', 'length')
        elif 'phi' in filename:
            for i in range(len(allmuscledata)):
                pcolorfig(angles, nodenum, np.array(allmuscledata[i]), 'node number', muscles[i], 'phi_')

            penatrationplot(allmuscledata, angles, nodenum, muscles)
    
    