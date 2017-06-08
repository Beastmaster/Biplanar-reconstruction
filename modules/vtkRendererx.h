
#ifndef _VTKRENDERER_H_
#define _VTKRENDERER_H_




#include "vtkObject.h"
#include "vtkSmartPointer.h"
#include "vtkRenderer.h"
#include "vtkActor.h"
#include "vtkRenderWindow.h"
#include "module_config.h"


// actor management
class vtkRendererx : public vtkObject
{
public:
	vtkTypeMacro(vtkRendererx, vtkRenderer);
	static vtkRendererx *New();

	void SetRenderWindow(vtkRenderWindow*);
	void SetRenderer(vtkRenderer*);
	void SetActor(int id, vtkActor*);
	void AddActor(vtkActor*);
	//void AddActor(vtkActor*); inhert from vtkRenderer
	void RemoveActor(int id); //inhert from vtkrenderer
	vtkActor* GetActor(int id);
	vtkRenderer* GetRenderer();
	int GetNumberOfActors();
	void ResetCamera();

	void Render();

private:
	vtkRendererx();
	~vtkRendererx() {};

	vtkSmartPointer<vtkRenderer> m_renderer;
	vtkSmartPointer<vtkRenderWindow> m_win;
};








#endif // !_VTKRENDERER_H_

