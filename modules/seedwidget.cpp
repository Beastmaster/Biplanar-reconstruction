/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/3

reference: http://www.vtk.org/Wiki/VTK/Examples/Widgets/SeedWidgetImage

*/



#include "seedwidget.h"

#include "vtkObjectFactory.h"
#include "vtkHandleWidget.h"
#include "vtkCoordinate.h"


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
			m_FrontalSeedRepresentation->GetSeedWorldPosition(id,pos);
			std::vector<double> tmp_pos(pos, pos + sizeof(pos) / sizeof(double));
			m_coordinate.push_back(tmp_pos);

			// add seeds manually
			double new_pos[3]; memset(new_pos, 0, 3 * sizeof(double));
			new_pos[0] = 100;  new_pos[1] = pos[1];
			auto tmp_seed = m_ProfileSeedWidget->CreateNewHandle();
			tmp_seed->GetHandleRepresentation()->SetWorldPosition(new_pos);
			tmp_seed->SetEnabled(1);
			m_ProfileSeedWidget->GetInteractor()->GetRenderWindow()->Render();
		}
		else if (num_profi_seeds>num_front_seeds)
		{
			double pos[3];
			int id = num_profi_seeds - 1;
			m_ProfileSeedRepresentation->GetSeedWorldPosition(id, pos);

			std::vector<double> tmp_pos(3, 0.0);
			tmp_pos[1] = pos[1];
			tmp_pos[2] = pos[0];
			m_coordinate.push_back(tmp_pos);

			// add seeds manually
			double new_pos[3]; memset(new_pos, 0, 3 * sizeof(double));
			new_pos[0] = 100;new_pos[1] = pos[1];
			auto tmp_seed = m_FrontalSeedWidget->CreateNewHandle();
			tmp_seed->GetHandleRepresentation()->SetWorldPosition(new_pos);
			tmp_seed->SetEnabled(1);
			m_FrontalSeedWidget->GetInteractor()->GetRenderWindow()->Render();
		}
		return;
	}
	if (event == vtkCommand::InteractionEvent)
	{
		if (calldata)
		{
			for (size_t i = 0; i < m_coordinate.size(); i++)
			{
				auto world2display = [](vtkRenderer* renderer, double* coor, double* nw) {
					vtkSmartPointer<vtkCoordinate> coordinate =
						vtkSmartPointer<vtkCoordinate>::New();
					coordinate->SetCoordinateSystemToWorld();
					coordinate->SetValue(coor[0], coor[1], 0);
					double* world = coordinate->GetComputedDoubleDisplayValue(renderer);
					nw[0] = world[0]; nw[1] = world[1]; nw[2] = world[2];
				};

				auto display2world = [](vtkRenderer* renderer, double* coor, double* nw) {
					vtkSmartPointer<vtkCoordinate> coordinate =
						vtkSmartPointer<vtkCoordinate>::New();
					coordinate->SetCoordinateSystemToDisplay();
					coordinate->SetValue(coor[0], coor[1], 0);
					double* world = coordinate->GetComputedWorldValue(renderer);
					nw[0] = world[0]; nw[1] = world[1]; nw[2] = world[2];
				};

				double pos_frontal[3]; memset(pos_frontal, 0, 3 * sizeof(double));
				double pos_profile[3]; memset(pos_profile, 0, 3 * sizeof(double));

				if (m_FrontalSeedWidget->GetWidgetState() == 8 )
				{
					this->m_FrontalSeedRepresentation->GetSeedWorldPosition(i, pos_frontal);
					m_coordinate[i][0] = pos_frontal[0];  // update x axis				
					m_coordinate[i][1] = pos_frontal[1];  // update y axis
				}
				else if(m_ProfileSeedWidget->GetWidgetState() == 8)
				{
					this->m_ProfileSeedRepresentation->GetSeedWorldPosition(i, pos_profile);
					m_coordinate[i][2] = pos_profile[0];  // update z axis
					m_coordinate[i][1] = pos_profile[1];  // update y axis
				}
				else
					return;
				
				// update display position & convert world coordinate to display coordinate
				pos_frontal[0] = m_coordinate[i][0];
				pos_frontal[1] = m_coordinate[i][1];

				pos_profile[0] = m_coordinate[i][2];
				pos_profile[1] = m_coordinate[i][1];

				m_FrontalSeedRepresentation->GetHandleRepresentation(i)->SetWorldPosition(pos_frontal);
				m_ProfileSeedRepresentation->GetHandleRepresentation(i)->SetWorldPosition(pos_profile);
				m_FrontalSeedWidget->Render();
				m_ProfileSeedWidget->Render();

			} // end for
		}  // end if calldata
		return;
	} // end if (event == vtkCommand::InteractionEvent)
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
void seedwidgets_man::SetSeedWorldPosition(unsigned int seedID, double pos[3])
{
	int total_seeds = m_seedRepresentation->GetNumberOfSeeds();
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







