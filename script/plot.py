import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.colors import LogNorm
from mpl_toolkits.mplot3d import Axes3D
import os
import random
import sys
from PIL import Image

def rotate_vector(vector, axis, angle):
    # Normalize the axis vector
    axis = axis / np.linalg.norm(axis)

    # Calculate the rotation matrix
    c = np.cos(angle)
    s = np.sin(angle)
    rotation_matrix = np.array([
        [c + (1 - c) * axis[0] ** 2, (1 - c) * axis[0] * axis[1] - s * axis[2], (1 - c) * axis[0] * axis[2] + s * axis[1]],
        [(1 - c) * axis[1] * axis[0] + s * axis[2], c + (1 - c) * axis[1] ** 2, (1 - c) * axis[1] * axis[2] - s * axis[0]],
        [(1 - c) * axis[2] * axis[0] - s * axis[1], (1 - c) * axis[2] * axis[1] + s * axis[0], c + (1 - c) * axis[2] ** 2]
    ])

    # Apply the rotation to the vector
    rotated_vector = np.dot(rotation_matrix, vector)

    return rotated_vector

def plot_ellipsoid(ax, a, b, c, axis, rotationangle, translate, colorval, stride):
    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(0, np.pi, 100)
    x = a * np.outer(np.cos(u), np.sin(v))
    y = b * np.outer(np.sin(u), np.sin(v))
    z = c * np.outer(np.ones_like(u), np.cos(v))

    # Perform rotation around the specified axis
    for i in range(len(x)):
        for j in range(len(x[i])):
            vector = np.array([x[i][j], y[i][j], z[i][j]])
            rotated_vector = rotate_vector(vector, axis, rotationangle/180*np.pi)
            x[i][j], y[i][j], z[i][j] = rotated_vector

    x += translate[0]
    y += translate[1]
    z += translate[2]
    
    ax.plot_wireframe(x, y, z, linewidth=0.3, color=colorval, cstride=stride)
    #ax.plot_surface(x, y, z, color=colorval) #suface

def plot_elliptical_cylinder(ax,a, b, c, axis, rotationangle, translate, colorval, stride):
    # Generate points for the elliptical cylinder
    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(-c, c, 100)
    x = a * np.outer(np.cos(u), np.ones_like(v))
    y = b * np.outer(np.sin(u), np.ones_like(v))
    z = np.outer(np.ones_like(u), v)

    # Perform rotation around the specified axis
    for i in range(len(x)):
        for j in range(len(x[i])):
            vector = np.array([x[i][j], y[i][j], z[i][j]])
            rotated_vector = rotate_vector(vector, axis, rotationangle)
            x[i][j], y[i][j], z[i][j] = rotated_vector

    x += translate[0]
    y += translate[1]
    z += translate[2]
    ax.plot_wireframe(x, y, z, linewidth=0.3, color=colorval, cstride=stride)
    #ax.plot_surface(x, y, z, color=colorval) #suface


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
    
def getshapename(filename):
    f=open(filename,"r")
    line=0
    bodyname=[]
    shapename=[]
    for i in f:
        if not line==0:
            if not i.strip().split('\t')[0] in bodyname:
                bodyname.append(i.strip().split('\t')[0])
                shapename.append(i.strip().split('\t')[1])
        line=line+1
    f.close()

    return shapename

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
            if 'length' == title:
                plt.plot(x[1:], sum[1:]-sum[:-1], linestyle='--', label=linename[i]+" difference")
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
        
def bodyrotateplot(allmuscledata, allmuscledatagamma, shapename,foldername,scale=1.0):
    gammaarray=np.array(allmuscledatagamma)
    stride = 50  # Adjust the stride value to control the sparsity of lines
    colors=[]
    for i in range(len(allmuscledata)):
        color = "#{:02x}{:02x}{:02x}".format(random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))
        colors.append(color)

    for i in range(len(allmuscledata[0][0])):
        figurename=foldername+'/bodyfigure'+str(i)
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        for j in range(len(allmuscledata)):
            axis=[allmuscledata[j][3][i],allmuscledata[j][4][i],allmuscledata[j][5][i]]
            translate=[allmuscledata[j][7][i],allmuscledata[j][8][i],allmuscledata[j][9][i]]
            if shapename[j]=="ellipsoid":
                plot_ellipsoid(ax,allmuscledata[j][0][i], allmuscledata[j][1][i], allmuscledata[j][2][i], axis, allmuscledata[j][6][i], translate, colors[j],stride)
            if shapename[j]=="cylinder":
                plot_elliptical_cylinder(ax,allmuscledata[j][0][i], allmuscledata[j][1][i], allmuscledata[j][2][i], axis, allmuscledata[j][6][i], translate, colors[j],stride)
        for j in range(len(gammaarray)):
            gammaj=gammaarray[j][:,i].reshape(-1,3)
            ax.plot(gammaj[:,0], gammaj[:,1], gammaj[:,2], marker='o', linestyle='-')

        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        ax.set_xlim(-1.0*scale, 1.0*scale) #x,y,z limit
        ax.set_ylim(-1.0*scale, 1.0*scale)
        ax.set_zlim(-1.0*scale, 1.0*scale)
        ax.axis('off')  #not show  axis
        ax.view_init(elev=0, azim=-70) #set view  axis
        plt.savefig(figurename)
        plt.close(fig)
        
def main(modelname,scale=1.0):
    prefix = modelname+'_'
    types = ['length', 'forcenode', 'phi', 'totalforce', 'bodystate']

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
                allmuscledata=np.array(allmuscledata)
                normalfig(allmuscledata[:,:,1:]-allmuscledata[:,:,:-1], muscles, angles[1:], 'deformation diff', 'length_diff')
            elif 'phi' in filename:
                for i in range(len(allmuscledata)):
                    pcolorfig(angles, nodenum, np.array(allmuscledata[i]), 'node number', muscles[i], 'phi_')
                penatrationplot(allmuscledata, angles, nodenum, muscles)
            elif 'bodystate' in filename:
                if not os.path.exists("animation"):
                    os.makedirs("animation")
                shapename=getshapename(filename)
                
                filenamegamma=prefix + 'muscle_gamma_result.txt'
                with open(filenamegamma, 'r') as file:
                    linesgamma = file.readlines()
                gammamuscles, gammaangles, gammanodenum = extractxyaxis(linesgamma, 1)
                allmuscledatagamma = arrangetolist(linesgamma, gammamuscles, filenamegamma, 1)
                bodyrotateplot(allmuscledata, allmuscledatagamma, shapename,"animation", scale)
                fig = plt.figure()
                ims = []
                for i in range(len(allmuscledata[0][0])):
                    image = Image.open(f"animation/bodyfigure{i}.png")
                    ims.append([plt.imshow(image)])
                    plt.axis('off')
                ani = animation.ArtistAnimation(fig, ims, interval=100, blit=True, repeat_delay=1000)
                ani.save('animation.gif', writer='pillow', fps=10) #save gif
                #Writer = animation.writers['ffmpeg']
                #writer = Writer(fps=10, metadata=dict(artist='Me'), bitrate=1800)
                #ani.save('animation.mp4', writer=writer)
                plt.close(fig)
    
        
if __name__ == "__main__":
    argv=sys.argv
    if len(argv)<3:
        main(argv[1])
    else:
        main(argv[1],float(argv[2]))
    
                
