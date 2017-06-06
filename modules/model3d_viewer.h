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
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCallbackCommand.h"


#include "module_config.h"

class model3d_viewer: public vtkObject
{
public:
	static model3d_viewer *New();
	vtkTypeMacro(model3d_viewer, vtkObject);

	void SetRenderWindow(vtkRenderWindow* win);


private:
	model3d_viewer();
	~model3d_viewer();

	vtkSmartPointer<vtkRenderWindow> m_renWin;
	vtkSmartPointer<vtkRenderer> m_renderer;	
};






#endif // !_MODEL3D_VIEWER_H_

