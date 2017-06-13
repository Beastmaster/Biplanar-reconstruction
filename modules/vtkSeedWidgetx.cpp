/*=========================================================================
Author: QIN Shuo
Date: 2017/6/6
Reference:
	vtkSeedWidget.h/cxx
Description:
	Duplication of vtkSeedWidget class, add vtkCommand::DeleteEvent signal

=========================================================================*/
#include "vtkCallbackCommand.h"
#include "vtkCommand.h"
#include "vtkCoordinate.h"
#include "vtkEvent.h"
#include "vtkHandleRepresentation.h"
#include "vtkHandleWidget.h"
#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSeedRepresentation.h"
#include "vtkWidgetCallbackMapper.h"
#include "vtkWidgetEvent.h"

#include <iterator>
#include <list>

#include "vtkSeedWidgetx.h"

vtkStandardNewMacro(vtkSeedWidgetx);

// The vtkSeedList is a PIMPLed list<T>.
class vtkSeedList : public std::list<vtkHandleWidget*> {};
typedef std::list<vtkHandleWidget*>::iterator vtkSeedListIterator;

//----------------------------------------------------------------------
vtkSeedWidgetx::vtkSeedWidgetx()
{
	this->ManagesCursor = 1;
	this->WidgetState = vtkSeedWidgetx::Start;

	// The widgets for moving the seeds.
	this->Seeds = new vtkSeedList;

	// These are the event callbacks supported by this widget
	this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonPressEvent,
		vtkWidgetEvent::AddPoint,
		this, vtkSeedWidgetx::AddPointAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonPressEvent,
		vtkWidgetEvent::Completed,
		this, vtkSeedWidgetx::CompletedAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::MouseMoveEvent,
		vtkWidgetEvent::Move,
		this, vtkSeedWidgetx::MoveAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonReleaseEvent,
		vtkWidgetEvent::EndSelect,
		this, vtkSeedWidgetx::EndSelectAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::KeyPressEvent,
		vtkEvent::NoModifier, 127, 1, "Delete",
		vtkWidgetEvent::Delete,
		this, vtkSeedWidgetx::DeleteAction);
	this->Defining = 1;

#pragma region CUSTOM_DEFINE
	m_direction = Frontal;
#pragma endregion CUSTOM_DEFINE
}

//----------------------------------------------------------------------
void vtkSeedWidgetx::DeleteSeed(int i)
{
	if (this->Seeds->size() <= static_cast< size_t >(i))
	{
		return;
	}

	vtkSeedRepresentation *rep =
		static_cast<vtkSeedRepresentation*>(this->WidgetRep);
	if (rep)
	{
		rep->RemoveHandle(i);
	}

	vtkSeedListIterator iter = this->Seeds->begin();
	std::advance(iter, i);
	(*iter)->SetEnabled(0);
	(*iter)->RemoveObservers(vtkCommand::StartInteractionEvent);
	(*iter)->RemoveObservers(vtkCommand::InteractionEvent);
	(*iter)->RemoveObservers(vtkCommand::EndInteractionEvent);
	vtkHandleWidget * w = (*iter);
	this->Seeds->erase(iter);
	w->Delete();
}

//----------------------------------------------------------------------
vtkSeedWidgetx::~vtkSeedWidgetx()
{
	// Loop over all seeds releasing their observers and deleting them
	while (!this->Seeds->empty())
	{
		this->DeleteSeed(static_cast<int>(this->Seeds->size()) - 1);
	}
	delete this->Seeds;
}

//----------------------------------------------------------------------
vtkHandleWidget * vtkSeedWidgetx::GetSeed(int i)
{
	if (this->Seeds->size() <= static_cast< size_t >(i))
	{
		return NULL;
	}
	vtkSeedListIterator iter = this->Seeds->begin();
	std::advance(iter, i);
	return *iter;
}

//----------------------------------------------------------------------
void vtkSeedWidgetx::CreateDefaultRepresentation()
{
	if (!this->WidgetRep)
	{
		this->WidgetRep = vtkSeedRepresentation::New();
	}
}

//----------------------------------------------------------------------
void vtkSeedWidgetx::SetEnabled(int enabling)
{
	this->Superclass::SetEnabled(enabling);

	vtkSeedListIterator iter;
	for (iter = this->Seeds->begin(); iter != this->Seeds->end(); ++iter)
	{
		(*iter)->SetEnabled(enabling);
	}

	if (!enabling)
	{
		this->RequestCursorShape(VTK_CURSOR_DEFAULT);
		this->WidgetState = vtkSeedWidgetx::Start;
	}

	this->Render();
}

// The following methods are the callbacks that the seed widget responds to.
//-------------------------------------------------------------------------
void vtkSeedWidgetx::AddPointAction(vtkAbstractWidget *w)
{
	vtkSeedWidgetx *self = reinterpret_cast<vtkSeedWidgetx*>(w);

	// Need to distinguish between placing handles and manipulating handles
	if (self->WidgetState == vtkSeedWidgetx::MovingSeed)
	{
		return;
	}

	// compute some info we need for all cases
	int X = self->Interactor->GetEventPosition()[0];
	int Y = self->Interactor->GetEventPosition()[1];

	// When a seed is placed, a new handle widget must be created and enabled.
	int state = self->WidgetRep->ComputeInteractionState(X, Y);
	if (state == vtkSeedRepresentation::NearSeed)
	{
		self->WidgetState = vtkSeedWidgetx::MovingSeed;

		// Invoke an event on ourself for the handles
		self->InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);
		self->Superclass::StartInteraction();
		self->InvokeEvent(vtkCommand::StartInteractionEvent, NULL);

		self->EventCallbackCommand->SetAbortFlag(1);
		self->Render();
	}

	else if (self->WidgetState != vtkSeedWidgetx::PlacedSeeds)
	{
		// we are placing a new seed. Just make sure we aren't in a mode which
		// dictates we've placed all seeds.

		self->WidgetState = vtkSeedWidgetx::PlacingSeeds;
		double e[3]; e[2] = 0.0;
		e[0] = static_cast<double>(X);
		e[1] = static_cast<double>(Y);

		vtkSeedRepresentation *rep =
			reinterpret_cast<vtkSeedRepresentation*>(self->WidgetRep);
		// if the handle representation is constrained, check to see if
		// the position follows the constraint.
		if (!rep->GetHandleRepresentation()->CheckConstraint(
			self->GetCurrentRenderer(), e))
		{
			return;
		}
		int currentHandleNumber = rep->CreateHandle(e);
		vtkHandleWidget *currentHandle = self->CreateNewHandle();
		rep->SetSeedDisplayPosition(currentHandleNumber, e);
		currentHandle->SetEnabled(1);
		self->InvokeEvent(vtkCommand::PlacePointEvent, &(currentHandleNumber));
		self->InvokeEvent(vtkCommand::InteractionEvent, &(currentHandleNumber));

		self->EventCallbackCommand->SetAbortFlag(1);
		self->Render();
	}

}

//-------------------------------------------------------------------------
void vtkSeedWidgetx::CompletedAction(vtkAbstractWidget *w)
{
	vtkSeedWidgetx *self = reinterpret_cast<vtkSeedWidgetx*>(w);

	// Do something only if we are in the middle of placing the seeds
	if (self->WidgetState == vtkSeedWidgetx::PlacingSeeds)
	{
		self->CompleteInteraction();
	}
}

//-------------------------------------------------------------------------
void vtkSeedWidgetx::CompleteInteraction()
{
	this->WidgetState = vtkSeedWidgetx::PlacedSeeds;
	this->EventCallbackCommand->SetAbortFlag(1);
	this->Defining = 0;
}

//-------------------------------------------------------------------------
void vtkSeedWidgetx::RestartInteraction()
{
	this->WidgetState = vtkSeedWidgetx::Start;
	this->Defining = 1;
}

//-------------------------------------------------------------------------
void vtkSeedWidgetx::MoveAction(vtkAbstractWidget *w)
{
	vtkSeedWidgetx *self = reinterpret_cast<vtkSeedWidgetx*>(w);

	// Do nothing if outside
	if (self->WidgetState == vtkSeedWidgetx::Start)
	{
		return;
	}

	// else we are moving a seed

	self->InvokeEvent(vtkCommand::MouseMoveEvent, NULL);

	// set the cursor shape to a hand if we are near a seed.
	int X = self->Interactor->GetEventPosition()[0];
	int Y = self->Interactor->GetEventPosition()[1];
	int state = self->WidgetRep->ComputeInteractionState(X, Y);

	// Change the cursor shape to a hand and invoke an interaction event if we
	// are near the seed
	if (state == vtkSeedRepresentation::NearSeed)
	{
		self->RequestCursorShape(VTK_CURSOR_HAND);

		vtkSeedRepresentation *rep = static_cast<
			vtkSeedRepresentation * >(self->WidgetRep);
		int seedIdx = rep->GetActiveHandle();
		self->InvokeEvent(vtkCommand::InteractionEvent, &seedIdx);

		self->EventCallbackCommand->SetAbortFlag(1);
	}
	else
	{
		self->RequestCursorShape(VTK_CURSOR_DEFAULT);
	}

	self->Render();
}

//-------------------------------------------------------------------------
void vtkSeedWidgetx::EndSelectAction(vtkAbstractWidget *w)
{
	vtkSeedWidgetx *self = reinterpret_cast<vtkSeedWidgetx*>(w);

	// Do nothing if outside
	if (self->WidgetState != vtkSeedWidgetx::MovingSeed)
	{
		return;
	}

	// Revert back to the mode we were in prior to selection.
	self->WidgetState = self->Defining ?
		vtkSeedWidgetx::PlacingSeeds : vtkSeedWidgetx::PlacedSeeds;

	// Invoke event for seed handle
	self->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, NULL);
	self->EventCallbackCommand->SetAbortFlag(1);
	self->InvokeEvent(vtkCommand::EndInteractionEvent, NULL);
	self->Superclass::EndInteraction();
	self->Render();
}

//-------------------------------------------------------------------------
void vtkSeedWidgetx::DeleteAction(vtkAbstractWidget *w)
{
	vtkSeedWidgetx *self = reinterpret_cast<vtkSeedWidgetx*>(w);

	// Do nothing if outside
	if (self->WidgetState != vtkSeedWidgetx::PlacingSeeds)
	{
		return;
	}

	// Remove last seed
	vtkSeedRepresentation *rep =
		reinterpret_cast<vtkSeedRepresentation*>(self->WidgetRep);
	int removeId = rep->GetActiveHandle();
	if (removeId != -1)
	{
		rep->RemoveActiveHandle();
	}
	else
	{
		rep->RemoveLastHandle();
		removeId = static_cast<int>(self->Seeds->size()) - 1;
	}
	self->DeleteSeed(removeId);
	// Got this event, abort processing if it

	self->InvokeEvent(vtkCommand::DeleteEvent);
	self->EventCallbackCommand->SetAbortFlag(1);
	self->Render();
}

//----------------------------------------------------------------------
void vtkSeedWidgetx::SetProcessEvents(int pe)
{
	this->Superclass::SetProcessEvents(pe);

	vtkSeedListIterator iter = this->Seeds->begin();
	for (; iter != this->Seeds->end(); ++iter)
	{
		(*iter)->SetProcessEvents(pe);
	}
}

//----------------------------------------------------------------------
void vtkSeedWidgetx::SetInteractor(vtkRenderWindowInteractor *rwi)
{
	this->Superclass::SetInteractor(rwi);
	vtkSeedListIterator iter = this->Seeds->begin();
	for (; iter != this->Seeds->end(); ++iter)
	{
		(*iter)->SetInteractor(rwi);
	}
}

//----------------------------------------------------------------------
void vtkSeedWidgetx::SetCurrentRenderer(vtkRenderer *ren)
{
	this->Superclass::SetCurrentRenderer(ren);
	vtkSeedListIterator iter = this->Seeds->begin();
	for (; iter != this->Seeds->end(); ++iter)
	{
		if (!ren)
		{
			// Disable widget if its being removed from the the renderer
			(*iter)->EnabledOff();
		}
		(*iter)->SetCurrentRenderer(ren);
	}
}

//----------------------------------------------------------------------
// Programmatically create a new handle.
vtkHandleWidget * vtkSeedWidgetx::CreateNewHandle()
{
	vtkSeedRepresentation *rep =
		vtkSeedRepresentation::SafeDownCast(this->WidgetRep);
	if (!rep)
	{
		vtkErrorMacro(<< "Please set, or create a default seed representation "
			<< "before adding requesting creation of a new handle.");
		return NULL;
	}

	// Create the handle widget or reuse an old one
	int currentHandleNumber = static_cast<int>(this->Seeds->size());
	vtkHandleWidget *widget = vtkHandleWidget::New();

	// Configure the handle widget
	widget->SetParent(this);
	widget->SetInteractor(this->Interactor);
	vtkHandleRepresentation *handleRep = rep->GetHandleRepresentation(currentHandleNumber);
	if (!handleRep)
	{
		widget->Delete();
		return NULL;
	}
	else
	{
		handleRep->SetRenderer(this->CurrentRenderer);
		widget->SetRepresentation(handleRep);

		// Now place the widget into the list of handle widgets (if not already there)
		this->Seeds->push_back(widget);
		return widget;
	}
}

//----------------------------------------------------------------------
void vtkSeedWidgetx::PrintSelf(ostream& os, vtkIndent indent)
{
	//Superclass typedef defined in vtkTypeMacro() found in vtkSetGet.h
	this->Superclass::PrintSelf(os, indent);

	os << indent << "WidgetState: " << this->WidgetState << endl;
}

void vtkSeedWidgetx::SetSeedWorldPosition(unsigned int seedID, double pos[3])
{
	unsigned int total_seeds = this->GetSeedRepresentation()->GetNumberOfSeeds();
	if (total_seeds <= seedID)
		return;

	this->GetSeedRepresentation()->GetHandleRepresentation(seedID)->SetWorldPosition(pos);
	this->GetInteractor()->GetRenderWindow()->Render();
}

void vtkSeedWidgetx::SetDirection(View_Direction xx)
{
	m_direction = xx;
}

void vtkSeedWidgetx::SetCallBack(globalEventCallback * callback)
{
	m_seedCallback = callback;
	this->AddObserver(vtkCommand::PlacePointEvent, m_seedCallback);
	this->AddObserver(vtkCommand::InteractionEvent, m_seedCallback);
	this->AddObserver(vtkCommand::DeleteEvent, m_seedCallback);
}

void vtkSeedWidgetx::AddSeed(double pos[3])
{
	auto tmp_seed = this->CreateNewHandle();
	tmp_seed->GetHandleRepresentation()->SetWorldPosition(pos);
	tmp_seed->SetEnabled(1);
}

int vtkSeedWidgetx::GetSeedWorldPosition(unsigned int id, double * pos)
{
	if ( id >= this->GetSeedRepresentation()->GetNumberOfSeeds())
		return 1;
	this->GetSeedWorldPosition(id, pos);
	return 0;
}

View_Direction vtkSeedWidgetx::GetDirection()
{
	return m_direction;
}
