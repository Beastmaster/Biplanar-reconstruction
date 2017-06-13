/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/6

Description:
	Callback fucntion, seed widget management
*/



#ifndef _seedImageCallback_h_
#define _seedImageCallback_h_


#include <functional>

#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkCommand.h"
#include "vtkImageActor.h"
#include "vtkPointHandleRepresentation2D.h"
#include "vtkPointHandleRepresentation3D.h"
#include "vtkProperty.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty2D.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkWidgetEvent.h"
#include "vtkSeedRepresentation.h"
#include "vtkSmartPointer.h"
#include "vtkTransformCoordinateSystems.h"
#include "vtkRenderer.h"

class vtkSeedWidgetx;

class globalEventCallback : public vtkCommand //,public vtkObject
{
public:
	static globalEventCallback *New()
	{
		return new globalEventCallback;
	}
	globalEventCallback() {};

	virtual void Execute(vtkObject*, unsigned long event, void *calldata);
	void SetFrontalWidget(vtkSeedWidgetx *);
	void SetProfileWidget(vtkSeedWidgetx *);
	void Set3dWindow(vtkRenderWindow*);

private:
	vtkSeedWidgetx* m_FrontalSeedWidget;
	vtkSeedWidgetx* m_ProfileSeedWidget;

	vtkRenderer*     m_3d_renderer;
	vtkRenderWindow* m_3d_renWin;

	std::vector<std::vector<double> > m_coordinate;
};


#endif // !_seedImageCallback_h_



