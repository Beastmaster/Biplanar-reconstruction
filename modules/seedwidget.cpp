/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/3

reference: http://www.vtk.org/Wiki/VTK/Examples/Widgets/SeedWidgetImage

*/



#include "seedwidget.h"

#include "vtkObjectFactory.h"
#include "vtkHandleWidget.h"



void vtkSeedImageCallback::SetFrontalRepresentation(vtkSmartPointer<vtkSeedRepresentation> rep)
{
	this->m_FrontalSeedRepresentation = rep;
}
void vtkSeedImageCallback::SetFrontalWidget(vtkSmartPointer<vtkSeedWidget> widget)
{
	this->m_FrontalSeedWidget = widget;
}

void vtkSeedImageCallback::SetProfileRepresentation(vtkSmartPointer<vtkSeedRepresentation> rep)
{
	this->m_ProfileSeedRepresentation = rep;
}

void vtkSeedImageCallback::SetProfileWidget(vtkSmartPointer<vtkSeedWidget> widget)
{
	this->m_ProfileSeedWidget = widget;
}



void vtkSeedImageCallback::Execute(vtkObject*, unsigned long event, void *calldata)
{
	if (event == vtkCommand::PlacePointEvent)
	{
#ifdef DEBUG_MODE
		std::cout << "Placing point..." << std::endl;
#endif // DEBUG_MODE

		int num_front_seeds = this->m_FrontalSeedRepresentation->GetNumberOfSeeds();
		int num_profi_seeds = this->m_ProfileSeedRepresentation->GetNumberOfSeeds();

		if (num_front_seeds>num_profi_seeds)
		{
			double pos[3];
			int id = num_front_seeds - 1;
			//m_FrontalSeedRepresentation->GetSeedDisplayPosition(id,pos);
			m_FrontalSeedRepresentation->GetSeedWorldPosition(id,pos);
			std::vector<double> tmp_pos(pos, pos + sizeof(pos) / sizeof(double));
			m_coordinate.push_back(tmp_pos);
#ifdef DEBUG_MODE
			STD_COUT(pos[0] << "," << pos[1]);
#endif // !DEBUG_MODE

			// add seeds manually
			auto tmp_seed = m_ProfileSeedWidget->CreateNewHandle();
			//tmp_seed->GetHandleRepresentation()->SetDisplayPosition(pos);
			tmp_seed->GetHandleRepresentation()->SetWorldPosition(pos);
			tmp_seed->SetEnabled(1);
			m_ProfileSeedWidget->GetInteractor()->GetRenderWindow()->Render();
		}
		else if (num_profi_seeds>num_front_seeds)
		{
			double pos[3];
			int id = num_profi_seeds - 1;
			//m_ProfileSeedRepresentation->GetSeedDisplayPosition(id, pos);
			m_ProfileSeedRepresentation->GetSeedWorldPosition(id, pos);
#ifdef DEBUG_MODE
			STD_COUT(pos[0] << "," << pos[1]);
#endif // !DEBUG_MODE
			std::vector<double> tmp_pos(3, 0.0);
			tmp_pos[1] = pos[0];
			tmp_pos[2] = pos[1];
			m_coordinate.push_back(tmp_pos);

			// add seeds manually
			auto tmp_seed = m_FrontalSeedWidget->CreateNewHandle();
			//tmp_seed->GetHandleRepresentation()->SetDisplayPosition(pos);
			tmp_seed->GetHandleRepresentation()->SetWorldPosition(pos);
			tmp_seed->SetEnabled(1);
			m_FrontalSeedWidget->GetInteractor()->GetRenderWindow()->Render();
		}
#ifdef DEBUG_MODE
		//std::cout << "Number of frontal seeds before:" << num_front_seeds << std::endl;
		//std::cout << "Number of frontal seeds after:" << this->m_FrontalSeedRepresentation->GetNumberOfSeeds() <<std::endl;
		//std::cout << "Number of profile seeds before:" << num_profi_seeds << std::endl;
		//std::cout << "Number of profile seeds after:" << this->m_ProfileSeedRepresentation->GetNumberOfSeeds() << std::endl;
		//STD_COUT("Number of stored points: " << m_coordinate.size());
#endif // DEBUG_MODE
		return;
	}
	if (event == vtkCommand::InteractionEvent)
	{
#ifdef DEBUG_MODE
		//STD_COUT("Interaction... Update all seed position");
#endif // DEBUG_MODE
		if (calldata)
		{
			

			//this->SeedRepresentation->GetSeedDisplayPosition(0, pos);
			//std::cout << "Moved to (" << pos[0] << " " << pos[1] << " " << pos[2] << ")" << std::endl;

			//double* pos_frontal = new double[3]; memset(pos_frontal, 0, 3 * sizeof(double));
			//double* pos_profile = new double[3]; memset(pos_profile, 0, 3 * sizeof(double));
			//for (size_t i = 0; i < m_coordinate.size(); i++)
			//{
			//	pos_frontal[0] = m_coordinate.at(i).at(0);
			//	pos_frontal[1] = m_coordinate.at(i).at(1);				
			//	pos_profile[0] = m_coordinate.at(i).at(1);
			//	pos_profile[1] = m_coordinate.at(i).at(2);
			//}

		}
		return;
	}
}




//#########################################################//

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

void seedwidgets_man::SetDirection(View_Direction dir)
{
	m_direction = dir;
	if (m_direction == Frontal)
	{
		seedCallback->SetFrontalRepresentation(m_seedRepresentation);
		seedCallback->SetFrontalWidget(m_seedWidget);
	}
	else if (m_direction == Profile)
	{
		seedCallback->SetProfileRepresentation(m_seedRepresentation);
		seedCallback->SetProfileWidget(m_seedWidget);
	}

	m_seedWidget->AddObserver(vtkCommand::PlacePointEvent, seedCallback);
	m_seedWidget->AddObserver(vtkCommand::InteractionEvent, seedCallback);
	//m_seedWidget->AddObserver(vtkCommand::EnterEvent, seedCallback);
}

void seedwidgets_man::SetCallBack(vtkSeedImageCallback * callback)
{
	seedCallback = callback;
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
}

seedwidgets_man::~seedwidgets_man()
{
}






