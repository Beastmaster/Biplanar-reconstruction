



import os
import glob


ddir = "D:/Project/spine_reconstruct/resources/vertebra/"

for stl in glob.glob(os.path.join(ddir,"*th.stl")):
    bname = os.path.basename(stl)
    b1 = bname.split("-")[1]
    b2 = b1.split("_")[0]+".stl"
    nstl = os.path.join(ddir,b2)
    os.rename(stl,nstl)
