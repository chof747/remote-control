#include "controller/button_controller.h"
#include "component/button_controls.h"

#include "logger.h"

#define MODULE "BTNCTRL"

ButtonController::ButtonController(ButtonControls *buttons)
//*********************************************************************************
{
    this->buttons = buttons;
}

void ButtonController::attach()
//*********************************************************************************
{
    buttons->setController(this);
}