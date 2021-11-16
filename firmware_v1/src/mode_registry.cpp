#include <Arduino.h>
#include "mode_registry.h"
#include <ESP8266WiFi.h> // ESP8266 WiFi driver
#include "Adafruit_SSD1306.h"

#include "remote.h"
#include "sensors.h"
#include "battery.h"
#include "firmware.h"
#include "mqtt.h"

uint8_t gCurrentMode;
ModeController** gModes;

void initializeModes(Adafruit_SSD1306* display) {

    gModes = new ModeController*[NUM_MODES];

    gModes[0] = new RemoteControl(display, 0);
    gModes[1] = new SensorMonitor(display, 1);
    gModes[2] = new BatteryControl(display, 2);
    gModes[3] = new FirmwareController(display, 2);

}

ModeController* currentMode() {
    return gModes[gCurrentMode];
}

uint8_t nextMode() {
    gCurrentMode++;
    if (gCurrentMode == NUM_MODES) {
        gCurrentMode = 0;
    }

    #ifdef SERIAL_PRINT
    Serial.printf("Switching to Mode %d\n", gCurrentMode);
    #endif

    gModes[gCurrentMode]->activate();

    return gCurrentMode;
}

char* uppercase(char* s) 
{
    char* c = s;
    while(0 != (int)*c)
    {
            *c = *c & ~(0x20);
            ++c;
    }

    return s;
}

void loopModes()
//******************************************************************************
{
   for(int i=0;i<NUM_MODES;++i) 
   {
        gModes[i]->onLoop();
   } 
}

void mqttCallback(char *topic, byte *payload, uint8_t length)
//******************************************************************************
{
    char message[length + 1];
    char command[50];

    memcpy(message, payload, length);
    message[length] = (char) 0;

    char * token = uppercase(strtok(topic, "/"));
    while( token != NULL) {
        strcpy(command,token);
        token = strtok(NULL,"/");
        uppercase(command);
    }
#ifdef SERIAL_PRINT
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("], command =  ");
    Serial.print(command);
    Serial.print(":");
    Serial.println(message);
#endif

    for(int i=0;i<NUM_MODES;++i) {
        gModes[i]->onMessage(topic, command, message);
    }
}

