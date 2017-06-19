'''
Test rotation order
'''

import vtk
import numpy as np


def angleX2mat(angle):
    angle = angle*np.pi/180
    mat = np.zeros((3,3))
    mat[0,0] = 1.0
    mat[1,1] = np.cos(angle)
    mat[1,2] = -np.sin(angle)
    mat[2,1] = np.sin(angle)
    mat[2,2] = np.cos(angle)
    return mat

def angleY2mat(angle):
    angle = angle*np.pi/180
    mat = np.zeros((3,3))
    mat[0,0] = np.cos(angle)
    mat[0,2] = np.sin(angle)
    mat[1,1] = 1.0
    mat[2,0] = -np.sin(angle)
    mat[2,2] = np.cos(angle)
    return mat

def angleZ2mat(angle):
    angle = angle*np.pi/180
    mat = np.zeros((3,3))
    mat[0,0] = np.cos(angle)
    mat[0,1] = -np.sin(angle)
    mat[1,0] = np.sin(angle)
    mat[1,1] = np.cos(angle)
    mat[2,2] = 1.0
    return mat



def rotate_Z(vector):
    '''
    Input:
        vector: a 3x vector
    Return:
        a 3x3 rotation matrix
    Note: 
        Init pos: along Z axis
        Right handed coordinate system
        Rotation order: Z->X->Y->Z
    '''
    mat = np.zeros([3,3])
    mat[0][0] = -vector[2]
    mat[0][1] =  vector[1]
    mat[0][2] =  vector[0]
    return mat

def rotate_Y(vector):
    '''
    Input:
        vector: a 3x vector
    Return:
        3x3 matrix
    Note: 
        Init pos: along Y axis
        Right handed coordinate system
        Rotation order: Y->X->Y->Z
    '''
    nvector = vector/np.linalg.norm(vector)
    nvector = np.matrix(nvector).T
    ss = np.array([0,1,0])
    mat = nvector*ss
    return mat


def rotate_X(vector):
    '''
    Input:
        vector: a 3x vector
    Return:
        [anglex,angley,anglez]
    Note: 
        Init pos: along X axis
        Right handed coordinate system
        Rotation order: X->Y->Z
    '''

    # vector normalization
    nvector = vector/np.linalg.norm(vector)
    PI = 3.1415926
    angleZ = np.arctan(nvector[1]/nvector[0])*180/PI
    angleY = -np.arcsin(nvector[2]) * 180 /PI
    angleX = 0.0
    print("x: {}, y: {}, z: {}".format(angleX,angleY,angleZ))
    return angleX,angleY,angleZ


def rotate_matY(vector):
    y_axes = np.array([1,0,0])
    vector = vector/np.linalg.norm(vector)
    #y_axes_T = np.matrix(y_axes).T
    #mat = y_axes_T*vector

    vectorT = np.matrix(vector).T
    mat = vectorT*y_axes

    mul = mat*np.matrix(mat).T
    det = np.linalg.det(mat)
    return mat

def main():
    start = [0,0,0]
    end = [2,1,-0.5]

    ax,ay,az=rotate_X(end)

    def c_ball():
        sphere = vtk.vtkSphereSource()
        sphere.SetRadius(0.1) 
        sphere.Update()
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputData(sphere.GetOutput())
        actor = vtk.vtkActor()
        actor.SetMapper(mapper)
        return actor 

    def source():
        cylinder = vtk.vtkCylinderSource()
        cylinder.SetRadius(0.01)
        cylinder.SetHeight(5)
        cylinder.Update()
        return cylinder.GetOutput()
    def c_actor(src):
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputData(src)
        actor = vtk.vtkActor()
        actor.SetMapper(mapper)
        return actor

    source1 = source()

    mat1 = angleZ2mat(-90)
    mat2 = angleX2mat(ax)
    #mat2 = np.matmul(mat2,mat1)
    mat3 = angleY2mat(ay)
    #mat3 = np.matmul(mat3,mat2)
    mat4 = angleZ2mat(az)
    #mat4 = np.matmul(mat4,mat3)

    total_mat = np.matmul(mat4,np.matmul(mat3,np.matmul(mat2,mat1)))

    vmat = vtk.vtkMatrix4x4()
    vmat.Identity()
    for i in range(3):
        for j in range(3):
            vmat.SetElement(i,j,total_mat[i,j])
    vmat.Modified()
    
    trans = vtk.vtkTransform()
    trans.SetMatrix(vmat)
    transFilter = vtk.vtkTransformPolyDataFilter()
    transFilter.SetInputData(source1)
    transFilter.SetTransform(trans)
    transFilter.Update()
    source2z = transFilter.GetOutput()
    
    renderer = vtk.vtkRenderer()
    renderer.SetBackground(.4, .5, .6)

    src1 = c_actor(source1)
    src2 = c_actor(source2z)
    #renderer.AddActor(src1)
    renderer.AddActor(src2)

    axes = vtk.vtkAxesActor()
    renderer.AddActor(axes)
    ball = c_ball()
    ball.SetPosition(end)
    renderer.AddActor(ball)


    win = vtk.vtkRenderWindow()
    win.AddRenderer(renderer)
    interactor = vtk.vtkRenderWindowInteractor()
    win.SetInteractor(interactor)
    win.Render()
    interactor.Start()



if __name__ == '__main__':
    main()

