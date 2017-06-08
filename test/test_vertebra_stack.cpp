/*
Author: QIN Shuo
Date:   2017/6/2
Description:
	test for display property
*/

#include <string.h>
#include "vtkSmartPointer.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkSTLReader.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

int test_vertebra_stack()
{
	auto renderer = vtkSmartPointer<vtkRenderer>::New();
	auto win = vtkSmartPointer<vtkRenderWindow>::New();
	auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	win->AddRenderer(renderer);
	win->SetInteractor(interactor);

	auto read_stl = [](std::string name)
	{
		auto reader = vtkSmartPointer<vtkSTLReader>::New();
		reader->SetFileName(name.c_str());
		reader->Update();
		auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(reader->GetOutput());
		auto actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		return actor;
	};

	// read Throcic
	double zz = 0;
	double yy = 0;
	double xx = 0;
	std::string Th_base = "D:/Project/spine_reconstruct/resources/vertebra/T";
	for (size_t i = 1; i <= 12; i++)
	{
		auto cp = Th_base;
		auto name = cp.append(std::to_string(i)).append(".stl");
		auto actor = read_stl(name);
		actor->SetPosition(xx, yy, zz);
		renderer->AddActor(actor);

		zz += 50;
	}
	Th_base = "D:/Project/spine_reconstruct/resources/vertebra/L";
	for (size_t i = 1; i <= 5; i++)
	{
		auto cp = Th_base;
		auto name = cp.append(std::to_string(i)).append(".stl");
		auto actor = read_stl(name);	
		actor->SetPosition(xx, yy, zz);
		renderer->AddActor(actor);

		zz += 50;
	}

	renderer->ResetCamera();
	win->Render();
	interactor->Start();

	return 0;
}





