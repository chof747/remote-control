#include "controller/idle_controller.h"
#include "logger.h"
#include "component/button_controls.h"
#include "component/display.h"
#include "component/mqtt.h"

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
    mqttClient.stat(MQTT_ACTIVATION_MSG, "inactive", true);

    buttonControls.activateWakeUp();
    display.turnOff();

    delay(10);
    ESP.deepSleep(0);
}

void IdleController::loop()
//*********************************************************************************
{

}