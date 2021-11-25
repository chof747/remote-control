#include "controller/base.h"

ControllerBase::ControllerBase(activation_cb_t cb): 
  next(NULL), alternateNext(NULL), activationCallBack(cb) {}

void ControllerBase::setNext(ControllerBase* c)
//****************************************************************************************
{
    this->next = c;
}

void ControllerBase::setAlternateNext(ControllerBase* c)
//****************************************************************************************
{
    this->alternateNext = c;
}

void ControllerBase::gotoNext()
//****************************************************************************************
{
    gotoController(next);
}

void ControllerBase::gotoAlternateNext()
//****************************************************************************************
{
    gotoController(alternateNext);
}

void ControllerBase::gotoController(ControllerBase* controller)
//****************************************************************************************
{
    if (NULL != controller)
    {
        controller->activate();
        activationCallBack(controller);
    }
}