/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/6

Description:
	Callback fucntion, seed widget management
*/


#include "vtkObjectFactory.h"
#include "vtkHandleWidget.h"
#include "vtkCoordinate.h"


#include "globalEventCallback.h"



void globalEventCallback::SetFrontalRepresentation(vtkSmartPointer<vtkSeedRepresentation> rep)
{
	this->m_FrontalSeedRepresentation = rep;
}
void globalEventCallback::SetFrontalWidget(vtkSeedWidgetx* widget)
{
	this->m_FrontalSeedWidget = widget;
}

void globalEventCallback::SetProfileRepresentation(vtkSmartPointer<vtkSeedRepresentation> rep)
{
	this->m_ProfileSeedRepresentation = rep;
}

void globalEventCallback::SetProfileWidget(vtkSeedWidgetx* widget)
{
	this->m_ProfileSeedWidget = widget;
}

void globalEventCallback::Set3dRenderer(vtkRendererx * ren)
{
	m_3d_renderer = ren;
}


void globalEventCallback::Execute(vtkObject*, unsigned long event, void *calldata)
{
	m_FrontalSeedRepresentation->GetInteractionState();
	m_ProfileSeedRepresentation->GetInteractionState();
	if (event == vtkCommand::DeleteEvent)
	{
		if (m_coordinate.size() > 0)
			m_coordinate.pop_back();
		else
			return;

		int num_front_seeds = this->m_FrontalSeedRepresentation->GetNumberOfSeeds();
		int num_profi_seeds = this->m_ProfileSeedRepresentation->GetNumberOfSeeds();
		if (num_front_seeds>num_profi_seeds) // delete last front seed
		{
			//m_FrontalSeedWidget->DeleteSeed(num_front_seeds - 1);
			//m_FrontalSeedWidget->Render();
		}
		else if (num_front_seeds<num_profi_seeds) // delete last profile seed
		{
			//m_ProfileSeedWidget->DeleteSeed(num_profi_seeds - 1);
			//m_ProfileSeedWidget->Render();
		}
	}
	if (event == vtkCommand::PlacePointEvent)
	{
		int num_front_seeds = this->m_FrontalSeedRepresentation->GetNumberOfSeeds();
		int num_profi_seeds = this->m_ProfileSeedRepresentation->GetNumberOfSeeds();

		if (num_front_seeds>17 || num_profi_seeds>17)
		{
			return;
		}

		if (num_front_seeds>num_profi_seeds)
		{
			double pos[3];
			int id = num_front_seeds - 1;
			m_FrontalSeedRepresentation->GetSeedWorldPosition(id, pos);
			std::vector<double> tmp_pos(pos, pos + sizeof(pos) / sizeof(double));
			tmp_pos[2] = 100;
			m_coordinate.push_back(tmp_pos);

			// add seeds manually
			double new_pos[3]; memset(new_pos, 0, 3 * sizeof(double));
			new_pos[0] = 100;  new_pos[1] = pos[1];
			//auto tmp_seed = m_ProfileSeedWidget->CreateNewHandle();
			//tmp_seed->GetHandleRepresentation()->SetWorldPosition(new_pos);
			//tmp_seed->SetEnabled(1);
			//m_ProfileSeedWidget->GetInteractor()->GetRenderWindow()->Render();
		}
		else if (num_profi_seeds>num_front_seeds)
		{
			double pos[3];
			int id = num_profi_seeds - 1;
			m_ProfileSeedRepresentation->GetSeedWorldPosition(id, pos);

			std::vector<double> tmp_pos(3, 0.0);
			tmp_pos[1] = pos[1];
			tmp_pos[2] = pos[0];
			tmp_pos[0] = 100.0;
			m_coordinate.push_back(tmp_pos);

			// add seeds manually
			double new_pos[3]; memset(new_pos, 0, 3 * sizeof(double));
			new_pos[0] = 100; new_pos[1] = pos[1];
			//auto tmp_seed = m_FrontalSeedWidget->CreateNewHandle();
			//tmp_seed->GetHandleRepresentation()->SetWorldPosition(new_pos);
			//tmp_seed->SetEnabled(1);
			//m_FrontalSeedWidget->GetInteractor()->GetRenderWindow()->Render();
		}
	}  // end if: Place point
	if (event == vtkCommand::InteractionEvent)
	{
		if (calldata)
		{
			for (unsigned int i = 0; i < m_coordinate.size(); i++)
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

				//if (m_FrontalSeedWidget->GetWidgetState() == 8)
				//{
				//	this->m_FrontalSeedRepresentation->GetSeedWorldPosition(i, pos_frontal);
				//	m_coordinate[i][0] = pos_frontal[0];  // update x axis				
				//	m_coordinate[i][1] = pos_frontal[1];  // update y axis
				//}
				//else if (m_ProfileSeedWidget->GetWidgetState() == 8)
				//{
				//	this->m_ProfileSeedRepresentation->GetSeedWorldPosition(i, pos_profile);
				//	m_coordinate[i][2] = pos_profile[0];  // update z axis
				//	m_coordinate[i][1] = pos_profile[1];  // update y axis
				//}
				//else
				//	return;

				// update display position & convert world coordinate to display coordinate
				pos_frontal[0] = m_coordinate[i][0];
				pos_frontal[1] = m_coordinate[i][1];

				pos_profile[0] = m_coordinate[i][2];
				pos_profile[1] = m_coordinate[i][1];

				//m_FrontalSeedRepresentation->GetHandleRepresentation(i)->SetWorldPosition(pos_frontal);
				//m_ProfileSeedRepresentation->GetHandleRepresentation(i)->SetWorldPosition(pos_profile);
				//m_FrontalSeedWidget->Render();
				//m_ProfileSeedWidget->Render();
			} // end for
		}  // end if calldata
	} // end if (event == vtkCommand::InteractionEvent)

	if ((event == vtkCommand::DeleteEvent)||(event == vtkCommand::InteractionEvent)||(event == vtkCommand::PlacePointEvent) )
	{
		// update 3d view
		for (size_t i = 0; (i < m_coordinate.size()) && (i<m_3d_renderer->GetNumberOfActors()); i++)
		{
			double pos[3];
			pos[0] = m_coordinate[i][0];
			pos[1] = m_coordinate[i][2];
			pos[2] = m_coordinate[i][1];
			m_3d_renderer->GetActor(i)->SetPosition(pos);
			m_3d_renderer->Render();
		}
	}

}



