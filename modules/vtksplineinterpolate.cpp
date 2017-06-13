/*
Author: QIN Shuo
Date:   2017/6/12
Description:
	Wrapper for vtkSpline interpolation
*/


#include "vtkObjectFactory.h"

#include "vtksplineinterpolate.h"

vtkStandardNewMacro(vtksplineinterpolate);
vtksplineinterpolate::vtksplineinterpolate()
{
	m_spline = vtkSmartPointer<vtkParametricSpline>::New();
}

vtksplineinterpolate::~vtksplineinterpolate()
{
}






























