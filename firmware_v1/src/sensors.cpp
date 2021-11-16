#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include "sensors.h"
#include "config.h"

#define NEXT_SENSOR_BTN 1
#define REMOTE_MODE_BTN 2

WiFiClient esp_client;

void rtrim(char* s, int length) 
//*********************************************************************************
{
    char* ptr = s + length - 1;
    while (*ptr == ' ')
    {
        --ptr;
    }

    *(++ptr) = '\0';
}

SensorMonitor::SensorMonitor(Adafruit_SSD1306 *display, uint8_t mode) : 
    ModeController::ModeController(display, mode)
//*********************************************************************************
{
    enableDisplay(true);
    sensorSwitch = false;
    strcpy(lines[0], "Sensoren");
    strcpy(lines[1], "1 - naechster");
    strcpy(lines[2], "");
}


void SensorMonitor::readSensor()
//*********************************************************************************
{
    HTTPClient client;
    client.begin(esp_client, SENSOR_URL);
    int httpCode = client.GET();
    Serial.printf("HTTP-Return Code: %d", httpCode);
    if (httpCode == 200)
    {
        char buf[161];
        strncpy(buf,client.getString().c_str(), 161);

        char* p = buf;

        int i=0;
        char* str = strtok(p, "\n");
        while((str != NULL) && (i<3))
        {
            strcpy(lines[i], str);
            i++;
            str = strtok(NULL, "\n");
        }

    }
}

bool SensorMonitor::handleButton(uint8_t btn)
//*********************************************************************************
{

#ifdef SERIAL_PRINT
    Serial.printf("Button: %d", btn);
#endif

    if (btn == NEXT_SENSOR_BTN)
    {
       needExecution();
    }

    return true;
}

bool SensorMonitor::onExecution()
//*********************************************************************************
{


    readSensor();

#ifdef SERIAL_PRINT
    Serial.printf("Reading  Sensor: %s, value: %s\n", lines[0], lines[1]);
#endif

    return false;
}

void SensorMonitor::onActivation()
//*********************************************************************************
{
#ifdef SERIAL_PRINT
    Serial.println("Activating Sensor Reading Mode ...");
#endif

    updateDisplay = true;
    sensorSwitch = false;
}

const char *SensorMonitor::showLine1()
//*********************************************************************************
{
    return lines[0];
}

const char *SensorMonitor::showLine2()
//*********************************************************************************
{
    return lines[1];
}

const char *SensorMonitor::showLine3()
//*********************************************************************************
{
    return lines[2];
}