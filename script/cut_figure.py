import os
import sys
import glob
from PIL import Image


def cutfigure(input_directory, leftratio, rightratio, topratio, bottomratio):
    output_directory=input_directory+"_cut"
    os.makedirs(output_directory, exist_ok=True)

    png_files = glob.glob(input_directory+"/*.png")

    for idx, image_path in enumerate(png_files, start=1):
        # Open the image
        img = Image.open(image_path)
        
        # Define the dimensions for cutting
        width, height = img.size
        left = width * leftratio
        top = height * topratio
        right = width * rightratio
        bottom = height * bottomratio
        
        # Crop the image
        cropped_img = img.crop((left, top, right, bottom))
        _, filename = os.path.split(image_path)
        
        # Save the cropped image in the "figure_cut" directory
        output_path = os.path.join(output_directory, filename)
        cropped_img.save(output_path)
        
if __name__ == "__main__":
    argv=sys.argv
    if "-h" in argv:
        print("use command: python3 cut_figure.py inputfolder leftratio rightratio topratio bottomratio")

    else:
        if len(argv)>5:
            cutfigure(argv[1],float(argv[2]),float(argv[3]),float(argv[4]),float(argv[5]))
