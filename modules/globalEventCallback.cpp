/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/6

Description:
	Callback fucntion, seed widget management
	For global interaction
*/


#include "vtkObjectFactory.h"
#include "vtkHandleWidget.h"
#include "vtkCoordinate.h"


#include "globalEventCallback.h"
#include "vtkSeedWidgetx.h"



void globalEventCallback::SetFrontalWidget(vtkSeedWidgetx* widget)
{
	this->m_FrontalSeedWidget = widget;
}

void globalEventCallback::SetProfileWidget(vtkSeedWidgetx * widget)
{
	this->m_ProfileSeedWidget = widget;
}


void globalEventCallback::Set3dWindow(vtkRenderWindow* win)
{
	m_3d_renWin = win;
	m_3d_renderer = win->GetRenderers()->GetFirstRenderer();
}

void globalEventCallback::UpdateView()
{
	// update 3d view
	auto pts = vtkSmartPointer<vtkPoints>::New();
	for (size_t i = 0; (i < m_coordinate.size()) && (i<m_3d_renderer->GetActors()->GetNumberOfItems()); i++)
	{
		double* pos = new double[3];
		pos[0] = m_coordinate[i][0];
		pos[1] = m_coordinate[i][2];
		pos[2] = m_coordinate[i][1];
		pts->InsertNextPoint(pos);
	}
	auto spline = vtkSmartPointer<vtksplineinterpolate>::New();
	spline->SetPoints(pts);
	spline->Update();
	auto rots = spline->GetTransformationList();

	for (size_t i = 0; (i < m_coordinate.size()) && (i<m_3d_renderer->GetActors()->GetNumberOfItems()); i++)
	{
		auto act = reinterpret_cast<vtkActor*>(m_3d_renderer->GetActors()->GetItemAsObject(i));
		auto trans = vtkSmartPointer<vtkTransform>::New();
		auto pt = pts->GetPoint(i);
		for (int ii = 0; ii < 3; ii++)
			rots[i]->SetElement(ii, 3, pt[ii]);
		trans->SetMatrix(rots[i]);
		act->SetUserTransform(trans);
	}
	m_3d_renWin->Render();
}



void globalEventCallback::Execute(vtkObject*, unsigned long event, void *calldata)
{
	int num_front_seeds = this->m_FrontalSeedWidget->GetNumberOfSeeds();
	int num_profi_seeds = this->m_ProfileSeedWidget->GetNumberOfSeeds();
	if (event == vtkCommand::DeleteEvent)
	{
		if (m_coordinate.size() > 0)
			m_coordinate.pop_back();
		else
			return;
		if (num_front_seeds>num_profi_seeds) // delete last front seed
		{
			m_FrontalSeedWidget->DeleteSeed(num_front_seeds - 1);
		}
		else if (num_front_seeds<num_profi_seeds) // delete last profile seed
		{
			m_ProfileSeedWidget->DeleteSeed(num_profi_seeds - 1);	
		}
		else
		{
			m_FrontalSeedWidget->DeleteSeed(num_front_seeds - 1);
			m_ProfileSeedWidget->DeleteSeed(num_profi_seeds - 1);
		}
		m_FrontalSeedWidget->Render();
		m_ProfileSeedWidget->Render();
	}
	if (event == vtkCommand::PlacePointEvent)
	{
		if (num_front_seeds>num_profi_seeds)
		{
			double pos[3];
			int id = num_front_seeds - 1;
			m_FrontalSeedWidget->GetSeedWorldPosition(id, pos);
			std::vector<double> tmp_pos(3,0.0);
			tmp_pos[0] = 100.0;
			tmp_pos[1] = pos[0];
			tmp_pos[2] = pos[1];
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
			m_ProfileSeedWidget->GetSeedWorldPosition(id, pos);

			std::vector<double> tmp_pos(3, 0.0);
			tmp_pos[0] = pos[0];
			tmp_pos[1] = 100.0;
			tmp_pos[2] = pos[1];
			m_coordinate.push_back(tmp_pos);

			// add seeds manually
			double new_pos[3]; memset(new_pos, 0, 3 * sizeof(double));
			new_pos[0] = 100; new_pos[1] = pos[1];
			auto tmp_seed = m_FrontalSeedWidget->CreateNewHandle();
			tmp_seed->GetHandleRepresentation()->SetWorldPosition(new_pos);
			tmp_seed->SetEnabled(1);
			m_FrontalSeedWidget->GetInteractor()->GetRenderWindow()->Render();
		}
	}  // end if: Place point
	if (event == vtkCommand::InteractionEvent)
	{
		if (calldata)
		{
			int flag_front_seed = m_FrontalSeedWidget->GetWidgetState();
			int flag_profi_seed = m_ProfileSeedWidget->GetWidgetState();
			if (flag_front_seed==8 || flag_profi_seed==8)
			{
				m_coordinate.clear();
			}
			for (unsigned int i = 0; i < (num_front_seeds>num_profi_seeds? num_profi_seeds:num_front_seeds); i++)
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
				this->m_FrontalSeedWidget->GetSeedWorldPosition(i, pos_frontal);
				this->m_ProfileSeedWidget->GetSeedWorldPosition(i, pos_profile);

				std::vector<double> tmp(3,0.0);
				if (flag_front_seed == 8)
				{
					tmp[0] = pos_profile[0]; 
					tmp[1] = pos_frontal[0]; // update y axis
					tmp[2] = pos_frontal[1]; // update z axis
				}
				else if (flag_profi_seed == 8)
				{
					tmp[0] = pos_profile[0]; // update x axis
					tmp[1] = pos_frontal[0];
					tmp[2] = pos_profile[1]; // update z axis

				}
				else
					return;

				m_coordinate.push_back(tmp);

				// update display position & convert world coordinate to display coordinate
				pos_frontal[0] = m_coordinate[i][1];
				pos_frontal[1] = m_coordinate[i][2];

				pos_profile[0] = m_coordinate[i][0];
				pos_profile[1] = m_coordinate[i][2];

				m_FrontalSeedWidget->SetSeedWorldPosition(i, pos_frontal);
				m_ProfileSeedWidget->SetSeedWorldPosition(i, pos_profile);
				m_FrontalSeedWidget->Render();
				m_ProfileSeedWidget->Render();
			} // end for
		}  // end if calldata
	} // end if (event == vtkCommand::InteractionEvent)

	//if ((event == vtkCommand::DeleteEvent) || (event == vtkCommand::InteractionEvent) || (event == vtkCommand::PlacePointEvent))
		UpdateView();

}



