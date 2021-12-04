#include "controller/remote_controller.h"
#include "component/button_controls.h"
#include "logger.h"
#include "component/display.h"
#include "component/mqtt.h"
#include "config.h"


#define MODULE "MSG_CTRL"
#define READY_MESSAGE "Bereit ..."
#define ACTIVATION_MESSAGE "Aktivierung Knopf %d"


RemoteController::RemoteController(ButtonControls* buttons,  ControllerBase* timeout, activation_cb_t cb)
:ControllerBase(cb), ButtonController(buttons), TimeOutController(DEEP_SLEEP_AFTER),
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

int RemoteController::getButton(btnStateType state)
//*********************************************************************************
{
    int button = (state.pushButtons[BUTTON_COUNT - 2]) ? 10 : 0;
    int i = 0;
    do {
        if (state.pushButtons[i])
        {
            button += i + 1;
            break;
        }

        ++i;
            
    } while (i < (BUTTON_COUNT - 2));

    return button;
}

void RemoteController::onClick(btnStateType state)
//*********************************************************************************
{
   restartTimeout();

   if (!state.modeButton)
   {
       int btn = getButton(state);
    
       display.clear();
       display.printto(1, ACTIVATION_MESSAGE, btn);

       char btntopic[6];
       sprintf(btntopic, "BTN%02d", btn);
       mqttClient.stat(btntopic, "TOGGLE");

   }

   startTimer(PRESS_MSG_TIME);

}