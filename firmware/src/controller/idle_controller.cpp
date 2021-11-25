#include "controller/idle_controller.h"
#include "logger.h"

#define MODULE "IDLE"

IdleController::IdleController(activation_cb_t cb): 
  ControllerBase(cb)
//*********************************************************************************
{
}

void IdleController::activate()
//*********************************************************************************
{
    Log.debug(MODULE, "Idle Controller activated");
}

void IdleController::loop()
//*********************************************************************************
{

}