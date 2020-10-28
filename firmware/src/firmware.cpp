#include "firmware.h" 
#include "config.h"
#include "mqtt.h"
#include "Arduino.h"
#include <ESP8266WiFi.h> 
#include <ESP8266httpUpdate.h>


#include <ESP8266WiFi.h>

#define NEXT_INFO_BTN 1
#define SEND_STATUS_BTN 2

FirmwareController::FirmwareController(Adafruit_SSD1306 *display, uint8_t mode) : 
    ModeController::ModeController(display, mode) 
//*********************************************************************************
{

        enableDisplay(true);

        mode = 0;
        strcpy(caption,"");
        strcpy(value,"");
        setLines();

}

void FirmwareController::onMessage(const char* topic, const char* command, const char* message) 
//*********************************************************************************
{
    Serial.printf("Topic: %s, Command: %s, Message: %s\n", topic, command, message);
    if ((0 == strcmp(topic, "CMND")) && 
        (0 == strcmp(command, "STATUS")) && 
        (0 == strcmp(message, "5")))
    {
        provideDeviceInfo();
    } 
    else if ((0 == strcmp(topic, "CMND")) && 
        (0 == strcmp(command, "UPGRADE")))
    {

        if (0 == strncmp(message, "http://", 7))
        {
            performOTAUpdate(message);
        }
        else if (0 == strncmp(message, "1", 1))
        {
            performOTAUpdate(DEFAULT_OTA_URL);
        }
        else
        {
            sprintf(gMqttMessageBuffer, "Error: no Update URL given!");
            mqttPublish(MQTT_STATS_TOPIC,"UPGRADE");
        }
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
//*********************************************************************************
{
    return caption;
}

const char* FirmwareController::showLine2() 
//*********************************************************************************
{
    return value;
}

bool FirmwareController::handleButton(uint8_t btn) 
//*********************************************************************************
{
    if (NEXT_INFO_BTN == btn)
    {
        ++mode;
        if(3 < mode)
        {
            mode = 0;
        }

        setLines();
    }
    else if (SEND_STATUS_BTN == btn) 
    {
        provideDeviceInfo();
    }

    return true;
}

void FirmwareController::setLines()
//*********************************************************************************
{
    switch(mode) 
    {
        case 0:
            strcpy(caption, "Firmware:");
            strncpy(value, VERSION,19) ;
            break;

        case 1:
            strcpy(caption, "IP Address:");
            strncpy(value, WiFi.localIP().toString().c_str(),19);
            break;

        case 2:
            strcpy(caption, "MAC Addr.:");
            strncpy(value,WiFi.macAddress().c_str(), 19);
            break;
        case 3:
            strcpy(caption, "Name:");
            strncpy(value,DEVICE_NAME, 19);
            break;
    }
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

void FirmwareController::performOTAUpdate(const char* url) 
//*********************************************************************************
{
    t_httpUpdate_return ret =  ESPhttpUpdate.update(url, VERSION);
    switch(ret) 
    {
        case HTTP_UPDATE_FAILED:
            sprintf(gMqttMessageBuffer, "[update] Update failed from %s", url);
            break;
        case HTTP_UPDATE_NO_UPDATES:
            sprintf(gMqttMessageBuffer, "[update] No Update for version %s", VERSION);
            break;
        case HTTP_UPDATE_OK:
            strcpy(gMqttMessageBuffer, "[update] Update ok."); // may not be called since we reboot the ESP
            break;
    }

    #ifdef SERIAL_PRINT
    Serial.println(gMqttMessageBuffer);
    #endif

    mqttPublish(MQTT_STATS_TOPIC, "UPGRADE");
}