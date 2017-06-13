/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/6

Description:
	3d visualization of spine model

*/


#ifndef _MODEL3D_VIEWER_H_
#define _MODEL3D_VIEWER_H_

#include "vtkObject.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCallbackCommand.h"

#include <vector>

#include "vtkRendererx.h"
#include "module_config.h"
#include "globalEventCallback.h"


class model3d_viewer: public vtkObject
{
public:
	static model3d_viewer *New();
	vtkTypeMacro(model3d_viewer, vtkObject);

	void SetRenderWindow(vtkRenderWindow* win); // install vis pipline after set
	void SetCallback(globalEventCallback*);
	
	void AddActor(vtkActor*);
	vtkActor* GetActor(int id);
	
	void Render();
	void ResetCamera();
	vtkSmartPointer<vtkRenderWindow> GetRenderWindow();
	vtkSmartPointer<vtkRendererx> GetRendererx();

private:
	model3d_viewer();
	~model3d_viewer();

	vtkSmartPointer<vtkRenderWindow> m_renWin;
	vtkSmartPointer<vtkRendererx>     m_renderer;
	vtkSmartPointer<globalEventCallback> m_seed_callback;
};






#endif // !_MODEL3D_VIEWER_H_

