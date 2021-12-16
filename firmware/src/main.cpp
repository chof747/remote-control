#include <Arduino.h>
#include "logger.h"
#include "component/component.h"
#include "component/status.h"
#include "component/wifi.h"
#include "component/mqtt.h"
#include "component/button_controls.h"
#include "component/display.h"

#include "controller/base.h"
#include "controller/idle_controller.h"
#include "controller/remote_controller.h"
#include "controller/device_data_controller.h"
#include "controller/idle_controller.h"

#define MAX_COMPONENTS 5

Component *components[MAX_COMPONENTS];
ControllerBase *activeController = NULL;

void onNextController(ControllerBase *active)
//****************************************************************************************
{
  activeController = active;
}

IdleController idle(&onNextController);
RemoteController remote(&buttonControls, &idle, &onNextController);
DeviceDataController deviceData(&buttonControls, &idle, &onNextController);

void setup()
//****************************************************************************************
{
  // put your setup code here, to run once:
  Log.info("MAIN", "Starting");

  components[0] = &statusComponent;
  components[1] = &display;
  components[2] = &buttonControls;
  components[3] = &wifiManager;
  components[4] = &mqttClient;

  for (int i = 0; i < MAX_COMPONENTS; ++i)
  {
    if (components[i]->powerOnTest())
    {
      if (!components[i]->setup())
      {
        display.loop();
        delay(ERR_MSG_TIME);
        idle.activate();
      }
    }
  }

  for (int i = 0; i < MAX_COMPONENTS; ++i)
  {
    components[i]->afterSetup();
  }

  mqttClient.stat(MQTT_ACTIVATION_MSG, "active", true);
  remote.setNext(&deviceData);
  deviceData.setNext(&remote);

  activeController = &remote;
  activeController->activate();
}

void loop()
//****************************************************************************************
{
  // put your main code here, to run repeatedly:
  for (int i = 0; i < MAX_COMPONENTS; ++i)
  {
    components[i]->loop();
  }

  //do any work in the currently active controller
  if (NULL != activeController)
  {
    activeController->loop();
  }
}