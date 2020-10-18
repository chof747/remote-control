#include "firmware.h" 
#include "config.h"
#include "mqtt.h"
#include "Arduino.h"

#include <ESP8266WiFi.h>

FirmwareController::FirmwareController(Adafruit_SSD1306 *display, uint8_t mode) : 
    ModeController::ModeController(display, mode) {

        enableDisplay(true);

}

void FirmwareController::onMessage(const char* topic, const char* command, const char* message) 
{
    //Serial.printf("Topic: %s, Command: %s, Message: %s\n", topic, command, message);
    if ((strcmp(topic, "cmnd") == 0) && 
        (strcmp(command, "status") == 0) && 
        (strcmp(message, "5") == 0)) 
    {
        provideDeviceInfo();
    }
}

void FirmwareController::onActivation() 
{
    #ifdef SERIAL_PRINT
    Serial.println("Activate Firmware Controller...");
    #endif
    
    updateDisplay = true;
}

const char* FirmwareController::showLine1() 
{
    return "Firmware: ";
}

void FirmwareController::provideDeviceInfo() 
//*********************************************************************************
{
    /**
     * 
     * Mac - mac Address
     * IPAddress - ip Address
     * Version - firmware version + firmware name in paranthesis (e.g. 1.0.1(remote-control))
     * DeviceName - IT friendly name of the device
     * FriendlyName - descriptive name
     * Hardware - hardware type 
     * 
     */

    sprintf(gMqttMessageBuffer, 
      "{\"device\":{\"Mac\":\"%s\",\"IPAddress\":\"%s\",\"Version\":\"%s\",\"DeviceName\":\"%s\",\"FriendlyName\":\"%s\",\"Hardware\":\"%s\"}}",
      WiFi.macAddress().c_str(),
      WiFi.localIP().toString().c_str(),
      VERSION,
      DEVICE_NAME,
      "Remote Control Prototype",
      "ESP8266EX"
      );

    //Serial.println(gMqttMessageBuffer);

    mqttPublish(MQTT_STATS_TOPIC, "STATUS5");
}