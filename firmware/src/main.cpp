#include <Arduino.h>
#include "logger.h"
#include "component/component.h"
#include "component/wifi.h"
#include "component/mqtt.h"
#include "component/button_controls.h"
#include "component/display.h"

#include "controller/base.h"
#include "controller/idle_controller.h"
#include "controller/remote_controller.h"

#define MAX_COMPONENTS 4

Component* components[MAX_COMPONENTS];
ControllerBase* activeController = NULL;

void onNextController(ControllerBase* active)
//****************************************************************************************
{
  activeController = active;
}

IdleController idle(&onNextController);
RemoteController remote(&buttonControls,&idle, &onNextController);


void setup() 
//****************************************************************************************
{
  // put your setup code here, to run once:
  Log.info("MAIN", "Starting");

  components[0] = &wifiManager;
  components[1] = &mqttClient;
  components[2] = &buttonControls;
  components[3] = &display;

  for(int i=0;i<MAX_COMPONENTS;++i)
  {
    if (components[i]->powerOnTest())
    {
      components[i]->setup();
    }
  }

  for(int i=0;i<MAX_COMPONENTS;++i)
  {
    components[i]->afterSetup();
  }

  activeController = &remote;
  activeController->activate();
}

void loop() 
//****************************************************************************************
{
  // put your main code here, to run repeatedly:
  for(int i=0;i<MAX_COMPONENTS;++i)
  {
    components[i]->loop();
  }

  //do any work in the currently active controller
  if (NULL != activeController)
  {
    activeController->loop();
  }
}