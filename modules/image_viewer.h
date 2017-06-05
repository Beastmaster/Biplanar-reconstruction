/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/3

*/



#ifndef _IMAGE_VIEWER_H__
#define _IMAGE_VIEWER_H__


#include "vtkObject.h"
#include "vtkImageMapper.h" // For all the inline methods
#include "vtkRenderWindow.h" // For all the inline methods
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkImageData.h"
#include "vtkImageMapToWindowLevelColors.h"

#include "module_config.h"
#include "interactor_style.h"
#include "seedwidget.h"




class image_viewer :public vtkObject
{
public:
	static image_viewer *New();
	vtkTypeMacro(image_viewer, vtkObject);

	void SetFrontalWindow(vtkRenderWindow*);
	void SetProfileWindow(vtkRenderWindow*);
	void Set3dWindow(vtkRenderWindow*);

	void SetFrontalImage(vtkImageData*);
	void SetProfileImage(vtkImageData*);

	void Render();

	void EnableSeedWidgets();
	void DisableSeedWidgets();
	
	void SetFrontalSeedPos(unsigned int seedID, double pos[3]);
	void AddFrontalSeed(double pos[3]);

private:
	image_viewer();
	~image_viewer();

	vtkSmartPointer<vtkImageData> m_FrontalImage;
	vtkSmartPointer<vtkImageData> m_ProfileImage;
	
	vtkSmartPointer<vtkImageMapToWindowLevelColors> m_frontal_windowlevel;
	vtkSmartPointer<vtkImageMapToWindowLevelColors> m_profile_windowlevel;

	vtkSmartPointer<vtkImageActor> m_frontal_actor;
	vtkSmartPointer<vtkImageActor> m_profile_actor;

	vtkSmartPointer<vtkRenderWindow> m_frontal_win;
	vtkSmartPointer<vtkRenderWindow> m_profile_win;
	vtkSmartPointer<vtkRenderWindow> m_3d_win;


	vtkSmartPointer<vtkRenderer> m_frontal_renderer;
	vtkSmartPointer<vtkRenderer> m_profile_renderer;
	vtkSmartPointer<vtkRenderer> m_3d_renderer;

	vtkSmartPointer<seedwidgets_man> m_frontal_seeds;
	vtkSmartPointer<seedwidgets_man> m_profile_seeds;

	vtkSmartPointer<vtkSeedImageCallback> m_seeds_callback;

};









#endif // !1



