/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/3

reference: http://www.vtk.org/Wiki/VTK/Examples/Widgets/SeedWidgetImage

*/

#ifndef _SEEDWIDGETS_CALLBACKS_H_
#define _SEEDWIDGETS_CALLBACKS_H_

#include <functional>

#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include <vtkCommand.h>
#include <vtkImageActor.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkPointHandleRepresentation3D.h>
#include "vtkProperty.h"
#include <vtkPolyDataMapper.h>
#include <vtkProperty2D.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWidgetEvent.h>
#include <vtkSeedRepresentation.h>
#include <vtkSmartPointer.h>
#include <vtkTransformCoordinateSystems.h>

#include "vtkSeedWidgetx.h"
#include "module_config.h"


class vtkSeedImageCallback : public vtkCommand //,public vtkObject
{
public:
	static vtkSeedImageCallback *New()
	{return new vtkSeedImageCallback;}
	vtkSeedImageCallback() {}

	virtual void Execute(vtkObject*, unsigned long event, void *calldata);
	void SetFrontalRepresentation(vtkSmartPointer<vtkSeedRepresentation> rep);
	void SetFrontalWidget(vtkSmartPointer<vtkSeedWidgetx> widget);
	void SetProfileRepresentation(vtkSmartPointer<vtkSeedRepresentation> rep);
	void SetProfileWidget(vtkSmartPointer<vtkSeedWidgetx> widget);

private:
	vtkSeedRepresentation* m_FrontalSeedRepresentation;
	vtkSeedWidgetx* m_FrontalSeedWidget;
	vtkSeedRepresentation* m_ProfileSeedRepresentation;
	vtkSeedWidgetx* m_ProfileSeedWidget;
	
	std::vector<std::vector<double> > m_coordinate;
};


enum View_Direction
{
	Frontal,
	Profile
};

class seedwidgets_man : public vtkObject
{
public:
	static seedwidgets_man *New();
	vtkTypeMacro(seedwidgets_man, vtkObject);

	void SetInteractor(vtkRenderWindowInteractor* );
	void Enable();
	void Disable();

	void SetSeedWorldPosition(unsigned int seedID, double pos[3]);
	void SetDirection(View_Direction);        // set after
	void SetCallBack(vtkSeedImageCallback*);  // set first
	void AddSeed(double pos[3]);


protected:
	seedwidgets_man();
	~seedwidgets_man();

private:
	vtkSmartPointer<vtkSeedWidgetx> m_seedWidget;
	vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
	vtkSmartPointer<vtkSeedRepresentation>  m_seedRepresentation;
	vtkSmartPointer<vtkPointHandleRepresentation3D> m_handle;
	View_Direction m_direction = Frontal;
	vtkSmartPointer<vtkSeedImageCallback> seedCallback;
};






#endif // !_SEEDWIDGETS_CALLBACKS_H_

