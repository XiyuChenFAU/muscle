import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.colors import LogNorm
from mpl_toolkits.mplot3d import Axes3D
import os
import sys
from PIL import Image
from matplotlib.ticker import MultipleLocator, AutoLocator

# color palette preserved from original script
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


class DataLoader:
    """All file I/O and light parsing."""
    def __init__(self, modelname):
        self.modelname = modelname

    def _read_lines(self, filename):
        with open(filename, 'r') as f:
            return f.readlines()

    def load_table_skip_header(self, filename):
        lines = self._read_lines(filename)
        return lines[1:]

    def load_body_result(self):
        return self._read_lines(f"{self.modelname}_body_result.txt")
        #return self.load_table_skip_header(f"{self.modelname}_body_result.txt")

    def load_bodystate(self):
        return self.load_table_skip_header(f"{self.modelname}_bodystate_result.txt")

    def load_type(self, typename):
        return self._read_lines(f"{self.modelname}_{typename}_result.txt")


class Geometry:
    @staticmethod
    def rotate_vector(vector, axis, angle_deg):
        """Rotate `vector` around `axis` by `angle_deg` degrees using Rodrigues' formula.
        axis is array-like length 3.
        """
        axis = np.asarray(axis, dtype=float)
        if np.linalg.norm(axis) == 0:
            return vector
        axis = axis / np.linalg.norm(axis)
        angle = np.deg2rad(angle_deg)
        c = np.cos(angle)
        s = np.sin(angle)
        x, y, z = axis
        R = np.array([
            [c + x*x*(1-c),     x*y*(1-c)-z*s, x*z*(1-c)+y*s],
            [y*x*(1-c)+z*s, c + y*y*(1-c),     y*z*(1-c)-x*s],
            [z*x*(1-c)-y*s, z*y*(1-c)+x*s, c + z*z*(1-c)]
        ])
        return R.dot(vector)


class Plot3D:
    def __init__(self, scale=1.0, viewangle=0.0):
        self.scale = scale
        self.viewangle = viewangle

    def plot_ellipsoid(self, ax, a, b, c, axis, rotationangle, translate, colorval, stride=2):
        u = np.linspace(0, 2 * np.pi, 40)
        v = np.linspace(0, np.pi, 40)
        x = a * np.outer(np.cos(u), np.sin(v))
        y = b * np.outer(np.sin(u), np.sin(v))
        z = c * np.outer(np.ones_like(u), np.cos(v))

        # rotate each point
        for i in range(x.shape[0]):
            for j in range(x.shape[1]):
                vec = np.array([x[i, j], y[i, j], z[i, j]])
                rv = Geometry.rotate_vector(vec, axis, rotationangle)
                x[i, j], y[i, j], z[i, j] = rv

        x += translate[0]
        y += translate[1]
        z += translate[2]
        ax.plot_wireframe(x, y, z, linewidth=0.3, color=colorval, rstride=1, cstride=stride)

    def plot_torus(self, ax, a, b, c, axis, rotationangle, translate, colorval, stride=2):
        u = np.linspace(0, 2 * np.pi, 40)
        v = np.linspace(0, 2 * np.pi, 40)  # Changed from np.pi to 2*np.pi for torus
        U, V = np.meshgrid(u, v)
        
        # Torus parameterization
        x = (a + b * np.cos(V)) * np.cos(U)
        y = (a + b * np.cos(V)) * np.sin(U)
        z = b * np.sin(V)

        # Reshape to 2D arrays if they're 1D
        if x.ndim == 1:
            x = x.reshape(len(u), len(v))
        if y.ndim == 1:
            y = y.reshape(len(u), len(v))
        if z.ndim == 1:
            z = z.reshape(len(u), len(v))

        # rotate each point
        for i in range(x.shape[0]):
            for j in range(x.shape[1]):
                vec = np.array([x[i, j], y[i, j], z[i, j]])
                rv = Geometry.rotate_vector(vec, axis, rotationangle)
                x[i, j], y[i, j], z[i, j] = rv

        x += translate[0]
        y += translate[1]
        z += translate[2]
        ax.plot_wireframe(x, y, z, linewidth=0.3, color=colorval, rstride=stride, cstride=stride)

    def plot_elliptical_cylinder(self, ax, a, b, c, axis, rotationangle, translate, colorval, stride=2):
        u = np.linspace(0, 2 * np.pi, 40)
        v = np.linspace(-c, c, 40)
        x = a * np.outer(np.cos(u), np.ones_like(v))
        y = b * np.outer(np.sin(u), np.ones_like(v))
        z = np.outer(np.ones_like(u), v)

        for i in range(x.shape[0]):
            for j in range(x.shape[1]):
                vec = np.array([x[i, j], y[i, j], z[i, j]])
                rv = Geometry.rotate_vector(vec, axis, rotationangle)
                x[i, j], y[i, j], z[i, j] = rv

        x += translate[0]
        y += translate[1]
        z += translate[2]
        ax.plot_wireframe(x, y, z, linewidth=0.3, color=colorval, rstride=1, cstride=stride)

    def configure_axes_limits(self, ax):
        ax.set_xlim(-1.0 * self.scale, 1.0 * self.scale)
        ax.set_ylim(-1.0 * self.scale, 1.0 * self.scale)
        ax.set_zlim(-1.0 * self.scale, 1.0 * self.scale)
        ax.axis('off')

    def configure_axes_limits(self, ax, bodydata=None, padding=0.05):
        if bodydata is not None and len(bodydata) > 0:
            # 计算每个方向的最小最大值
            bodydata = np.array(bodydata)
            min_vals = bodydata.min(axis=0)
            max_vals = bodydata.max(axis=0)

            # 添加padding
            ranges = max_vals - min_vals
            min_vals -= ranges * padding
            max_vals += ranges * padding

            # 保证三个方向范围相同
            max_range = max(max_vals - min_vals)
            center = (min_vals + max_vals) / 2
            ax.set_xlim(center[0] - max_range/2, center[0] + max_range/2)
            ax.set_ylim(center[1] - max_range/2, center[1] + max_range/2)
            ax.set_zlim(center[2] - max_range/2, center[2] + max_range/2)
        else:
            # fallback
            ax.set_xlim(-1.0 * self.scale, 1.0 * self.scale)
            ax.set_ylim(-1.0 * self.scale, 1.0 * self.scale)
            ax.set_zlim(-1.0 * self.scale, 1.0 * self.scale)

        ax.axis('off')

    def set_view(self, ax, elev=0, azim=None):
        if azim is None:
            azim = self.viewangle
        ax.view_init(elev=elev, azim=azim)


class Plot2D:
    @staticmethod
    def pcolorfig(x, y, data2d, ylabelname, title, prefixname):
        fig = plt.figure()
        plt.pcolormesh(x, y, data2d, cmap='viridis')
        plt.colorbar()
        plt.gca().xaxis.set_major_locator(MultipleLocator(120))
        plt.yticks(y)
        plt.xlabel('angle')
        plt.ylabel(ylabelname)
        plt.title(prefixname + title)
        figname = prefixname + title + '.png'
        plt.savefig(figname)
        plt.close(fig)

    @staticmethod
    def normalfig(data, linename, x, ylabelname, title):
        fig = plt.figure()
        allmusclname = []
        for i in range(len(data)):
            if 'length' in title:
                summed = np.sum(np.array(data[i]), axis=0)
                plt.plot(x, summed, label=linename[i], color=colors[i % len(colors)])
                if title == 'length':
                    plt.plot(x[1:], summed[1:] - summed[:-1], linestyle='--', label=linename[i] + " difference", color=colors[i % len(colors)])
            else:
                if title == 'moment arm':
                    if linename[i].split("-")[0] in allmusclname:
                        idx = allmusclname.index(linename[i].split("-")[0])
                        plt.plot(x, data[i][0], label=linename[i], color=colors[idx % len(colors)])
                    else:
                        allmusclname.append(linename[i].split("-")[0])
                        plt.plot(x, data[i][0], label=linename[i], color=colors[i % len(colors)])
                else:
                    plt.plot(x, data[i], label=linename[i], color=colors[i % len(colors)])

        plt.gca().xaxis.set_major_locator(MultipleLocator(120))
        plt.gca().xaxis.set_minor_locator(AutoLocator())
        plt.xlabel('angle')
        plt.ylabel(ylabelname)
        plt.legend()
        figurename = title + '.png'
        plt.savefig(figurename)
        plt.close(fig)


class Postprocessor:
    """High-level orchestration: reads files, processes data and creates figures/animations.
    This class preserves the behaviour of your original script while being modular.
    """
    def __init__(self, modelname, scale=1.0, viewangle=0.0, axisplot=False):
        self.modelname = modelname
        self.scale = float(scale)
        self.viewangle = float(viewangle)
        self.axisplot = bool(axisplot)
        self.loader = DataLoader(modelname)
        self.plot3d = Plot3D(scale=self.scale, viewangle=self.viewangle)

        # body values (for drawing axis quivers)
        body_lines = self.loader.load_body_result()
        self.bodyvalue = self._parse_body_values(body_lines)

    # ------------------- parsing helpers (kept behaviour) -------------------
    @staticmethod
    def _parse_body_values(lines):
        # original reads columns starting at 4th (index 3)
        t = 0
        bodyvalue = []
        for i in lines:
            if t == 0:
                t = 1
            else:
                parts = i.strip().split('\t')
                rowvals = [float(x) for x in parts[3:]]
                bodyvalue.append(rowvals)
        return np.array(bodyvalue) if len(bodyvalue) else np.empty((0, 0))

    @staticmethod
    def extractxyaxis(lines, initial):
        lock = 0
        muscles = []
        angles = []
        nodenum = []
        for i, line in enumerate(lines):
            row = line.strip().split('\t')
            if (row[0] not in muscles) and (row[0] != 'rotation_angle'):
                muscles.append(row[0])
                lock = 1
            if lock == 1:
                if row[0] == muscles[0]:
                    nodenum.append(row[2])
            if i == 0:
                for j in range(len(row)):
                    if initial == 1 and j > 3:
                        angles.append(float(row[j]))
                    elif initial == 0 and j > 2:
                        angles.append(float(row[j]))
        return muscles, angles, nodenum

    @staticmethod
    def arrangetolist(lines, muscles, filename, inital):
        allmuscledata = []
        if 'totalforce' not in filename:
            for m in muscles:
                muscledata = []
                for line in lines:
                    row = line.strip().split('\t')
                    if row[0] == m:
                        rowdata = [float(row[j + 3 + inital]) for j in range(len(row) - 3 - inital)]
                        muscledata.append(rowdata)
                allmuscledata.append(muscledata)
        else:
            for line in lines:
                row = line.strip().split('\t')
                if row[0] != 'rotation_angle':
                    rowdata = [float(row[j + 3 + inital]) for j in range(len(row) - 3 - inital)]
                    allmuscledata.append(rowdata)
        return allmuscledata

    def getshapename(self, filename):
        lines = self.loader._read_lines(filename)
        bodyname = []
        shapename = []
        for i, l in enumerate(lines):
            if i == 0:
                continue
            parts = l.strip().split('\t')
            if parts[0] not in bodyname:
                bodyname.append(parts[0])
                shapename.append(parts[1])
        return shapename

    # ------------------- plotting helpers -------------------
    def axisplotfig(self, ax, index):
        # index is column index
        if self.bodyvalue.size == 0:
            return
        n = int(self.bodyvalue.shape[0] / 12)
        for i in range(n):
            base = 12 * i
            ax.quiver(self.bodyvalue[base][index], self.bodyvalue[base + 1][index], self.bodyvalue[base + 2][index],
                      self.bodyvalue[base + 3][index], self.bodyvalue[base + 4][index], self.bodyvalue[base + 5][index],
                      length=0.1, normalize=True, color=colors[0])
            ax.quiver(self.bodyvalue[base][index], self.bodyvalue[base + 1][index], self.bodyvalue[base + 2][index],
                      self.bodyvalue[base + 6][index], self.bodyvalue[base + 7][index], self.bodyvalue[base + 8][index],
                      length=0.1, normalize=True, color=colors[1])
            ax.quiver(self.bodyvalue[base][index], self.bodyvalue[base + 1][index], self.bodyvalue[base + 2][index],
                      self.bodyvalue[base + 9][index], self.bodyvalue[base + 10][index], self.bodyvalue[base + 11][index],
                      length=0.1, normalize=True, color=colors[2])

    def gatherdataandplot(self, data, another):
        allpenatration = []
        for m in range(len(data[0])):
            oneline = []
            for n in range(len(data[0][0])):
                values = [data[p][m][n] for p in range(len(another))]
                penatration = np.min(values) if np.min(values) <= 0 else 0
                oneline.append(penatration)
            allpenatration.append(oneline)
        return allpenatration

    def penatrationplot(self, data, x, y, musclepair):
        muscles = []
        tissues = []
        for item in musclepair:
            name = item.strip().split('-')
            if name[0] not in muscles:
                muscles.append(name[0])
            if name[1] not in tissues:
                tissues.append(name[1])

        for i in range(len(muscles)):
            onemuscledata = [data[j] for j in range(len(musclepair)) if muscles[i] in musclepair[j]]
            allpenatration = self.gatherdataandplot(onemuscledata, tissues)
            Plot2D.pcolorfig(x, y, np.array(allpenatration), 'node number', muscles[i], 'penetration_')

        for i in range(len(tissues)):
            onetissuedata = [data[j] for j in range(len(musclepair)) if tissues[i] in musclepair[j]]
            allpenatration = self.gatherdataandplot(onetissuedata, muscles)
            Plot2D.pcolorfig(x, y, np.array(allpenatration), 'node number', tissues[i], 'penetration_')

    # ------------------- body & animation -------------------
    def bodyrotateplot(self, allmuscledata, allmuscledatagamma, shapename, foldername, viewangle):
        gammaarray = np.array(allmuscledatagamma)
        stride = 2
        os.makedirs(foldername, exist_ok=True)

        for i in range(len(allmuscledata[0][0])):
            figurename = os.path.join(foldername, f'bodyfigure{i}.png')
            fig = plt.figure()
            ax = fig.add_subplot(111, projection='3d')

            for j in range(len(allmuscledata)):
                axis = [allmuscledata[j][3][i], allmuscledata[j][4][i], allmuscledata[j][5][i]]
                translate = [allmuscledata[j][7][i], allmuscledata[j][8][i], allmuscledata[j][9][i]]
                if shapename[j] == "ellipsoid":
                    self.plot3d.plot_ellipsoid(ax, allmuscledata[j][0][i], allmuscledata[j][1][i], allmuscledata[j][2][i],
                                               axis, allmuscledata[j][6][i], translate, colors[(len(colors) - 1 - j) % len(colors)], stride)
                if shapename[j] == "cylinder":
                    self.plot3d.plot_elliptical_cylinder(ax, allmuscledata[j][0][i], allmuscledata[j][1][i], allmuscledata[j][2][i],
                                                         axis, allmuscledata[j][6][i], translate, colors[(len(colors) - 1 - j) % len(colors)], stride)
                if shapename[j] == "torus":
                    self.plot3d.plot_torus(ax, allmuscledata[j][0][i], allmuscledata[j][1][i], allmuscledata[j][2][i],
                                          axis, allmuscledata[j][6][i], translate, colors[(len(colors) - 1 - j) % len(colors)], stride)

            if self.axisplot:
                self.axisplotfig(ax, i + 1)

            # plot gamma curves
            for j in range(len(gammaarray)):
                gammaj = gammaarray[j][:, i].reshape(-1, 3)
                ax.plot(gammaj[:, 0], gammaj[:, 1], gammaj[:, 2], linewidth=2.8, linestyle='-', color=colors[j % len(colors)])
                ax.scatter(gammaj[0, 0], gammaj[0, 1], gammaj[0, 2], color=colors[j % len(colors)], marker='o', s=30)
                ax.scatter(gammaj[-1, 0], gammaj[-1, 1], gammaj[-1, 2], color=colors[j % len(colors)], marker='o', s=30)

            ax.set_xlabel('X')
            ax.set_ylabel('Y')
            ax.set_zlabel('Z')
            translations = []
            """
            for j in range(len(allmuscledata)):
                translate = [allmuscledata[j][7][i], allmuscledata[j][8][i], allmuscledata[j][9][i]]
                translations.append(translate)

            self.plot3d.configure_axes_limits(ax, bodydata=translations, padding=0.1)
            """

            self.plot3d.configure_axes_limits(ax)
            self.plot3d.set_view(ax, elev=0, azim=viewangle)
            plt.savefig(figurename)
            plt.close(fig)

    # ------------------- main pipeline -------------------
    def checkbody(self):
        lines = self.loader.load_bodystate()
        statename = []
        statevalue = []
        shapename = []
        for l in lines:
            parts = l.strip().split('\t')
            if parts[0] not in statename:
                statename.append(parts[0])
                statevalue.append([])
                shapename.append(parts[1])
            statevalue[-1].append(float(parts[3]))

        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        stride = 2
        for i in range(len(statename)):
            vals = statevalue[i]
            if shapename[i] == "ellipsoid":
                self.plot3d.plot_ellipsoid(ax, vals[0], vals[1], vals[2], [vals[3], vals[4], vals[5]], vals[6], [vals[7], vals[8], vals[9]], colors[(len(colors) - 1 - i) % len(colors)], stride)
            if shapename[i] == "cylinder":
                self.plot3d.plot_elliptical_cylinder(ax, vals[0], vals[1], vals[2], [vals[3], vals[4], vals[5]], vals[6], [vals[7], vals[8], vals[9]], colors[(len(colors) - 1 - i) % len(colors)], stride)
            if shapename[i] == "torus":
                self.plot3d.plot_torus(ax, vals[0], vals[1], vals[2], [vals[3], vals[4], vals[5]], vals[6], [vals[7], vals[8], vals[9]], colors[(len(colors) - 1 - i) % len(colors)], stride)
            self.axisplotfig(ax, 0)

        self.plot3d.configure_axes_limits(ax)
        self.plot3d.set_view(ax, elev=0, azim=self.viewangle)
        plt.savefig('body_initial.png')
        plt.close(fig)

    def postprocessingresult(self):
        self.checkbody()
        prefix = f"{self.modelname}_"
        types = ['length', 'forcenode', 'phi', 'totalforce', 'bodystate', 'momentarm']

        for typ in types:
            filename = prefix + typ + '_result.txt'
            if not os.path.exists(filename):
                # preserve behaviour: skip silently if missing
                continue
            with open(filename, 'r') as f:
                lines = f.readlines()

            if ('forcenode' in typ) or ('totalforce' in typ):
                muscles, angles, nodenum = self.extractxyaxis(lines, 0)
                allmuscledata = self.arrangetolist(lines, muscles, filename, 0)

                if 'forcenode' in typ:
                    for i in range(len(allmuscledata)):
                        Plot2D.pcolorfig(angles, nodenum, np.array(allmuscledata[i]), 'node number', muscles[i], 'forcenode_')
                elif 'totalforce' in typ:
                    Plot2D.normalfig(allmuscledata, muscles, angles, 'total force', 'totalforce')
            else:
                muscles, angles, nodenum = self.extractxyaxis(lines, 1)
                allmuscledata = self.arrangetolist(lines, muscles, filename, 1)

                if 'length' in typ:
                    Plot2D.normalfig(allmuscledata, muscles, angles, 'total length', 'total length')
                    arr = np.array(allmuscledata)
                    Plot2D.normalfig((arr[:, :, 1:] - arr[:, :, :-1]) / (angles[1] - angles[0]), muscles, angles[1:], 'muscle length rate', 'muscle length rate')
                elif 'momentarm' in typ:
                    Plot2D.normalfig(allmuscledata, muscles, angles, 'moment arm', 'moment arm')
                elif 'phi' in typ:
                    for i in range(len(allmuscledata)):
                        Plot2D.pcolorfig(angles, nodenum, np.array(allmuscledata[i]), 'node number', muscles[i], 'phi_')
                    self.penatrationplot(allmuscledata, angles, nodenum, muscles)
                elif 'bodystate' in typ:
                    os.makedirs('animation', exist_ok=True)
                    shapename = self.getshapename(filename)

                    filenamegamma = prefix + 'muscle_gamma_result.txt'
                    if not os.path.exists(filenamegamma):
                        continue
                    with open(filenamegamma, 'r') as f:
                        linesgamma = f.readlines()

                    gammamuscles, gammaangles, gammanodenum = self.extractxyaxis(linesgamma, 1)
                    allmuscledatagamma = self.arrangetolist(linesgamma, gammamuscles, filenamegamma, 1)

                    # front view
                    os.makedirs('animation_front', exist_ok=True)
                    self.bodyrotateplot(allmuscledata, allmuscledatagamma, shapename, 'animation_front', self.viewangle + 90)
                    self._make_gif_from_folder('animation_front', 'animation_front.gif')

                    # side view
                    os.makedirs('animation_side', exist_ok=True)
                    self.bodyrotateplot(allmuscledata, allmuscledatagamma, shapename, 'animation_side', self.viewangle)
                    self._make_gif_from_folder('animation_side', 'animation_side.gif')

    def _make_gif_from_folder(self, folder, outname):
        # Creates a gif from bodyfigure*.png inside folder (keeps original behaviour)
        files = sorted([f for f in os.listdir(folder) if f.startswith('bodyfigure') and f.endswith('.png')], key=lambda x: int(''.join(filter(str.isdigit, x)) or -1))
        if not files:
            return
        fig = plt.figure()
        ims = []
        for fname in files:
            image = Image.open(os.path.join(folder, fname))
            ims.append([plt.imshow(image)])
            plt.axis('off')
        ani = animation.ArtistAnimation(fig, ims, interval=100, blit=True, repeat_delay=1000)
        ani.save(outname, writer='pillow', fps=10)
        plt.close(fig)


def parse_cli(argv):
    if '-h' in argv or len(argv) == 1:
        helpmsg = (
            "Usage: -model <modelname> [-scale <float>] [-viewangle <float>] [-axisplot] [-onlyinitbody]\n"
            "Example: python postprocess_refactored.py -model myModel -scale 1.0 -viewangle 0 -axisplot"
        )
        print(helpmsg)
        sys.exit(0)

    scale = 1.0
    viewangle = 0.0
    axisplot = False
    onlyinit = False
    modelname = None

    if '-scale' in argv:
        scale = float(argv[argv.index('-scale') + 1])
    if '-viewangle' in argv:
        viewangle = float(argv[argv.index('-viewangle') + 1])
    if '-axisplot' in argv:
        axisplot = True
    if '-onlyinitbody' in argv:
        onlyinit = True
    if '-model' in argv:
        modelname = argv[argv.index('-model') + 1]

    if modelname is None:
        print('Error: -model <modelname> required')
        sys.exit(1)

    return modelname, scale, viewangle, axisplot, onlyinit


if __name__ == '__main__':
    modelname, scale, viewangle, axisplot, onlyinit = parse_cli(sys.argv)
    p = Postprocessor(modelname, scale=scale, viewangle=viewangle, axisplot=axisplot)
    if onlyinit:
        p.checkbody()
    else:
        p.postprocessingresult()
