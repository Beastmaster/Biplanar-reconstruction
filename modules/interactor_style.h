/*
Author: QIN Shuo
Date:   2017/6/2
Description:
	Custom modified interactor style
*/

#ifndef _INTERACTOR_STYLE_H_
#define _INTERACTOR_STYLE_H_




#include "vtkInteractorStyle.h"
#include "vtkSmartPointer.h"
#include "vtkObjectFactory.h"
#include "vtkAlgorithm.h"

class interactor_style: public vtkInteractorStyle
{
public:
	static interactor_style *New();
	vtkTypeMacro(interactor_style, vtkInteractorStyle);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Event bindings controlling the effects of pressing mouse buttons
	// or moving the mouse.
	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnMiddleButtonDown();
	virtual void OnMiddleButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
	virtual void OnChar();

	// These methods for the different interactions in different modes
	// are overridden in subclasses to perform the correct motion. Since
	// they are called by OnTimer, they do not have mouse coord parameters
	// (use interactor's GetEventPosition and GetLastEventPosition)
	virtual void Rotate();
	virtual void Spin();
	virtual void Pan();
	virtual void Dolly();

	// Description:
	// Set the apparent sensitivity of the interactor style to mouse motion.
	vtkSetMacro(MotionFactor, double);
	vtkGetMacro(MotionFactor, double);

protected:
	interactor_style();
	~interactor_style();

	double MotionFactor;

	virtual void Dolly(double factor);

private:
	interactor_style(const interactor_style&);  // Not implemented.
	void operator=(const interactor_style&);  // Not implemented.

};




#endif // !_INTERACTOR_STYLE_H_