#include "controller/remote_controller.h"
#include "component/button_controls.h"
#include "logger.h"
#include "component/display.h"
#include "config.h"


#define MODULE "MSG_CTRL"
#define READY_MESSAGE "Bereit ..."
#define ACTIVATION_MESSAGE "Aktivierung Knopf ..."


RemoteController::RemoteController(ButtonControls* buttons,  ControllerBase* timeout, activation_cb_t cb)
:ControllerBase(cb), ButtonController(buttons), TimeOutController(CONTROLLER_TIMEOUT),
timeoutTarget(timeout)
//*********************************************************************************
{
  
}

void RemoteController::activate()
//*********************************************************************************
{
    Log.debug(MODULE, "Message Controller activated");

    attach();
    display.clear();
    display.printto(1, READY_MESSAGE);
    restartTimeout();
}

void RemoteController::loop()
//*********************************************************************************
{
    if (checkTimeForTimeOut()) 
    {
        gotoController(timeoutTarget);
    }

    if (checkTimeForTimer())
    {
        display.clear();
        display.printto(1, READY_MESSAGE);
    }
}

void RemoteController::onClick(btnStateType state)
//*********************************************************************************
{
   restartTimeout();

   String buttonspressed = "";

    for (int i = 0; i < BUTTON_COUNT - 1; ++i)
    {
        if (state.pushButtons[i]) 
        {
            buttonspressed +=  i + 1;
            buttonspressed += ", ";
        }
    }

    if (state.modeButton)
    {
        buttonspressed += "M, ";
    }

    if (buttonspressed.length() > 1) 
    {
        int lastIndex = buttonspressed.length() - 1;
        buttonspressed.remove(lastIndex);
        buttonspressed.remove(lastIndex - 1);
    }

   startTimer(5000);
   display.clear();
   display.printto(1, ACTIVATION_MESSAGE);
   display.printto(2, buttonspressed.c_str());

}