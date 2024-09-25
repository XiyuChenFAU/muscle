import os
import pandas as pd
import glob
import sys

datatype=["body", "bodystate", "forcenode", "length", "momentarm", "momentarmnode", "muscle_eta", "muscle_gamma", "phi", "totalforce"]

def combinetwofile(file1, file2, signvalue):
    df1 = pd.read_csv(file1, delimiter='\t')
    df1 = df1.dropna(axis=1, how='all')
    df2 = pd.read_csv(file2, delimiter='\t')
    df2 = df2.dropna(axis=1, how='all')
    print(df1)
    if 'initial' in df1.columns[0]:
        df1_label = df1.iloc[:, :5]
        df1_extra = signvalue * df1.iloc[:, 5:].iloc[:, ::-1]
        df2_extra = df2.iloc[:, 6:]
    else:
        df1_label = df1.iloc[:, :4]
        df1_extra = signvalue * df1.iloc[:, 4:].iloc[:, ::-1]
        df2_extra = df2.iloc[:, 5:]
    

    result = pd.concat([df1_label, df1_extra, df2_extra], axis=1)
    return result
    

def combinefolder(folder1, folder2, output_folder):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    for q in datatype:
        file1 = glob.glob(os.path.join(folder1, '*_'+q+'_result.txt'))
        file2 = glob.glob(os.path.join(folder2, '*_'+q+'_result.txt'))
        signvalue=1.0
        if "momentarm" in q:
            signvalue=-1.0
        result = combinetwofile(file1[0], file2[0], signvalue)
        if 'output_' in output_folder:
            result.to_csv(os.path.join(output_folder, output_folder.split('output_')[1]+'_'+q+'_result.txt'), sep='\t', index=False)
        else:
            result.to_csv(os.path.join(output_folder, output_folder+'_'+q+'_result.txt'), sep='\t', index=False)
    
argv=sys.argv
#first argv reverse file folder, second argv second file folder, third argv is output folder
combinefolder(argv[1],argv[2],argv[3])

