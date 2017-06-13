/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/6

Description:
	3d visualization of spine model

*/


#include "vtkObjectFactory.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkAxesActor.h"

#include "model3d_viewer.h"


vtkStandardNewMacro(model3d_viewer);
void model3d_viewer::SetRenderWindow(vtkRenderWindow * win)
{
	m_renWin = win;
	m_renWin->AddRenderer(m_renderer);
	m_renderer->SetRenderWindow(m_renWin);
	m_renderer->ResetCamera();
	m_renderer->SetBackground(1,1,1);
}

void model3d_viewer::SetCallback(globalEventCallback * calls)
{
	m_seed_callback = calls;
	this->AddObserver(vtkCommand::PlacePointEvent, m_seed_callback);
	this->AddObserver(vtkCommand::InteractionEvent, m_seed_callback);
	this->AddObserver(vtkCommand::DeleteEvent, m_seed_callback);
}

void model3d_viewer::AddActor(vtkActor* act)
{
	m_renderer->AddActor(act);
	Render();
}

vtkActor * model3d_viewer::GetActor(int id)
{
	
	return reinterpret_cast<vtkActor*> (m_renderer->GetActors()->GetItemAsObject(id));
}

void model3d_viewer::Render()
{
	m_renWin->Render();
}

void model3d_viewer::ResetCamera()
{
	m_renderer->ResetCamera();
	Render();
}

vtkSmartPointer<vtkRenderWindow> model3d_viewer::GetRenderWindow()
{
	return m_renWin;
}

vtkSmartPointer<vtkRenderer> model3d_viewer::GetRenderer()
{
	return m_renderer;
}

model3d_viewer::model3d_viewer()
{
	m_renWin = vtkSmartPointer<vtkRenderWindow>::New();
	m_renderer = vtkSmartPointer<vtkRenderer>::New();
}

model3d_viewer::~model3d_viewer()
{
}



