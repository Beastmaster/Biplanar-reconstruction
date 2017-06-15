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

#include <vector>

#include "module_config.h"

class vtksplineinterpolate: public vtkParametricFunctionSource
{
public:
	static vtksplineinterpolate *New();
	vtkTypeMacro(vtksplineinterpolate, vtkParametricFunctionSource);


	void SetPoints(vtkPoints*);
	void SetPoints(std::vector<double*>);
	void Update();
	vtkPolyData* GetOutput();

	// Calculate the orientation on the knot point (transform matrix)
	std::vector<vtkSmartPointer<vtkMatrix4x4> > GetTransformationList();


protected:
	vtksplineinterpolate();
	~vtksplineinterpolate();

private:
	int m_inter_rate;
	std::vector<vtkSmartPointer<vtkMatrix4x4> >      m_rotation;
	vtkSmartPointer<vtkPoints> m_points;
	vtkSmartPointer<vtkPoints> m_inter_points;
	vtkSmartPointer < vtkParametricSpline > m_spline;
};



#endif // !_vtksplineinterpolate_h_

