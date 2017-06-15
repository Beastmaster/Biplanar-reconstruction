/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/5


reference:
http://www.vtk.org/Wiki/VTK/Examples/Cxx/VisualizationAlgorithms/TubesFromSplines
*/





#include <vtkSmartPointer.h>
#include <vtkVersion.h>

#include <vtkParametricFunctionSource.h>
#include <vtkTupleInterpolator.h>
#include <vtkTubeFilter.h>
#include <vtkParametricSpline.h>

#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSphereSource.h>
#include <vtkArrowSource.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include  <vtkMath.h>
#include <gdcmSmartPointer.h>

#include "vtksplineinterpolate.h"

#include <functional>

int test_3d_curve()
{
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	points->InsertPoint(0, 1, 0, 0);
	points->InsertPoint(1, 2, 0, 0);
	points->InsertPoint(2, 3, 1, 0);
	points->InsertPoint(3, 4, 1, 0);
	points->InsertPoint(4, 5, 0, 0);
	points->InsertPoint(5, 6, 0, 0);

	// Fit a spline to the points
	vtkSmartPointer<vtkParametricSpline> spline =
		vtkSmartPointer<vtkParametricSpline>::New();
	spline->SetPoints(points);
	vtkSmartPointer<vtkParametricFunctionSource> functionSource =
		vtkSmartPointer<vtkParametricFunctionSource>::New();
	functionSource->SetParametricFunction(spline);
	functionSource->SetUResolution(10 * points->GetNumberOfPoints());
	functionSource->Update();

	// Interpolate the scalars
	double rad;
	vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius =
		vtkSmartPointer<vtkTupleInterpolator> ::New();
	interpolatedRadius->SetInterpolationTypeToLinear();
	interpolatedRadius->SetNumberOfComponents(1);
	rad = .2; interpolatedRadius->AddTuple(0, &rad);
	rad = .2; interpolatedRadius->AddTuple(1, &rad);
	rad = .2; interpolatedRadius->AddTuple(2, &rad);
	rad = .1; interpolatedRadius->AddTuple(3, &rad);
	rad = .1; interpolatedRadius->AddTuple(4, &rad);
	rad = .1; interpolatedRadius->AddTuple(5, &rad);

	// Generate the radius scalars
	vtkSmartPointer<vtkDoubleArray> tubeRadius =
		vtkSmartPointer<vtkDoubleArray>::New();
	unsigned int n = functionSource->GetOutput()->GetNumberOfPoints();
	tubeRadius->SetNumberOfTuples(n);
	tubeRadius->SetName("TubeRadius");
	double tMin = interpolatedRadius->GetMinimumT();
	double tMax = interpolatedRadius->GetMaximumT();
	double r;
	for (unsigned int i = 0; i < n; ++i)
	{
		double t = (tMax - tMin) / (n - 1) * i + tMin;
		interpolatedRadius->InterpolateTuple(t, &r);
		tubeRadius->SetTuple1(i, r);
	}

	// Add the scalars to the polydata
	vtkSmartPointer<vtkPolyData> tubePolyData =
		vtkSmartPointer<vtkPolyData>::New();
	tubePolyData = functionSource->GetOutput();
	tubePolyData->GetPointData()->AddArray(tubeRadius);
	tubePolyData->GetPointData()->SetActiveScalars("TubeRadius");

	// Create the tubes
	vtkSmartPointer<vtkTubeFilter> tuber =
		vtkSmartPointer<vtkTubeFilter>::New();
#if VTK_MAJOR_VERSION <= 5
	tuber->SetInput(tubePolyData);
#else
	tuber->SetInputData(tubePolyData);
#endif
	tuber->SetNumberOfSides(20);
	tuber->SetVaryRadiusToVaryRadiusByAbsoluteScalar();

	//--------------
	// Setup actors and mappers
	vtkSmartPointer<vtkPolyDataMapper> lineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
	lineMapper->SetInput(tubePolyData);
#else
	lineMapper->SetInputData(tubePolyData);
#endif
	lineMapper->SetScalarRange(tubePolyData->GetScalarRange());

	vtkSmartPointer<vtkPolyDataMapper> tubeMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	tubeMapper->SetInputConnection(tuber->GetOutputPort());
	tubeMapper->SetScalarRange(tubePolyData->GetScalarRange());

	vtkSmartPointer<vtkActor> lineActor = vtkSmartPointer<vtkActor>::New();
	lineActor->SetMapper(lineMapper);
	vtkSmartPointer<vtkActor> tubeActor = vtkSmartPointer<vtkActor>::New();
	tubeActor->SetMapper(tubeMapper);

	// Setup render window, renderer, and interactor
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderer->AddActor(lineActor);
	renderer->AddActor(tubeActor);
	renderer->SetBackground(.4, .5, .6);
	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}




int test_3d_curve_class()
{
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	points->InsertPoint(0, 10/2, 0, 0);
	points->InsertPoint(1, 20/2, 0, 0);
	points->InsertPoint(2, 30/2, 10/2, 0);
	points->InsertPoint(3, 40/2, 10/2, 0);
	points->InsertPoint(4, 50/2, 0, 10);
	points->InsertPoint(5, 60/2, 0, 0);

	auto spline = vtkSmartPointer<vtksplineinterpolate>::New();
	spline->SetPoints(points);
	spline->Update();
	auto rots = spline->GetTransformationList();

	auto ptpoly = spline->GetOutput();

	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(ptpoly);

	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	// Setup render window, renderer, and interactor
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderer->AddActor(actor);
	for (size_t i = 0; i < points->GetNumberOfPoints(); i++)
	{
		auto arrow = vtkSmartPointer<vtkArrowSource>::New();
		arrow->Update();
		
		auto trans = vtkSmartPointer<vtkTransform>::New();
		trans->SetMatrix(rots.at(i));
		vtkSmartPointer<vtkTransformPolyDataFilter> transformPD =
			vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		transformPD->SetTransform(trans);
		transformPD->SetInputData(arrow->GetOutput());
		transformPD->Update();

		auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(transformPD->GetOutput());
		//mapper->SetInputData(arrow->GetOutput());
		actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		actor->SetPosition(points->GetPoint(i));
		renderer->AddActor(actor);
	}


	vtkSmartPointer<vtkAxesActor> axes =
		vtkSmartPointer<vtkAxesActor>::New();

	vtkSmartPointer<vtkOrientationMarkerWidget> widget =
		vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	widget->SetOrientationMarker(axes);
	widget->SetInteractor(renderWindowInteractor);
	widget->SetViewport(0.0, 0.0, 0.4, 0.4);
	widget->SetEnabled(1);
	widget->InteractiveOn();

	renderer->ResetCamera();
	renderer->SetBackground(.4, .5, .6);
	renderWindow->Render();
	renderWindowInteractor->Start();

	return 0;
}




int test_orientation()
{
	auto create_sphere = []()
	{
		auto start_sphere = vtkSmartPointer<vtkSphereSource>::New();
		//start_sphere->SetRadius(1);
		start_sphere->Update(); 
		auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(start_sphere->GetOutput());
		auto actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		return actor;
	};



	double start[3] = { 5,0,0 };
	double end[3] = { 5.59111,-0.0916362,-0.00732185 };

	//
	//ptx1:  8.81125,-0.214678,-0.0171531
	//ptx2:  10.4373, 0.158952, 0.0126282
	//ptx1 : 14.5423, 4.68594, 0.0677066
	//ptx2 : 15.6663, 5.27892, -0.1141
	//ptx1 : 19.1717, 5.24086, -0.274653
	//ptx2 : 20.4074, 4.8453, 0.240367


	// Compute a basis
	double normalizedX[3];
	double normalizedY[3];
	double normalizedZ[3];

	// The X axis is a vector from start to end
	vtkMath::Subtract(end, start, normalizedX);
	double length = vtkMath::Norm(normalizedX);
	vtkMath::Normalize(normalizedX);

	// The Z axis is an arbitrary vector cross X
	double arbitrary[3];
	arbitrary[0] = vtkMath::Random(-10, 10);
	arbitrary[1] = vtkMath::Random(-10, 10);
	arbitrary[2] = vtkMath::Random(-10, 10);
	vtkMath::Cross(normalizedX, arbitrary, normalizedZ);
	vtkMath::Normalize(normalizedZ);

	// The Y axis is Z cross X
	vtkMath::Cross(normalizedZ, normalizedX, normalizedY);
	vtkSmartPointer<vtkMatrix4x4> matrix =
		vtkSmartPointer<vtkMatrix4x4>::New();

	// Create the direction cosine matrix
	matrix->Identity();
	for (unsigned int i = 0; i < 3; i++)
	{
		matrix->SetElement(i, 0, normalizedX[i]);
		matrix->SetElement(i, 1, normalizedY[i]);
		matrix->SetElement(i, 2, normalizedZ[i]);
	}
	auto transform = vtkSmartPointer<vtkTransform>::New();
	transform->SetMatrix(matrix);

	auto sphere1 = create_sphere();
	sphere1->SetPosition(start);
	auto sphere2 = create_sphere();
	sphere2->SetPosition(end);

	auto arrow = vtkSmartPointer<vtkArrowSource>::New();
	//arrow->SetTipLength(10);
	arrow->Update();
	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(arrow->GetOutput());
	auto arr_actor = vtkSmartPointer<vtkActor>::New();
	arr_actor->SetMapper(mapper);
	arr_actor->SetUserTransform(transform);
	//arr_actor->RotateY(y_xz);
	//arr_actor->RotateZ(z_xy);
	arr_actor->SetPosition(start);

	// Setup render window, renderer, and interactor
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderer->AddActor(sphere1);
	renderer->AddActor(sphere2);
	renderer->AddActor(arr_actor);

	// create coordinate widget
	vtkSmartPointer<vtkAxesActor> axes =
		vtkSmartPointer<vtkAxesActor>::New();
	vtkSmartPointer<vtkOrientationMarkerWidget> widget =
		vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	widget->SetOrientationMarker(axes);
	widget->SetInteractor(renderWindowInteractor);
	widget->SetViewport(0.0, 0.0, 0.4, 0.4);
	widget->SetEnabled(1);
	widget->InteractiveOn();

	renderer->ResetCamera();
	renderer->SetBackground(.4, .5, .6);
	renderWindow->Render();
	renderWindowInteractor->Start();

	return 0;

}





