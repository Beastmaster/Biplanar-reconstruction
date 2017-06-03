/*
Author: QIN Shuo
Date:   2017/6/2
Description:
Main file for test
*/


#include "dicom_retrieve.h"

#include "vtkImageData.h"

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageMapper.h>
#include <vtkImageMapper3D.h>
#include <vtkMapper.h>
#include <vtkImageActor.h>
#include <vtkActor2D.h>
#include <vtkNIFTIImageWriter.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleImage.h>
#include "interactor_style.h"

#include <stdio.h>
#include <iostream>

void test_reader(std::string folder_name)
{

	auto reader = vtkSmartPointer<dicom_retrieve>::New();
	reader->SetDicomFolder(folder_name);
	reader->Update();


	auto display_test = [](vtkImageData* im){
		double* range = im->GetScalarRange();
		auto mapper = vtkSmartPointer<vtkImageMapper>::New();
		mapper->SetInputData(im);
		mapper->SetColorWindow(range[1]-range[0]);
		mapper->SetColorLevel((range[1] - range[0])/2);
		//mapper->UpdateInformation();
		auto actor = vtkSmartPointer<vtkActor2D>::New();
		actor->SetMapper(mapper);
		return actor;
	};

	auto frontal = reader->GetFrontal();
	auto profile = reader->GetProfile();

	auto writer = vtkSmartPointer<vtkNIFTIImageWriter>::New();
	writer->SetInputData(frontal);
	writer->SetFileName("frontal.nii");
	writer->Update();

	auto actor1 = display_test(frontal);
	auto actor2 = display_test(profile);

	vtkSmartPointer<vtkCamera> camera =
		vtkSmartPointer<vtkCamera>::New();
	camera->SetParallelScale(0.01);

	auto renderer1 = vtkSmartPointer<vtkRenderer>::New();
	renderer1->SetViewport(0.0, 0.0, 0.5, 1);
	renderer1->SetActiveCamera(camera);
	renderer1->AddActor2D(actor1);
	

	auto renderer2 = vtkSmartPointer<vtkRenderer>::New();
	renderer2->SetViewport(0.5,0.0 , 1.0, 1);
	renderer2->AddActor2D(actor2);

	auto renWin = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renWin);
	auto style = vtkSmartPointer<interactor_style>::New();
	//renderWindowInteractor->SetInteractorStyle(style);


	renWin->AddRenderer(renderer1);
	renWin->AddRenderer(renderer2);
	renWin->SetSize(800,800);

	renWin->Render();
	renderWindowInteractor->Start();
}







