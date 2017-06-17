'''
Author: QIN Shuo
Date: 2017/6/15

Description:
    Use marchingcube to convert vertebra image to stl model
'''


import sys
import glob
import os
import vtk

from util import *

def generate_stl(fname,dst_dir):
    """
    fname:   .nii file
    dst_dir: stl file output directory
    """

    def MarchingCubes(image,threshold):
        '''
        http://www.vtk.org/Wiki/VTK/Examples/Cxx/Modelling/ExtractLargestIsosurface 
        '''
        mc = vtk.vtkMarchingCubes()
        mc.SetInputData(image)
        mc.ComputeNormalsOn()
        mc.ComputeGradientsOn()
        mc.SetValue(0, threshold)
        mc.Update()
        # To remain largest region
        confilter =vtk.vtkPolyDataConnectivityFilter()
        confilter.SetInputData(mc.GetOutput())
        confilter.SetExtractionModeToLargestRegion()
        confilter.Update()
        return confilter.GetOutput()
    
    def smoothPoly(poly,num_iter = 15,rlx_factor=0.1):
        '''
        http://www.vtk.org/doc/nightly/html/classvtkSmoothPolyDataFilter.html
        '''
        smoothFilter = vtk.vtkSmoothPolyDataFilter()
        smoothFilter.SetInputData(poly)
        smoothFilter.SetNumberOfIterations(num_iter)
        smoothFilter.SetRelaxationFactor(rlx_factor)
        smoothFilter.FeatureEdgeSmoothingOff()
        #smoothFilter.FeatureEdgeSmoothingOn()
        smoothFilter.BoundarySmoothingOff()
        #smoothFilter.BoundarySmoothingOff()
        smoothFilter.Update()
        return smoothFilter.GetOutput()


    def gen_name(fname,dst_dir=None):
        if dst_dir is None:
            fl = fname.split(".")
            bname = fl[0]+".stl"
            return bname
        else:
            bname = os.path.basename(fname)
            fl = bname.split(".")
            bname = fl[0]+".stl"
            return os.path.join(dst_dir,bname)

        
    nii = read_nii(fname)
    ori_poly = MarchingCubes(nii,1)
    smh_poly = smoothPoly(ori_poly)
    stl_name = gen_name(fname,dst_dir=dst_dir)
    write_stl(smh_poly,stl_name)


def get_fnames(ddir):
    lsx = []
    for dd in os.listdir(ddir):
        if os.path.isdir(os.path.join(ddir,dd)):
            niis = glob.glob(os.path.join(ddir,dd,"*th.nii"))
            lsx = lsx + niis
    return lsx

if __name__ == "__main__":
    
    src_dir = "E:/segmented ct spine_david/"
    fnames = get_fnames(src_dir)

    dst_dir = "D:/Project/spine_reconstruct/test"
    for ff in fnames:
        print(ff)
        generate_stl(ff,dst_dir)

