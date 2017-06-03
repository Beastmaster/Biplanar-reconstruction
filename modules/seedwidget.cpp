/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/3

reference: http://www.vtk.org/Wiki/VTK/Examples/Widgets/SeedWidgetImage

*/



#include "seedwidget.h"

#include "vtkObjectFactory.h"






vtkStandardNewMacro(seedwidgets_man);
void seedwidgets_man::SetInteractor(vtkRenderWindowInteractor * inter)
{
	m_interactor = inter;
	m_seedWidget->SetInteractor(m_interactor);
}
void seedwidgets_man::Enable()
{
	m_seedWidget->On();
}
void seedwidgets_man::Disable()
{
	m_seedWidget->Off();
}
seedwidgets_man::seedwidgets_man()
{
	// setup seed widgets
	// Create the representation
	vtkSmartPointer<vtkPointHandleRepresentation2D> handle =
		vtkSmartPointer<vtkPointHandleRepresentation2D>::New();
	handle->GetProperty()->SetColor(1, 0, 0);
	vtkSmartPointer<vtkSeedRepresentation> rep =
		vtkSmartPointer<vtkSeedRepresentation>::New();
	rep->SetHandleRepresentation(handle);

	// Seed widget
	m_seedWidget = vtkSmartPointer<vtkSeedWidget>::New();
	m_seedWidget->SetRepresentation(rep);

	vtkSmartPointer<vtkSeedImageCallback> seedCallback =
		vtkSmartPointer<vtkSeedImageCallback>::New();
	seedCallback->SetRepresentation(rep);
	seedCallback->SetWidget(m_seedWidget);
	m_seedWidget->AddObserver(vtkCommand::PlacePointEvent, seedCallback);
	m_seedWidget->AddObserver(vtkCommand::InteractionEvent, seedCallback);
	
}

seedwidgets_man::~seedwidgets_man()
{
}






