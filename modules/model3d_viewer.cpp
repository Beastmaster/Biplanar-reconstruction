/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/6

Description:
3d visualization of spine model

*/





#include "vtkObjectFactory.h"



#include "model3d_viewer.h"

void callbackfunction(vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* vtkNotUsed(clientData), void* vtkNotUsed(callData))
{
	std::cout << "callback triger" << std::endl;

	vtkRenderWindowInteractor *iren =
		static_cast<vtkRenderWindowInteractor*>(caller);

	std::cout << "Pressed: " << iren->GetKeySym() << std::endl;
}

vtkStandardNewMacro(model3d_viewer);
void model3d_viewer::SetRenderWindow(vtkRenderWindow * win)
{
	m_renWin = win;

	auto test_callback = vtkSmartPointer<vtkCallbackCommand>::New();
	test_callback->SetCallback(callbackfunction);
	auto interactor = win->GetInteractor();
}

model3d_viewer::model3d_viewer()
{
	m_renWin = vtkSmartPointer<vtkRenderWindow>::New();
	m_renderer = vtkSmartPointer<vtkRenderer>::New();
}

model3d_viewer::~model3d_viewer()
{
}








