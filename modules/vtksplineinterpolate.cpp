/*
Author: QIN Shuo
Date:   2017/6/12
Description:
	Wrapper for vtkSpline interpolation
*/


#include "vtkObjectFactory.h"
#include "vtkKdTree.h"
#include "vtkIdList.h"
#include "vtkMatrix4x4.h"
#include "vtkMath.h"
#include "vtksplineinterpolate.h"



vtkStandardNewMacro(vtksplineinterpolate);

vtksplineinterpolate::vtksplineinterpolate()
{
	m_spline = vtkSmartPointer<vtkParametricSpline>::New();
	m_inter_rate = 10;
}

vtksplineinterpolate::~vtksplineinterpolate()
{
}



void vtksplineinterpolate::SetPoints(vtkPoints * pts)
{
	m_points = pts;
}

void vtksplineinterpolate::SetPoints(std::vector<double*> pts)
{
	m_points = vtkSmartPointer<vtkPoints>::New();
	for (size_t i = 0; i < pts.size(); i++)
	{
		m_points->InsertNextPoint(pts[i]);
	}
}

void vtksplineinterpolate::Update()
{
	m_spline->SetPoints(m_points);
	this->SetParametricFunction(m_spline);
	this->SetUResolution(m_inter_rate * m_points->GetNumberOfPoints());
	Superclass::Update();

	m_inter_points = vtkSmartPointer<vtkPoints>::New();
	m_inter_points = Superclass::GetOutput()->GetPoints();
}

vtkPolyData * vtksplineinterpolate::GetOutput()
{
	return Superclass::GetOutput();
}

std::vector<vtkSmartPointer<vtkMatrix4x4> > vtksplineinterpolate::GetTransformationList()
{
	// put interpolated points into a kd-tree
	auto kd_tree = vtkSmartPointer<vtkKdTree>::New();
	kd_tree->BuildLocatorFromPoints(m_inter_points);

	// iterate: find nearest points and ID
	for (size_t i = 0; i < m_points->GetNumberOfPoints(); i++)
	{
		auto pt = m_points->GetPoint(i);
		auto idlist = vtkSmartPointer<vtkIdList>::New();
		kd_tree->FindClosestNPoints(1, pt, idlist);

		int id0 = idlist->GetId(0);
		int id1 = 0;
		int id2 = 0;
		if (id0 < 5)
		{
			id1 = 0;
			id2 = 2;
		}
		else if (id0 == m_inter_points->GetNumberOfPoints() - 1)
		{
			id1 = id0 - 2;
			id2 = id0;
		}
		else
		{
			id1 = id0 - 1;
			id2 = id0 + 1;
		}

		double ptx1[3], ptx2[3];
		m_inter_points->GetPoint(id1,ptx1);
		m_inter_points->GetPoint(id2,ptx2);

		// Compute a basis
		double normalizedX[3];
		double normalizedY[3];
		double normalizedZ[3];

		// The X axis is a vector from start to end
		vtkMath::Subtract(ptx2, ptx1, normalizedX);
		double length = vtkMath::Norm(normalizedX);
		vtkMath::Normalize(normalizedX);



		// Create the direction cosine matrix
		auto matrix = vtkSmartPointer<vtkMatrix4x4>::New();
		matrix->Identity();
		for (unsigned int i = 0; i < 3; i++)
		{
			matrix->SetElement(i, 0, normalizedX[i]);
			matrix->SetElement(i, 1, normalizedY[i]);
			matrix->SetElement(i, 2, normalizedZ[i]);
		}
		matrix->Modified();

		m_rotation.push_back(matrix);

#ifdef DEBUG_MODE
		STD_COUT("id:" << id0);
		STD_COUT("ptx1:  " << ptx1[0] << "," << ptx1[1] << "," << ptx1[2]);
		STD_COUT("ptx2:  " << ptx2[0] << "," << ptx2[1] << "," << ptx2[2]);
		STD_COUT("determine: " << matrix->Determinant());
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				std::cout << matrix->GetElement(i, j) << ",";
			}
			std::cout << std::endl;
		}
		//STD_COUT("vect:"<<vector[0]<<","<<vector[1]<<","<<vector[2]);
#endif // !DEBUG_MODE
	}

	return m_rotation;
}

vtkSmartPointer<vtkMatrix4x4> vtksplineinterpolate::angleX2mat(double angle)
{
	auto mat = vtkSmartPointer<vtkMatrix4x4>::New();
	mat->SetElement(0,0,1.0);
	mat->SetElement(1,1,cos(angle));
	mat->SetElement(1, 2, -sin(angle));
	mat->SetElement(2, 1, sin(angle));
	mat->SetElement(2, 2, cos(angle));
	mat->Modified();
	return mat;
}

vtkSmartPointer<vtkMatrix4x4> vtksplineinterpolate::angleY2mat(double angle)
{
	angle = ANG2RAD(angle);
	auto mat = vtkSmartPointer<vtkMatrix4x4>::New();
	mat->SetElement(0, 0, cos(angle));
	mat->SetElement(0, 2, sin(angle));
	mat->SetElement(1, 1, 1.0);
	mat->SetElement(2, 0, -sin(angle));
	mat->SetElement(2, 2, cos(angle));
	mat->Modified();
	return mat;
}

vtkSmartPointer<vtkMatrix4x4> vtksplineinterpolate::angleZ2mat(double angle)
{
	angle = ANG2RAD(angle);
	auto mat = vtkSmartPointer<vtkMatrix4x4>::New();
	mat->SetElement(0, 0, cos(angle));
	mat->SetElement(0, 1, -sin(angle));
	mat->SetElement(1, 0, sin(angle));
	mat->SetElement(1, 1, cos(angle));
	mat->SetElement(2, 2, 1.0);
	mat->Modified();
	return mat;
}

inline void vtksplineinterpolate::rotate_XYZ(double * vec, double * anglex, double * angley, double * anglez)
{
	// normalize vector first
	vtkMath::Normalize(vec);

	*anglez = atan(vec[1] / vec[0]) * 180 / PI;
	*angley = -asin(vec[2]) * 180 / PI;
	*anglex = 0.0;
}





