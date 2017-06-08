

#include "vtkObjectFactory.h"

#include "vtkRendererx.h"


vtkStandardNewMacro(vtkRendererx);

void vtkRendererx::SetRenderWindow(vtkRenderWindow * win)
{
	m_win = win;
}

void vtkRendererx::SetRenderer(vtkRenderer * ren)
{
	m_renderer = ren;
}

void vtkRendererx::SetActor(int id, vtkActor * actor)
{
	m_renderer->GetActors()->ReplaceItem(id, actor);
}

void vtkRendererx::AddActor(vtkActor * act)
{
	m_renderer->AddActor(act);
}

void vtkRendererx::RemoveActor(int id)
{
	m_renderer->GetActors()->RemoveItem(id);
}

vtkActor * vtkRendererx::GetActor(int id)
{
	return reinterpret_cast<vtkActor*> (this->m_renderer->GetActors()->GetItemAsObject(id));
}

vtkRenderer * vtkRendererx::GetRenderer()
{
	return m_renderer;
}

int vtkRendererx::GetNumberOfActors()
{
	return m_renderer->GetActors()->GetNumberOfItems();
}

void vtkRendererx::ResetCamera()
{
	m_renderer->ResetCamera();
}

void vtkRendererx::Render()
{
	m_win->Render();
}

vtkRendererx::vtkRendererx()
{
	m_renderer = vtkSmartPointer<vtkRenderer>::New();
}



