/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/3

*/

#include "image_viewer.h"

#include "vtkObjectFactory.h"
#include "vtkImageMapper3D.h"
#include "vtkRenderWindowInteractor.h"


vtkStandardNewMacro(image_viewer);
image_viewer::image_viewer()
{
	m_frontal_win = vtkSmartPointer<vtkRenderWindow>::New();
	m_profile_win = vtkSmartPointer<vtkRenderWindow>::New();

	m_frontal_renderer = vtkSmartPointer<vtkRenderer>::New();
	m_profile_renderer = vtkSmartPointer<vtkRenderer>::New();

	m_frontal_windowlevel = vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();
	m_profile_windowlevel = vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();

	m_frontal_actor = vtkSmartPointer<vtkImageActor>::New();
	m_profile_actor = vtkSmartPointer<vtkImageActor>::New();
}

image_viewer::~image_viewer()
{

}


void image_viewer::SetFrontalWindow(vtkRenderWindow * im)
{
	m_frontal_win = im;
}

void image_viewer::SetProfileWindow(vtkRenderWindow * im)
{
	m_profile_win = im;
}

void image_viewer::SetFrontalImage(vtkImageData* im)
{
	m_FrontalImage = im;
	
	m_frontal_windowlevel->SetInputData(m_FrontalImage);
	auto range = m_FrontalImage->GetScalarRange();
	m_frontal_windowlevel->SetLevel((range[1] + range[0]) / 2);
	m_frontal_windowlevel->SetWindow(range[1] - range[0]);
	m_frontal_windowlevel->Update();
	m_frontal_actor->GetMapper()->SetInputData(m_frontal_windowlevel->GetOutput());
	m_frontal_renderer->AddActor(m_frontal_actor);
	m_frontal_renderer->ResetCamera();
	m_frontal_win->AddRenderer(m_frontal_renderer);
	m_frontal_win->GetInteractor()->SetInteractorStyle(vtkSmartPointer<interactor_style>::New());
	m_frontal_win->Render();

	// setup seeds widgets
	m_frontal_seeds = vtkSmartPointer<seedwidgets_man>::New();
	m_frontal_seeds->SetInteractor(m_frontal_win->GetInteractor());
	m_frontal_seeds->SetCallBack(m_seeds_callback);
	m_frontal_seeds->SetDirection(Frontal);
}

void image_viewer::SetProfileImage(vtkImageData* im)
{
	m_ProfileImage = im;

	m_profile_windowlevel->SetInputData(m_ProfileImage);
	auto range = m_ProfileImage->GetScalarRange();
	m_profile_windowlevel->SetLevel((range[1] + range[0]) / 2);
	m_profile_windowlevel->SetWindow(range[1] - range[0]);
	m_profile_windowlevel->Update();
	m_profile_actor->GetMapper()->SetInputData(m_profile_windowlevel->GetOutput());
	m_profile_renderer->AddActor(m_profile_actor);
	m_profile_renderer->ResetCamera();
	m_profile_win->AddRenderer(m_profile_renderer);
	m_profile_win->GetInteractor()->SetInteractorStyle(vtkSmartPointer<interactor_style>::New());
	m_profile_win->Render();
	// setup seeds
	m_profile_seeds = vtkSmartPointer<seedwidgets_man>::New();
	m_profile_seeds->SetInteractor(m_profile_win->GetInteractor());
	m_profile_seeds->SetCallBack(m_seeds_callback);
	m_profile_seeds->SetDirection(Profile);
}

void image_viewer::SetCallback(seedImageCallback * calls)
{
	m_seeds_callback = calls;
}



void image_viewer::Render()
{
	m_frontal_win->Render();
	m_profile_win->Render();
}

void image_viewer::EnableSeedWidgets()
{
	m_frontal_seeds->Enable();
	m_profile_seeds->Enable();
}

void image_viewer::DisableSeedWidgets()
{
	m_frontal_seeds->Disable();
	m_profile_seeds->Disable();
}

void image_viewer::SetFrontalSeedPos(unsigned int seedID, double pos[3])
{
	m_frontal_seeds->SetSeedWorldPosition(seedID, pos);
}

void image_viewer::AddFrontalSeed(double pos[3])
{
	m_frontal_seeds->AddSeed(pos);
	m_frontal_win->Render();
}

void image_viewer::SetProfileSeedPos(unsigned int seedID, double pos[3])
{
	m_profile_seeds->AddSeed(pos);
	m_profile_win->Render();
}

void image_viewer::AddProfileSeed(double pos[3])
{
	m_profile_seeds->AddSeed(pos);
	m_profile_win->Render();
}






