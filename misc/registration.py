'''
Author: QIN Shuo
Date: 2017/6/15

Description:
    Register vertebra to a specific vertebra
'''




import vtk


def register(ref_poly,mv_poly):
    """
    Input:
    ref_poly: reference vertebra, type: vtkPolyData
    mv_poly:  moving vertebra, type: vtkPolyData
    Return:
    vtkMatrix4x4
    """

    icp = vtk.vtkIterativeClosestPointTransform()
    icp.SetSource(ref_poly)
    icp.SetTarget(mv_poly)
    icp.GetLandmarkTransform().SetModeToRigidBody()
    icp.SetMaximumNumberOfIterations(20)
    #icp.StartByMatchingCentroidsOn()
    icp.Modified()
    icp.Update()
    mat = icp.GetMatrix()
    return mat


def transform_poly(poly_in,mat):
    trans = vtk.vtkTransform()
    trans.SetMatrix(mat)

    transFilter = vtk.vtkTransformPolyDataFilter()
    transFilter.SetInputData(poly_in)
    transFilter.SetTransform(trans)

    transFilter.Update()
    poly_out = transFilter.GetOutput()
    return poly_out


if __name__ == '__main__':
    pass


