'''
Author: QIN Shuo
Date: 2017/6/16

Description:
    frequently used methods
'''




import vtk


def read_stl(fname):
    """
    fname:  string (.stl)
    Return: vtkPolyData
    """
    reader = vtk.vtkSTLReader()
    reader.SetFileName(fname)
    reader.Update()
    return reader.GetOutput()


def read_nii(fname):
    '''
    '''
    reader = vtk.vtkNIFTIImageReader()
    reader.SetFileName(fname)
    reader.Update()
    return reader.GetOutput()


def write_stl(poly,fname):
    """
    poly:  vtkPolyData
    fname: string (.stl)
    """
    writer = vtk.vtkSTLWriter()
    writer.SetInputData(poly)
    writer.SetFileName(fname)
    writer.Update()
    

