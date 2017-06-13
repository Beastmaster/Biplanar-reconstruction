/*
Author: QIN Shuo
Date:   2017/6/12
Description:
	Wrapper for vtkSpline interpolation
*/



#ifndef _vtksplineinterpolate_h_
#define _vtksplineinterpolate_h_

#include "vtkObject.h"
#include "vtkSmartPointer.h"
#include "vtkParametricSpline.h"
#include "vtkParametricFunctionSource.h"
#include "vtkDoubleArray.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"



class vtksplineinterpolate: public vtkParametricFunctionSource
{
public:
	static vtksplineinterpolate *New();
	vtkTypeMacro(vtksplineinterpolate, vtkParametricFunctionSource);



protected:
	vtksplineinterpolate();
	~vtksplineinterpolate();

private:
	vtkSmartPointer < vtkParametricSpline > m_spline;
};



#endif // !_vtksplineinterpolate_h_

