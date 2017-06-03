/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/3

reference: http://www.vtk.org/Wiki/VTK/Examples/Widgets/SeedWidgetImage

*/

#ifndef _SEEDWIDGETS_CALLBACKS_H_
#define _SEEDWIDGETS_CALLBACKS_H_

#include "vtkObject.h"
#include <vtkCommand.h>
#include <vtkImageActor.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty2D.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>
#include <vtkSmartPointer.h>


class vtkSeedImageCallback : public vtkCommand
{
public:
	static vtkSeedImageCallback *New()
	{
		return new vtkSeedImageCallback;
	}

	vtkSeedImageCallback() {}

	virtual void Execute(vtkObject*, unsigned long event, void *calldata)
	{
		if (event == vtkCommand::PlacePointEvent)
		{
			std::cout << "Placing point..." << std::endl;
			std::cout << "There are now " << this->SeedRepresentation->GetNumberOfSeeds() << " seeds." << std::endl;
			for (unsigned int seedId = 0; seedId < this->SeedRepresentation->GetNumberOfSeeds(); seedId++)
			{
				double pos[3];
				this->SeedRepresentation->GetSeedDisplayPosition(seedId, pos);
				std::cout << "Seed " << seedId << " : (" << pos[0] << " " << pos[1] << " " << pos[2] << ")" << std::endl;
			}
			return;
		}
		if (event == vtkCommand::InteractionEvent)
		{
			std::cout << "Interaction..." << std::endl;
			if (calldata)
			{
				double pos[3];
				this->SeedRepresentation->GetSeedDisplayPosition(0, pos);
				std::cout << "Moved to (" << pos[0] << " " << pos[1] << " " << pos[2] << ")" << std::endl;
			}
			return;
		}
	}

	void SetRepresentation(vtkSmartPointer<vtkSeedRepresentation> rep)
	{
		this->SeedRepresentation = rep;
	}
	void SetWidget(vtkSmartPointer<vtkSeedWidget> widget)
	{
		this->SeedWidget = widget;
	}

private:
	vtkSeedRepresentation* SeedRepresentation;
	vtkSeedWidget* SeedWidget;
};




class seedwidgets_man : public vtkObject
{
public:
	static seedwidgets_man *New();
	vtkTypeMacro(seedwidgets_man, vtkObject);

	void SetInteractor(vtkRenderWindowInteractor* );
	void Enable();
	void Disable();


protected:
	seedwidgets_man();
	~seedwidgets_man();

private:
	vtkSmartPointer<vtkSeedWidget> m_seedWidget;
	vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
};






#endif // !_SEEDWIDGETS_CALLBACKS_H_