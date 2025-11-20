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
from abc import ABC, abstractmethod

# Constants
COLORS = [
    'r', 'g', 'b', 'm', 'aqua', 'black', 'brown', 'burlywood',
    'cadetblue','chartreuse', 'y', 'k', 'w', 'c',
    'chocolate', 'coral', 'cornflowerblue', 'crimson', 'cyan',
    'darkblue', 'darkcyan', 'darkgoldenrod', 'darkgray', 'darkgreen',
    'darkgrey', 'darkkhaki', 'darkmagenta', 'darkolivegreen', 'darkorange',
    'darkorchid', 'darkred', 'darksalmon', 'darkseagreen', 'darkslateblue',
    'darkslategray', 'darkslategrey', 'darkturquoise', 'darkviolet', 'deeppink',
    'deepskyblue', 'dimgray', 'dimgrey', 'dodgerblue', 'firebrick', 'forestgreen'
]

class Geometry(ABC):
    """Abstract base class for geometric shapes"""
    
    @staticmethod
    def rotate_vector(vector, axis, angle):
        """Rotate a vector around an axis by given angle"""
        axis = axis / np.linalg.norm(axis)
        cos_theta = np.cos(angle)
        sin_theta = np.sin(angle)
        
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
        
        return np.dot(rotation_matrix, vector)

    @abstractmethod
    def plot(self, ax, color, stride):
        pass


class Ellipsoid(Geometry):
    """Ellipsoid geometry"""
    
    def __init__(self, a, b, c, axis, rotation_angle, translate):
        self.a = a
        self.b = b
        self.c = c
        self.axis = axis
        self.rotation_angle = rotation_angle
        self.translate = translate
    
    def plot(self, ax, color, stride):
        u = np.linspace(0, 2 * np.pi, 30)
        v = np.linspace(0, np.pi, 30)
        x = self.a * np.outer(np.cos(u), np.sin(v))
        y = self.b * np.outer(np.sin(u), np.sin(v))
        z = self.c * np.outer(np.ones_like(u), np.cos(v))

        for i in range(len(x)):
            for j in range(len(x[i])):
                vector = np.array([x[i][j], y[i][j], z[i][j]])
                rotated_vector = self.rotate_vector(vector, self.axis, self.rotation_angle/180*np.pi)
                x[i][j], y[i][j], z[i][j] = rotated_vector

        x += self.translate[0]
        y += self.translate[1]
        z += self.translate[2]
        
        ax.plot_wireframe(x, y, z, linewidth=0.3, color=color, cstride=stride)


class EllipticalCylinder(Geometry):
    """Elliptical cylinder geometry"""
    
    def __init__(self, a, b, c, axis, rotation_angle, translate):
        self.a = a
        self.b = b
        self.c = c
        self.axis = axis
        self.rotation_angle = rotation_angle
        self.translate = translate
    
    def plot(self, ax, color, stride):
        u = np.linspace(0, 2 * np.pi, 30)
        v = np.linspace(-self.c, self.c, 30)
        x = self.a * np.outer(np.cos(u), np.ones_like(v))
        y = self.b * np.outer(np.sin(u), np.ones_like(v))
        z = np.outer(np.ones_like(u), v)

        for i in range(len(x)):
            for j in range(len(x[i])):
                vector = np.array([x[i][j], y[i][j], z[i][j]])
                rotated_vector = self.rotate_vector(vector, self.axis, self.rotation_angle/180*np.pi)
                x[i][j], y[i][j], z[i][j] = rotated_vector

        x += self.translate[0]
        y += self.translate[1]
        z += self.translate[2]
        ax.plot_wireframe(x, y, z, linewidth=0.3, color=color, cstride=stride)


class BoundaryCalculator:
    """Calculate global boundaries for animation"""
    
    @staticmethod
    def calculate_global_boundaries(body_data, gamma_data, margin_ratio=0.1):
        """
        Calculate global min/max boundaries across all frames
        margin_ratio: additional margin as ratio of data range
        """
        all_x, all_y, all_z = [], [], []
        
        # Collect all body points
        for frame_idx in range(len(body_data[0][0])):
            for j, muscle_data in enumerate(body_data):
                # Get body parameters for this frame
                a, b, c = muscle_data[0][frame_idx], muscle_data[1][frame_idx], muscle_data[2][frame_idx]
                axis = [muscle_data[3][frame_idx], muscle_data[4][frame_idx], muscle_data[5][frame_idx]]
                rotation_angle = muscle_data[6][frame_idx]
                translate = [muscle_data[7][frame_idx], muscle_data[8][frame_idx], muscle_data[9][frame_idx]]
                
                # Generate sample points for this body
                body_points = BoundaryCalculator._generate_body_points(
                    a, b, c, axis, rotation_angle, translate
                )
                all_x.extend(body_points[:, 0])
                all_y.extend(body_points[:, 1])
                all_z.extend(body_points[:, 2])
        
        # Collect all gamma points
        gamma_array = np.array(gamma_data)
        for j, gamma_j in enumerate(gamma_array):
            for frame_idx in range(gamma_j.shape[1]):
                gamma_points = gamma_j[:, frame_idx].reshape(-1, 3)
                all_x.extend(gamma_points[:, 0])
                all_y.extend(gamma_points[:, 1])
                all_z.extend(gamma_points[:, 2])
        
        # Calculate boundaries with margin
        x_min, x_max = np.min(all_x), np.max(all_x)
        y_min, y_max = np.min(all_y), np.max(all_y)
        z_min, z_max = np.min(all_z), np.max(all_z)
        
        x_range = x_max - x_min
        y_range = y_max - y_min
        z_range = z_max - z_min
        
        # Add margin
        x_min -= x_range * margin_ratio
        x_max += x_range * margin_ratio
        y_min -= y_range * margin_ratio
        y_max += y_range * margin_ratio
        z_min -= z_range * margin_ratio
        z_max += z_range * margin_ratio
        
        return {
            'x': (x_min, x_max),
            'y': (y_min, y_max),
            'z': (z_min, z_max)
        }
    
    @staticmethod
    def _generate_body_points(a, b, c, axis, rotation_angle, translate, num_points=100):
        """Generate sample points for a body to estimate its boundaries"""
        # For ellipsoid
        u = np.linspace(0, 2 * np.pi, int(np.sqrt(num_points)))
        v = np.linspace(0, np.pi, int(np.sqrt(num_points)))
        x = a * np.outer(np.cos(u), np.sin(v))
        y = b * np.outer(np.sin(u), np.sin(v))
        z = c * np.outer(np.ones_like(u), np.cos(v))
        
        # Flatten and rotate
        points = np.column_stack([x.ravel(), y.ravel(), z.ravel()])
        rotated_points = np.array([Geometry.rotate_vector(
            point, axis, rotation_angle/180*np.pi
        ) for point in points])
        
        # Translate
        rotated_points[:, 0] += translate[0]
        rotated_points[:, 1] += translate[1]
        rotated_points[:, 2] += translate[2]
        
        return rotated_points


class DataProcessor:
    """Handles data reading and processing"""
    
    @staticmethod
    def read_body_data(filename):
        """Read body result data"""
        bodyvalue = []
        with open(filename, 'r') as file:
            for i, line in enumerate(file):
                if i == 0:
                    continue
                values = [float(x) for x in line.strip().split("\t")[3:]]
                bodyvalue.append(values)
        return np.array(bodyvalue)
    
    @staticmethod
    def extract_xy_axis(lines, initial):
        """Extract muscles, angles and node numbers from data lines"""
        muscles = []
        angles = []
        nodenum = []
        
        for i, line in enumerate(lines):
            row = line.strip().split('\t')
            if (row[0] not in muscles) and (row[0] != 'rotation_angle'):
                muscles.append(row[0])
            
            if muscles and row[0] == muscles[0]:
                nodenum.append(row[2])
                
            if i == 0:
                start_idx = 4 if initial == 1 else 3
                angles.extend([float(x) for x in row[start_idx:]])
        
        return muscles, angles, nodenum
    
    @staticmethod
    def arrange_to_list(lines, muscles, filename, initial):
        """Arrange raw data to 2D or 3D list"""
        allmuscledata = []
        
        if 'totalforce' not in filename:
            for muscle in muscles:
                muscledata = []
                for line in lines:
                    row = line.strip().split('\t')
                    if row[0] == muscle:
                        start_idx = 3 + initial
                        rowdata = [float(x) for x in row[start_idx:]]
                        muscledata.append(rowdata)
                allmuscledata.append(muscledata)
        else:
            for line in lines:
                row = line.strip().split('\t')
                if row[0] != 'rotation_angle':
                    start_idx = 3 + initial
                    muscledata = [float(x) for x in row[start_idx:]]
                    allmuscledata.append(muscledata)
        
        return allmuscledata
    
    @staticmethod
    def get_shape_names(filename):
        """Extract shape names from file"""
        shapename = []
        bodyname = []
        
        with open(filename, 'r') as f:
            for i, line in enumerate(f):
                if i == 0:
                    continue
                parts = line.strip().split('\t')
                if parts[0] not in bodyname:
                    bodyname.append(parts[0])
                    shapename.append(parts[1])
        
        return shapename


class Plotter:
    """Handles all plotting operations"""
    
    def __init__(self, scale, viewangle):
        self.scale = scale
        self.viewangle = viewangle
        self.boundaries = None
    
    def _set_plot_limits(self, ax):
        """Set plot limits automatically or manually"""
        if self.boundaries:
            # Use calculated boundaries
            ax.set_xlim(self.boundaries['x'][0], self.boundaries['x'][1])
            ax.set_ylim(self.boundaries['y'][0], self.boundaries['y'][1])
            ax.set_zlim(self.boundaries['z'][0], self.boundaries['z'][1])
        else:
            # Use manual scaling
            ax.set_xlim(-1.0 * self.scale, 1.0 * self.scale)
            ax.set_ylim(-1.0 * self.scale, 1.0 * self.scale)
            ax.set_zlim(-1.0 * self.scale, 1.0 * self.scale)
    
    def pcolor_fig(self, x, y, data_2d, ylabel_name, title, prefix_name):
        """Create pcolormesh figure"""
        fig = plt.figure()
        plt.pcolormesh(x, y, data_2d, cmap='viridis')
        plt.colorbar()
        plt.gca().xaxis.set_major_locator(MultipleLocator(120))
        plt.yticks(y)
        plt.xlabel('angle')
        plt.ylabel(ylabel_name)
        plt.title(prefix_name + title)
        figname = prefix_name + title + '.png'
        plt.savefig(figname)
        plt.close(fig)
    
    def normal_fig(self, data, line_names, x, ylabel_name, title):
        """Create normal line plot"""
        all_muscle_names = []
        fig = plt.figure()
        
        for i, line_data in enumerate(data):
            if 'length' in title:
                sum_data = np.sum(np.array(line_data), axis=0)
                plt.plot(x, sum_data, label=line_names[i], color=COLORS[i % len(COLORS)])
                if 'length' == title:
                    diff_data = sum_data[1:] - sum_data[:-1]
                    plt.plot(x[1:], diff_data, linestyle='--', 
                            label=line_names[i] + " difference", color=COLORS[i % len(COLORS)])
            else:
                if 'moment arm' == title:
                    muscle_base = line_names[i].split("-")[0]
                    if muscle_base in all_muscle_names:
                        idx = all_muscle_names.index(muscle_base)
                        plt.plot(x, line_data[0], label=line_names[i], color=COLORS[idx % len(COLORS)])
                    else:
                        all_muscle_names.append(muscle_base)
                        plt.plot(x, line_data[0], label=line_names[i], color=COLORS[i % len(COLORS)])
                else:
                    plt.plot(x, line_data, label=line_names[i], color=COLORS[i % len(COLORS)])
        
        plt.gca().xaxis.set_major_locator(MultipleLocator(120))
        plt.gca().xaxis.set_minor_locator(AutoLocator())
        plt.xlabel('angle')
        plt.ylabel(ylabel_name)
        plt.legend()
        figure_name = title + '.png'
        plt.savefig(figure_name)
        plt.close(fig)
    
    def plot_axis(self, ax, body_data, index):
        """Plot coordinate axes"""
        num_axes = body_data.shape[0] // 12
        for i in range(num_axes):
            start_idx = 12 * i
            # X axis
            ax.quiver(body_data[start_idx][index], body_data[start_idx+1][index], 
                     body_data[start_idx+2][index], body_data[start_idx+3][index],
                     body_data[start_idx+4][index], body_data[start_idx+5][index],
                     length=0.1, normalize=True, color=COLORS[0])
            # Y axis  
            ax.quiver(body_data[start_idx][index], body_data[start_idx+1][index],
                     body_data[start_idx+2][index], body_data[start_idx+6][index],
                     body_data[start_idx+7][index], body_data[start_idx+8][index],
                     length=0.1, normalize=True, color=COLORS[1])
            # Z axis
            ax.quiver(body_data[start_idx][index], body_data[start_idx+1][index],
                     body_data[start_idx+2][index], body_data[start_idx+9][index],
                     body_data[start_idx+10][index], body_data[start_idx+11][index],
                     length=0.1, normalize=True, color=COLORS[2])


class PenetrationAnalyzer:
    """Handles penetration analysis and plotting"""
    
    def __init__(self, plotter):
        self.plotter = plotter
    
    def analyze_penetration(self, data, x, y, muscle_pairs):
        """Analyze and plot penetration data"""
        muscles, tissues = self._extract_muscles_tissues(muscle_pairs)
        
        # Plot by muscle type
        for muscle in muscles:
            muscle_data = [data[j] for j, pair in enumerate(muscle_pairs) if muscle in pair]
            penetration_data = self._gather_penetration_data(muscle_data, tissues)
            self.plotter.pcolor_fig(x, y, penetration_data, 'node number', muscle, 'penetration_')
        
        # Plot by tissue type
        for tissue in tissues:
            tissue_data = [data[j] for j, pair in enumerate(muscle_pairs) if tissue in pair]
            penetration_data = self._gather_penetration_data(tissue_data, muscles)
            self.plotter.pcolor_fig(x, y, penetration_data, 'node number', tissue, 'penetration_')
    
    def _extract_muscles_tissues(self, muscle_pairs):
        """Extract unique muscles and tissues from pairs"""
        muscles = []
        tissues = []
        for pair in muscle_pairs:
            names = pair.strip().split('-')
            if names[0] not in muscles:
                muscles.append(names[0])
            if names[1] not in tissues:
                tissues.append(names[1])
        return muscles, tissues
    
    def _gather_penetration_data(self, data, reference_list):
        """Gather penetration data from multiple sources"""
        all_penetration = []
        for m in range(len(data[0])):
            line_data = []
            for n in range(len(data[0][0])):
                values = [data[p][m][n] for p in range(len(reference_list))]
                penetration = np.min(np.array(values)) if np.min(np.array(values)) <= 0 else 0
                line_data.append(penetration)
            all_penetration.append(line_data)
        return all_penetration


class AnimationCreator:
    """Handles animation creation"""
    
    def __init__(self, plotter, data_processor):
        self.plotter = plotter
        self.data_processor = data_processor
        self.boundary_calculator = BoundaryCalculator()
    
    def create_body_animation(self, body_data, gamma_data, shape_names, folder_name, 
                            view_angle, axis_plot, body_values, auto_scale=True):
        """Create body rotation animation with automatic scaling"""
        # Calculate global boundaries if auto_scale is enabled
        if auto_scale:
            boundaries = self.boundary_calculator.calculate_global_boundaries(body_data, gamma_data)
            # Update plotter scale based on the largest dimension
            max_range = max(
                boundaries['x'][1] - boundaries['x'][0],
                boundaries['y'][1] - boundaries['y'][0],
                boundaries['z'][1] - boundaries['z'][0]
            )
            # Use the largest range as the scale for all dimensions
            self.plotter.scale = max_range / 2
            self.plotter.boundaries = boundaries
        else:
            self.plotter.boundaries = None
        
        gamma_array = np.array(gamma_data)
        stride = 50
        
        # Create frames
        for i in range(len(body_data[0][0])):
            self._create_single_frame(body_data, gamma_array, shape_names, 
                                    folder_name, i, stride, view_angle, axis_plot, body_values)
        
        # Create animation
        self._create_gif_animation(folder_name, len(body_data[0][0]))
    
    def _create_single_frame(self, body_data, gamma_array, shape_names, folder_name, 
                           frame_idx, stride, view_angle, axis_plot, body_values):
        """Create a single animation frame"""
        figure_name = f'{folder_name}/bodyfigure{frame_idx}'
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        
        # Plot bodies - all in black
        for j, muscle_data in enumerate(body_data):
            axis = [muscle_data[3][frame_idx], muscle_data[4][frame_idx], muscle_data[5][frame_idx]]
            translate = [muscle_data[7][frame_idx], muscle_data[8][frame_idx], muscle_data[9][frame_idx]]
            
            if shape_names[j] == "ellipsoid":
                ellipsoid = Ellipsoid(muscle_data[0][frame_idx], muscle_data[1][frame_idx], 
                                    muscle_data[2][frame_idx], axis, muscle_data[6][frame_idx], translate)
                ellipsoid.plot(ax, 'gray', stride) 
                # ellipsoid.plot(ax, COLORS[len(COLORS)-1-j % len(COLORS)], stride)
            elif shape_names[j] == "cylinder":
                cylinder = EllipticalCylinder(muscle_data[0][frame_idx], muscle_data[1][frame_idx],
                                            muscle_data[2][frame_idx], axis, muscle_data[6][frame_idx], translate)
                cylinder.plot(ax, 'gray', stride)
                # cylinder.plot(ax, COLORS[len(COLORS)-1-j % len(COLORS)], stride)
        
        # Plot axes if requested
        if axis_plot:
            self.plotter.plot_axis(ax, body_values, frame_idx + 1)
        
        # Plot gamma trajectories
        for j, gamma_j in enumerate(gamma_array):
            gamma_points = gamma_j[:, frame_idx].reshape(-1, 3)
            ax.plot(gamma_points[:, 0], gamma_points[:, 1], gamma_points[:, 2], 
                   linewidth=2.8, linestyle='-', color=COLORS[j % len(COLORS)])  # 肌肉用彩色
            ax.scatter(gamma_points[0, 0], gamma_points[0, 1], gamma_points[0, 2], 
                      color=COLORS[j % len(COLORS)], marker='o', s=30)  # 起点用彩色
            ax.scatter(gamma_points[-1, 0], gamma_points[-1, 1], gamma_points[-1, 2], 
                      color=COLORS[j % len(COLORS)], marker='o', s=30)  # 终点用彩色
        
        # Configure plot with automatic or manual scaling
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        self.plotter._set_plot_limits(ax)  # 使用自动或手动缩放
        ax.axis('off')
        ax.view_init(elev=0, azim=view_angle)
        plt.savefig(figure_name)
        plt.close(fig)
    
    def _create_gif_animation(self, folder_name, num_frames):
        """Create GIF animation from frames"""
        fig = plt.figure()
        images = []
        for i in range(num_frames):
            image = Image.open(f"{folder_name}/bodyfigure{i}.png")
            images.append([plt.imshow(image)])
            plt.axis('off')
        
        ani = animation.ArtistAnimation(fig, images, interval=100, blit=True, repeat_delay=1000)
        ani.save(f'{folder_name}.gif', writer='pillow', fps=10)
        plt.close(fig)


class Postprocess:
    """Main postprocessing class"""
    
    def __init__(self, modelname, scale, viewangle, axisplot, auto_scale):
        self.modelname = modelname
        self.scale = scale
        self.viewangle = viewangle
        self.axisplot = axisplot
        self.auto_scale = auto_scale
        
        # Initialize components
        self.data_processor = DataProcessor()
        self.plotter = Plotter(scale, viewangle)
        self.animation_creator = AnimationCreator(self.plotter, self.data_processor)
        self.penetration_analyzer = PenetrationAnalyzer(self.plotter)
        
        # Load body data
        filename_body = f"{self.modelname}_body_result.txt"
        self.bodyvalue = self.data_processor.read_body_data(filename_body)
    
    def check_body(self):
        """Check and plot initial body configuration"""
        filename_bodystate = f"{self.modelname}_bodystate_result.txt"
        statename, statevalue, shapename = self._read_body_state(filename_bodystate)
        
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        stride = 50
        
        # Plot each body part
        for i, (name, values, shape) in enumerate(zip(statename, statevalue, shapename)):
            axis = [values[3], values[4], values[5]]
            translate = [values[7], values[8], values[9]]
            
            if shape == "ellipsoid":
                ellipsoid = Ellipsoid(values[0], values[1], values[2], axis, values[6], translate)
                ellipsoid.plot(ax, 'gray', stride)
                # ellipsoid.plot(ax, COLORS[len(COLORS)-1-j % len(COLORS)], stride)
            elif shape == "cylinder":
                cylinder = EllipticalCylinder(values[0], values[1], values[2], axis, values[6], translate)
                cylinder.plot(ax, 'gray', stride)
                # cylinder.plot(ax, COLORS[len(COLORS)-1-j % len(COLORS)], stride)
            self.plotter.plot_axis(ax, self.bodyvalue, 0)
        
        # Configure final plot
        figure_name = 'body_initial.png'
        ax.set_xlim(-1.0 * self.scale, 1.0 * self.scale)
        ax.set_ylim(-1.0 * self.scale, 1.0 * self.scale)
        ax.set_zlim(-1.0 * self.scale, 1.0 * self.scale)
        ax.axis('off')
        ax.view_init(elev=0, azim=self.viewangle)
        plt.savefig(figure_name)
        plt.close(fig)
    
    def _read_body_state(self, filename):
        """Read body state data from file"""
        statename, statevalue, shapename = [], [], []
        
        with open(filename, 'r') as file:
            for i, line in enumerate(file):
                if i == 0:
                    continue
                parts = line.strip().split('\t')
                if parts[0] not in statename:
                    statename.append(parts[0])
                    shapename.append(parts[1])
                    statevalue.append([float(x) for x in parts[3:]])
        
        return statename, statevalue, shapename
    
    def postprocessing_result(self):
        """Main postprocessing routine"""
        self.check_body()
        prefix = f"{self.modelname}_"
        types = ['length', 'forcenode', 'phi', 'totalforce', 'bodystate', 'momentarm']
        
        for data_type in types:
            filename = f"{prefix}{data_type}_result.txt"
            self._process_file(filename, data_type)
    
    def _process_file(self, filename, data_type):
        """Process individual data file"""
        with open(filename, 'r') as file:
            lines = file.readlines()
        
        if data_type in ['forcenode', 'totalforce', 'momentarm']:
            muscles, angles, nodenum = self.data_processor.extract_xy_axis(lines, 0)
            all_muscle_data = self.data_processor.arrange_to_list(lines, muscles, filename, 0)
            
            if data_type == 'forcenode':
                for i, muscle_data in enumerate(all_muscle_data):
                    self.plotter.pcolor_fig(angles, nodenum, np.array(muscle_data), 
                                          'node number', muscles[i], 'forcenode_')
            elif data_type == 'totalforce':
                self.plotter.normal_fig(all_muscle_data, muscles, angles, 'total force', 'totalforce')
            elif data_type == 'momentarm':
                self.plotter.normal_fig(all_muscle_data, muscles, angles, 'moment arm', 'moment arm')
                
        else:
            muscles, angles, nodenum = self.data_processor.extract_xy_axis(lines, 1)
            all_muscle_data = self.data_processor.arrange_to_list(lines, muscles, filename, 1)
            
            if data_type == 'length':
                self.plotter.normal_fig(all_muscle_data, muscles, angles, 'total length', 'total length')
                all_muscle_data_array = np.array(all_muscle_data)
                length_rate = (all_muscle_data_array[:, :, 1:] - all_muscle_data_array[:, :, :-1]) / (angles[1] - angles[0])
                self.plotter.normal_fig(length_rate, muscles, angles[1:], 'muscle length rate', 'muscle length rate')
                
            elif data_type == 'phi':
                for i, muscle_data in enumerate(all_muscle_data):
                    self.plotter.pcolor_fig(angles, nodenum, np.array(muscle_data), 
                                          'node number', muscles[i], 'phi_')
                self.penetration_analyzer.analyze_penetration(all_muscle_data, angles, nodenum, muscles)
                
            elif data_type == 'bodystate':
                self._process_body_state_data(all_muscle_data, muscles, angles)

    def _process_body_state_data(self, body_data, muscles, angles):
        """Process body state data and create animations"""
        # Create directories
        for folder in ["animation", "animation_front", "animation_side"]:
            if not os.path.exists(folder):
                os.makedirs(folder)
        
        shape_names = self.data_processor.get_shape_names(f"{self.modelname}_bodystate_result.txt")
        
        # Load gamma data
        filename_gamma = f"{self.modelname}_muscle_gamma_result.txt"
        with open(filename_gamma, 'r') as file:
            lines_gamma = file.readlines()
        
        gamma_muscles, gamma_angles, gamma_nodenum = self.data_processor.extract_xy_axis(lines_gamma, 1)
        all_muscle_data_gamma = self.data_processor.arrange_to_list(lines_gamma, gamma_muscles, filename_gamma, 1)
        
        # Create animations from different views with auto-scaling
        self.animation_creator.create_body_animation(
            body_data, all_muscle_data_gamma, shape_names, "animation_front", 
            self.viewangle + 90, self.axisplot, self.bodyvalue, auto_scale=self.auto_scale
        )
        
        self.animation_creator.create_body_animation(
            body_data, all_muscle_data_gamma, shape_names, "animation_side", 
            self.viewangle, self.axisplot, self.bodyvalue, auto_scale=self.auto_scale
        )


if __name__ == "__main__":
    argv = sys.argv
    
    if "-h" in argv:
        print("-model <modelname>: input model name (must give model name)")
        print("-scale <float>: input the value to scale the animation and body initial figure (default 1.0)")
        print("-viewangle <float>: input the view angle for body initial figure (default 0.0)")
        print("-onlyinitbody: only show the body initial figure")
        print("-axisplot: show coordinate axes in animation")
        print("-autoscale: enable automatic scaling based on model boundaries (recommended)")
    else:
        # Parse arguments
        scale = float(argv[argv.index("-scale") + 1]) if "-scale" in argv else 1.0
        viewangle = float(argv[argv.index("-viewangle") + 1]) if "-viewangle" in argv else 0.0
        axisplot = 1 if "-axisplot" in argv else 0
        auto_scale = 1 if "-autoscale" in argv else 0
        
        if "-model" in argv:
            modelname = argv[argv.index("-model") + 1]
            post = Postprocess(modelname, scale, viewangle, axisplot, auto_scale)
            
            if "-onlyinitbody" in argv:
                post.check_body()
            else:
                post.postprocessing_result()
        else:
            print("Error: Model name must be provided using -model argument")
            print("Use -h for help")