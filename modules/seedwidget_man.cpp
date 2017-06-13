/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/3

reference: http://www.vtk.org/Wiki/VTK/Examples/Widgets/SeedWidgetImage

*/

#include "vtkObjectFactory.h"
#include "vtkHandleWidget.h"

#include "seedwidget_man.h"


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
void seedwidgets_man::SetSeedWorldPosition(unsigned int seedID, double pos[3])
{
	unsigned int total_seeds = m_seedRepresentation->GetNumberOfSeeds();
	if (total_seeds<=seedID)
		return;

	m_seedRepresentation->GetHandleRepresentation(seedID)->SetWorldPosition(pos);
	m_interactor->GetRenderWindow()->Render();
}

void seedwidgets_man::SetDirection(View_Direction dir)
{
	m_direction = dir;
	
	if (dir == Frontal)
	{
		seedCallback->SetFrontalRepresentation(m_seedRepresentation);
		seedCallback->SetFrontalWidget(m_seedWidget);
	}
	
	if (dir == Profile)
	{
		seedCallback->SetProfileRepresentation(m_seedRepresentation);
		seedCallback->SetProfileWidget(m_seedWidget);
	}
	m_seedWidget->AddObserver(vtkCommand::PlacePointEvent, seedCallback);
	m_seedWidget->AddObserver(vtkCommand::InteractionEvent, seedCallback);
	m_seedWidget->AddObserver(vtkCommand::DeleteEvent, seedCallback);
}

void seedwidgets_man::SetCallBack(globalEventCallback * callback)
{
	seedCallback = callback;
}

void seedwidgets_man::AddSeed(double pos[3])
{
	auto tmp_seed = m_seedWidget->CreateNewHandle();
	tmp_seed->GetHandleRepresentation()->SetWorldPosition(pos);
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
	m_seedWidget = vtkSmartPointer<vtkSeedWidgetx>::New();
	m_seedWidget->SetRepresentation(m_seedRepresentation);
}

seedwidgets_man::~seedwidgets_man()
{
}







