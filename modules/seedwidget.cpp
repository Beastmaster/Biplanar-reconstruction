/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/3

reference: http://www.vtk.org/Wiki/VTK/Examples/Widgets/SeedWidgetImage

*/



#include "seedwidget.h"

#include "vtkObjectFactory.h"
#include "vtkHandleWidget.h"





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
void seedwidgets_man::SetSeedDisplayPosition(unsigned int seedID, double pos[3])
{
	int total_seeds = m_seedRepresentation->GetNumberOfSeeds();
	if (total_seeds<=seedID)
		return;

	m_seedRepresentation->SetSeedDisplayPosition(seedID,pos);
	m_interactor->GetRenderWindow()->Render();
}

void seedwidgets_man::AddSeed(double pos[3])
{
	auto tmp_seed = m_seedWidget->CreateNewHandle();
	tmp_seed->GetHandleRepresentation()->SetDisplayPosition(pos);
	tmp_seed->SetEnabled(1);
}



seedwidgets_man::seedwidgets_man()
{
	// setup seed widgets
	// Create the representation
	m_handle = vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
	m_handle->GetProperty()->SetColor(1, 1, 0);
	m_handle->SetHandleSize(20);
	m_seedRepresentation = vtkSmartPointer<vtkSeedRepresentation>::New();
	m_seedRepresentation->SetHandleRepresentation(m_handle);


	// Seed widget
	m_seedWidget = vtkSmartPointer<vtkSeedWidget>::New();
	m_seedWidget->SetRepresentation(m_seedRepresentation);

	vtkSmartPointer<vtkSeedImageCallback> seedCallback =
		vtkSmartPointer<vtkSeedImageCallback>::New();
	seedCallback->SetRepresentation(m_seedRepresentation);
	seedCallback->SetWidget(m_seedWidget);
	m_seedWidget->AddObserver(vtkCommand::PlacePointEvent, seedCallback);
	//m_seedWidget->AddObserver(vtkCommand::InteractionEvent, seedCallback);
}

seedwidgets_man::~seedwidgets_man()
{
}






