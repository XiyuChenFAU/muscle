import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.colors import LogNorm
from mpl_toolkits.mplot3d import Axes3D
import os
import random
import sys
from PIL import Image
from matplotlib.ticker import MultipleLocator, AutoLocator

colors = [
    'r', 'g', 'b', 'm', 'aqua', 'black', 'brown', 'burlywood',
    'cadetblue','chartreuse', 'y', 'k', 'w', 'c',
    'chocolate', 'coral', 'cornflowerblue', 'crimson', 'cyan',
    'darkblue', 'darkcyan', 'darkgoldenrod', 'darkgray', 'darkgreen',
    'darkgrey', 'darkkhaki', 'darkmagenta', 'darkolivegreen', 'darkorange',
    'darkorchid', 'darkred', 'darksalmon', 'darkseagreen', 'darkslateblue',
    'darkslategray', 'darkslategrey', 'darkturquoise', 'darkviolet', 'deeppink',
    'deepskyblue', 'dimgray', 'dimgrey', 'dodgerblue', 'firebrick', 'forestgreen'
]

class Postprocess:
    def __init__(self,modelname,scale,viewangle,axisplot):
        self.modelname=modelname
        self.scale=scale
        self.viewangle=viewangle
        self.axisplot=axisplot
        filenamebody = self.modelname +'_body_result.txt'
        file2=open(filenamebody, 'r')
        t,bodyvalue=0,[]
        for i in file2:
            if t==0:
                t=1
            else:
                line=i.strip().split("\t")
                bodyvalue1=[]
                for j in range(len(line)-3):
                    bodyvalue1.append(float(line[j+3]))
                bodyvalue.append(bodyvalue1)
        self.bodyvalue=np.array(bodyvalue)
        file2.close()
        
    def rotate_vector(self,vector, axis, angle):
        # Normalize the axis vector
        axis = axis / np.linalg.norm(axis)

        # Calculate the rotation matrix
        cos_theta = np.cos(angle)
        sin_theta = np.sin(angle)
        
        # Compute the rotation matrix
        rotation_matrix = np.array([
            [cos_theta + axis[0]**2 * (1 - cos_theta),
             axis[0] * axis[1] * (1 - cos_theta) - axis[2] * sin_theta,
             axis[0] * axis[2] * (1 - cos_theta) + axis[1] * sin_theta],
            
            [axis[1] * axis[0] * (1 - cos_theta) + axis[2] * sin_theta,
             cos_theta + axis[1]**2 * (1 - cos_theta),
             axis[1] * axis[2] * (1 - cos_theta) - axis[0] * sin_theta],
            
            [axis[2] * axis[0] * (1 - cos_theta) - axis[1] * sin_theta,
             axis[2] * axis[1] * (1 - cos_theta) + axis[0] * sin_theta,
             cos_theta + axis[2]**2 * (1 - cos_theta)]
        ])
        # Apply the rotation to the vector
        rotated_vector = np.dot(rotation_matrix, vector)

        return rotated_vector

    def plot_ellipsoid(self, ax, a, b, c, axis, rotationangle, translate, colorval, stride):
        u = np.linspace(0, 2 * np.pi, 40)
        v = np.linspace(0, np.pi, 40)
        x = a * np.outer(np.cos(u), np.sin(v))
        y = b * np.outer(np.sin(u), np.sin(v))
        z = c * np.outer(np.ones_like(u), np.cos(v))

        # Perform rotation around the specified axis
        for i in range(len(x)):
            for j in range(len(x[i])):
                vector = np.array([x[i][j], y[i][j], z[i][j]])
                rotated_vector = self.rotate_vector(vector, axis, rotationangle/180*np.pi)
                x[i][j], y[i][j], z[i][j] = rotated_vector

        x += translate[0]
        y += translate[1]
        z += translate[2]
        
        ax.plot_wireframe(x, y, z, linewidth=0.3, color=colorval, cstride=stride)
        #ax.plot_surface(x, y, z, color=colorval) #suface

    def plot_elliptical_cylinder(self, ax, a, b, c, axis, rotationangle, translate, colorval,stride):
        # Generate points for the elliptical cylinder
        u = np.linspace(0, 2 * np.pi, 40)
        v = np.linspace(-c, c, 40)
        x = a * np.outer(np.cos(u), np.ones_like(v))
        y = b * np.outer(np.sin(u), np.ones_like(v))
        z = np.outer(np.ones_like(u), v)

        # Perform rotation around the specified axis
        for i in range(len(x)):
            for j in range(len(x[i])):
                vector = np.array([x[i][j], y[i][j], z[i][j]])
                rotated_vector = self.rotate_vector(vector, axis, rotationangle/180*np.pi)
                x[i][j], y[i][j], z[i][j] = rotated_vector

        x += translate[0]
        y += translate[1]
        z += translate[2]
        ax.plot_wireframe(x, y, z, linewidth=0.3, color=colorval, cstride=stride)
        #ax.plot_surface(x, y, z, color=colorval) #suface


    # extract first line and first row from excel
    def extractxyaxis(self,lines, initial):
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
    def arrangetolist(self, lines, muscles, filename, inital):
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
        
    def getshapename(self,filename):
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

    def pcolorfig(self, x, y, data2d, ylabelname, title, prefixname):
        fig = plt.figure()
        plt.pcolormesh(x, y, data2d, cmap='viridis')
        plt.colorbar()
        plt.gca().xaxis.set_major_locator(MultipleLocator(10))
        plt.yticks(y)
        plt.xlabel('angle')
        plt.ylabel(ylabelname)
        plt.title(prefixname +title)
        figname = prefixname + title + '.png'
        plt.savefig(figname)
        plt.close(fig)

    def normalfig(self, data, linename, x, ylabelname, title):
        fig=plt.figure()
        for i in range(len(data)):
            if 'length' in title:
                sum = np.sum(np.array(data[i]), axis=0)
                plt.plot(x, sum, label=linename[i],color=colors[i%len(colors)])
                if 'length' == title:
                    plt.plot(x[1:], sum[1:]-sum[:-1], linestyle='--', label=linename[i]+" difference",color=colors[i%len(colors)])
            else:
                plt.plot(x, data[i], label=linename[i],color=colors[i%len(colors)])
        plt.gca().xaxis.set_major_locator(MultipleLocator(10))
        plt.gca().xaxis.set_minor_locator(AutoLocator())
        plt.xlabel('angle')
        plt.ylabel(ylabelname)
        plt.legend()
        figurename = title + '.png'
        plt.savefig(figurename)
        plt.close(fig)

    def gatherdataandplot(self, data, another):
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
        

    def penatrationplot(self, data, x, y, musclepair):
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
            allpenatration = self.gatherdataandplot(onemuscledata, tissues)
            self.pcolorfig(x, y, allpenatration, 'node number', muscles[i], 'penetration_')

        # figure according to tissue type
        for i in range(len(tissues)):
            onetissuedata = []
            for j in range(len(musclepair)):
                if tissues[i] in musclepair[j]:
                    onetissuedata.append(data[j])
            allpenatration = self.gatherdataandplot(onetissuedata, muscles)
            self.pcolorfig(x, y, allpenatration, 'node number', tissues[i], 'penetration_')
            
    def bodyrotateplot(self, allmuscledata, allmuscledatagamma, shapename,foldername,viewangle):
        gammaarray=np.array(allmuscledatagamma)
        stride = 50  # Adjust the stride value to control the sparsity of lines

        for i in range(len(allmuscledata[0][0])):
            figurename=foldername+'/bodyfigure'+str(i)
            fig = plt.figure()
            ax = fig.add_subplot(111, projection='3d')
            for j in range(len(allmuscledata)):
                axis=[allmuscledata[j][3][i],allmuscledata[j][4][i],allmuscledata[j][5][i]]
                translate=[allmuscledata[j][7][i],allmuscledata[j][8][i],allmuscledata[j][9][i]]
                if shapename[j]=="ellipsoid":
                    self.plot_ellipsoid(ax,allmuscledata[j][0][i], allmuscledata[j][1][i], allmuscledata[j][2][i], axis, allmuscledata[j][6][i], translate, colors[len(colors)-1-j%len(colors)],stride)
                if shapename[j]=="cylinder":
                    self.plot_elliptical_cylinder(ax,allmuscledata[j][0][i], allmuscledata[j][1][i], allmuscledata[j][2][i], axis, allmuscledata[j][6][i], translate, colors[len(colors)-1-j%len(colors)],stride)
            if self.axisplot:
                self.axisplotfig(ax,i+1)
            for j in range(len(gammaarray)):
                gammaj=gammaarray[j][:,i].reshape(-1,3)
                ax.plot(gammaj[:,0], gammaj[:,1], gammaj[:,2], linewidth=2.8, linestyle='-', color=colors[j%len(colors)])

            ax.set_xlabel('X')
            ax.set_ylabel('Y')
            ax.set_zlabel('Z')
            ax.set_xlim(-1.0*scale, 1.0*scale) #x,y,z limit
            ax.set_ylim(-1.0*scale, 1.0*scale)
            ax.set_zlim(-1.0*scale, 1.0*scale)
            ax.axis('off')  #not show  axis
            ax.view_init(elev=0, azim=viewangle) #set view  axis
            plt.savefig(figurename)
            plt.close(fig)
            
    def postprocessingresult(self):
        self.checkbody()
        prefix = self.modelname+'_'
        types = ['length', 'forcenode', 'phi', 'totalforce', 'bodystate']
        

        for i in range(len(types)):
            filename = prefix + types[i] +'_result.txt'
            with open(filename, 'r') as file:
                lines = file.readlines()
            
            if ('forcenode' in filename.split("_")[-2]) or ('totalforce' in filename.split("_")[-2]):
                muscles, angles, nodenum = self.extractxyaxis(lines, 0)
                allmuscledata = self.arrangetolist(lines, muscles, filename, 0)

                # plot
                if 'forcenode' in filename.split("_")[-2]:
                    for i in range(len(allmuscledata)):
                        self.pcolorfig(angles, nodenum, np.array(allmuscledata[i]), 'node number', muscles[i], 'forcenode_')
                elif 'totalforce' in filename.split("_")[-2]:
                    self.normalfig(allmuscledata, muscles, angles, 'total force', 'totalforce')
            else:
                muscles, angles, nodenum = self.extractxyaxis(lines, 1)
                allmuscledata = self.arrangetolist(lines, muscles, filename, 1)

                # plot
                if 'length' in filename.split("_")[-2]:
                    self.normalfig(allmuscledata, muscles, angles, 'total length', 'total length')
                    allmuscledata=np.array(allmuscledata)
                    self.normalfig(allmuscledata[:,:,1:]-allmuscledata[:,:,:-1], muscles, angles[1:], 'total length change', 'total length change')
                elif 'phi' in filename.split("_")[-2]:
                    for i in range(len(allmuscledata)):
                        self.pcolorfig(angles, nodenum, np.array(allmuscledata[i]), 'node number', muscles[i], 'phi_')
                    self.penatrationplot(allmuscledata, angles, nodenum, muscles)
                elif 'bodystate' in filename.split("_")[-2]:
                    if not os.path.exists("animation"):
                        os.makedirs("animation")
                    shapename=self.getshapename(filename)
                    
                    filenamegamma=prefix + 'muscle_gamma_result.txt'
                    with open(filenamegamma, 'r') as file:
                        linesgamma = file.readlines()
                    gammamuscles, gammaangles, gammanodenum = self.extractxyaxis(linesgamma, 1)
                    allmuscledatagamma = self.arrangetolist(linesgamma, gammamuscles, filenamegamma, 1)
                    
                    if not os.path.exists("animation_front"):
                        os.makedirs("animation_front")
                    self.bodyrotateplot(allmuscledata, allmuscledatagamma, shapename,"animation_front", 0)
                    fig = plt.figure()
                    ims = []
                    for i in range(len(allmuscledata[0][0])):
                        image = Image.open(f"animation_front/bodyfigure{i}.png")
                        ims.append([plt.imshow(image)])
                        plt.axis('off')
                    ani = animation.ArtistAnimation(fig, ims, interval=100, blit=True, repeat_delay=1000)
                    ani.save('animation_front.gif', writer='pillow', fps=10) #save gif
                    #Writer = animation.writers['ffmpeg']
                    #writer = Writer(fps=10, metadata=dict(artist='Me'), bitrate=1800)
                    #ani.save('animation.mp4', writer=writer)
                    plt.close(fig)
                    if not os.path.exists("animation_side"):
                        os.makedirs("animation_side")
                    self.bodyrotateplot(allmuscledata, allmuscledatagamma, shapename,"animation_side", -90)
                    fig = plt.figure()
                    ims = []
                    for i in range(len(allmuscledata[0][0])):
                        image = Image.open(f"animation_side/bodyfigure{i}.png")
                        ims.append([plt.imshow(image)])
                        plt.axis('off')
                    ani = animation.ArtistAnimation(fig, ims, interval=100, blit=True, repeat_delay=1000)
                    ani.save('animation_side.gif', writer='pillow', fps=10) #save gif
                    #Writer = animation.writers['ffmpeg']
                    #writer = Writer(fps=10, metadata=dict(artist='Me'), bitrate=1800)
                    #ani.save('animation.mp4', writer=writer)
                    plt.close(fig)
                    
    def axisplotfig(self,ax,index):
        for i in range(int(self.bodyvalue.shape[0]/12)):
            ax.quiver(self.bodyvalue[12*i][index], self.bodyvalue[12*i+1][index], self.bodyvalue[12*i+2][index], self.bodyvalue[12*i+3][index], self.bodyvalue[12*i+4][index], self.bodyvalue[12*i+5][index], length=0.1, normalize=True,color=colors[0])
            ax.quiver(self.bodyvalue[12*i][index], self.bodyvalue[12*i+1][index], self.bodyvalue[12*i+2][index], self.bodyvalue[12*i+6][index], self.bodyvalue[12*i+7][index], self.bodyvalue[12*i+8][index], length=0.1, normalize=True,color=colors[1])
            ax.quiver(self.bodyvalue[12*i][index], self.bodyvalue[12*i+1][index], self.bodyvalue[12*i+2][index], self.bodyvalue[12*i+9][index], self.bodyvalue[12*i+10][index], self.bodyvalue[12*i+11][index], length=0.1, normalize=True,color=colors[2])
            

    def checkbody(self):
        stride = 50  # Adjust the stride value to control the sparsity of lines

        filenamebodystate = self.modelname +'_bodystate_result.txt'
        file1=open(filenamebodystate, 'r')
        t,statename,statevalue,shapename=0,[],[],[]
        for i in file1:
            if t==0:
                t=1
            else:
                line=i.strip().split("\t")
                if not line[0] in statename:
                    statevalue.append([])
                    statename.append(line[0])
                    shapename.append(line[1])
                statevalue[-1].append(float(line[3]))
        file1.close()
            
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        for i in range(len(statename)):
            if shapename[i]=="ellipsoid":
                self.plot_ellipsoid(ax, statevalue[i][0], statevalue[i][1], statevalue[i][2], [statevalue[i][3],statevalue[i][4],statevalue[i][5]], statevalue[i][6], [statevalue[i][7],statevalue[i][8],statevalue[i][9]], colors[len(colors)-1-i%len(colors)], stride)
            if shapename[i]=="cylinder":
                self.plot_elliptical_cylinder(ax, statevalue[i][0], statevalue[i][1], statevalue[i][2], [statevalue[i][3],statevalue[i][4],statevalue[i][5]], statevalue[i][6], [statevalue[i][7],statevalue[i][8],statevalue[i][9]], colors[len(colors)-1-i%len(colors)], stride)
            self.axisplotfig(ax,0)
        figurename='body_initial.png'
        ax.set_xlim(-1.0*self.scale, 1.0*self.scale) #x,y,z limit
        ax.set_ylim(-1.0*self.scale, 1.0*self.scale)
        ax.set_zlim(-1.0*self.scale, 1.0*self.scale)
        ax.axis('off')  #not show  axis
        ax.view_init(elev=0, azim=self.viewangle) #set view  axis
        plt.savefig(figurename)
        plt.close(fig)
    
        
if __name__ == "__main__":
    argv=sys.argv
    if "-h" in argv:
        print("-model <modelname>: input model name ( must give model name)")
        print("-scale <float>: input the value to scale the animation and body initial figure (default 1.0)")
        print("-viewangle <float>: input the view angle for body initial figure (default 0.0)")
        print("-onlyinitbody: only show the body initial figure")

    else:
        if "-scale" in argv:
            scale=float(argv[argv.index("-scale")+1])
        else:
            scale=1.0
            
        if "-viewangle" in argv:
            viewangle=float(argv[argv.index("-viewangle")+1])
        else:
            viewangle=0.0
        
        if "-axisplot" in argv:
            axisplot=1
        else:
            axisplot=0
        
        if "-model" in argv:
            modelname=argv[argv.index("-model")+1]
            post=Postprocess(modelname,scale,viewangle,axisplot)
            if "-onlyinitbody" in argv:
                post.checkbody()
            else:
                post.postprocessingresult()
                
