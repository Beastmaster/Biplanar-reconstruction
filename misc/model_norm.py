'''
Author: QIN Shuo
Date: 2017/6/15

Description:
    Spine model normalization: move to centroid, correct orientation
'''

import glob
import os
import vtk
from util import *


def centerize(poly):
    """
    Move origin of the polydata to centeroid
    Input: vtkPolyData
    Output:vtkPolyData
    """
    def find_centeroid(poly):
        '''
        Return a 3x tuple
        '''
        centerMass = vtk.vtkCenterOfMass()
        centerMass.SetInputData(poly)
        centerMass.SetUseScalarsAsWeights(False)
        centerMass.Update()
        return centerMass.GetCenter()
        
    centeroid = find_centeroid(poly)
    print("centroid:",centeroid)
    #mat = vtk.vtkMatrix4x4()
    #for i in range(3):
    #    mat.SetElement(i,3,centeroid[i])
    #mat.Modified()
    centeroid = [-x for x in centeroid]
    trans = vtk.vtkTransform()
    trans.Translate(centeroid)
    transFilter = vtk.vtkTransformPolyDataFilter()
    transFilter.SetTransform(trans)
    transFilter.SetInputData(poly)
    transFilter.Update()
    return transFilter.GetOutput()


def test():
    fname = "D:/Project/spine_reconstruct/test/vertebras/1-L1_th.stl"
    poly = read_stl(fname)
    polyout = centerize(poly)
    fname = "test.stl"
    write_stl(polyout,fname)

def main():
    stl_dir = "D:/Project/spine_reconstruct/test/vertebras"
    dst_dir = "D:/Project/spine_reconstruct/test/vertebras_trans/"

    def per(fname):
        print(fname)
        poly = read_stl(fname)
        poly_out = centerize(poly)
        nfullname = os.path.join(dst_dir,os.path.basename(fname))
        write_stl(poly_out,nfullname)

    stl_names = glob.glob(os.path.join(stl_dir,"*.stl"))
    list(map(per,stl_names))

def test():
    items = [1, 2, 3, 4, 5]
    def sq(x):
        a=x**2
        print(a)
    print(list(map(sq, items)))

if __name__ == '__main__':
    #test()
    main()




