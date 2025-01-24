import glob
import sys

argv=sys.argv

def time_headline_value(folder_name,time_value):
    files = glob.glob(folder_name+"/"+folder_name[7:]+"*.txt")

    for file in files:
        with open(file, "r") as f:
            lines = f.readlines()
        
        first_line = lines[0].strip()
        tokens = first_line.split("\t") 
        
        if "initial" in first_line:
            header = tokens[:4]
            numbers = tokens[4:]
        else:
            header = tokens[:3]
            numbers = tokens[3:]
        modified_numbers = [str(float(num) * time_value) for num in numbers]
        str1=""
        for q in header:
            str1=str1+q+"\t"
        
        new_first_line = "\t".join(modified_numbers) + "\n"
        
        lines[0] = str1+new_first_line
        with open(file, "w") as f:
            f.writelines(lines)

time_headline_value(argv[1],float(argv[2]))