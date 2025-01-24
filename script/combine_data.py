import os
import pandas as pd
import glob
import sys

datatype=["body", "bodystate", "forcenode", "length", "momentarm", "momentarmnode", "muscle_eta", "muscle_gamma", "phi", "totalforce"]

def readdata(filepath):

    with open(filepath, "r") as file:
        lines = file.readlines()

    header = lines[0].strip().split("\t")

    data = [line.strip().split("\t") for line in lines[1:]]

    df = pd.DataFrame(data, columns=header)
    return df


def combinetwofile(file1, file2):
    df1 = readdata(file1)
    df1 = df1.dropna(axis=1, how='all')
    df2 = readdata(file2)
    df2 = df2.dropna(axis=1, how='all')
    print(df1)
    if 'initial' in df1.columns:
        df1_label = df1.iloc[:, :4]
        df1_extra = df1.iloc[:, 4:].iloc[:, ::-1]
        df2_extra = df2.iloc[:, 5:]
    else:
        df1_label = df1.iloc[:, :3]
        df1_extra = df1.iloc[:, 3:].iloc[:, ::-1]
        df2_extra = df2.iloc[:, 4:]
    
    
    result = pd.concat([df1_label, df1_extra, df2_extra], axis=1)
    return result
    

def combinefolder(folder1, folder2, output_folder):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    for q in datatype:
        file1 = glob.glob(os.path.join(folder1, '*_'+q+'_result.txt'))
        file2 = glob.glob(os.path.join(folder2, '*_'+q+'_result.txt'))
        result = combinetwofile(file1[0],file2[0])
        if 'output_' in output_folder:
            result.to_csv(os.path.join(output_folder, output_folder.split('output_')[1]+'_'+q+'_result.txt'), sep='\t', index=False)
        else:
            result.to_csv(os.path.join(output_folder, output_folder+'_'+q+'_result.txt'), sep='\t', index=False)
    
argv=sys.argv
#first argv reverse file folder, second argv second file folder, third argv is output folder
combinefolder(argv[1],argv[2],argv[3])

