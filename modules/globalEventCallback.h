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

//#include "vtkSeedWidgetx.h"
#include "vtkRendererx.h"

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
	void Set3dRenderer(vtkRendererx*);

private:
	vtkSeedWidgetx* m_FrontalSeedWidget;
	vtkSeedWidgetx* m_ProfileSeedWidget;

	vtkRendererx* m_3d_renderer;

	std::vector<std::vector<double> > m_coordinate;
};


#endif // !_seedImageCallback_h_



