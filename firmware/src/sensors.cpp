#include <Arduino.h>
#include <ESP8266HTTPClient.h>

#include "sensors.h"
#include "config.h"

#define NEXT_SENSOR_BTN 1
#define REMOTE_MODE_BTN 2

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

    sensors = NULL;
    nSensors = 0;

    currentSensorName = NULL;
    currentSensorReading = NULL;
    currentSensor = 0;

    sensorSwitch = false;
}

void SensorMonitor::clearSensors()
//*********************************************************************************
{
    if (sensors != NULL) 
    {
        delete sensors;
        for(int i=0;i<nSensors; ++i) 
        {
            delete values[i];
        }
        delete values;
    }
}

void SensorMonitor::createSensors(int count) 
//*********************************************************************************
{
    sensors = new sensor_t[count];
    values  = new char*[count];

    nSensors = count;
    Serial.printf("Count Sensors: %d\n", nSensors);
    for(int i=0; i<nSensors;++i) 
    {
        values[i] = new char[12];
    }
}

void SensorMonitor::obtainSensors()
//*********************************************************************************
{
    clearSensors();

    #ifdef SERIAL_PRINT
        Serial.println("Getting Sensor List ...");
    #endif

    HTTPClient client;
    client.begin(SENSOR_LIST_URL);
    int httpCode = client.GET(); //Send the request

    String payload = client.getString();
    const char* payload_ptr = payload.c_str();

    memcpy(&sensorMain, payload_ptr, sizeof(sensorMain));
    rtrim(sensorMain.prefix, 15);
    int count = atoi(sensorMain.number);

    #ifdef SERIAL_PRINT
        Serial.printf("Number of Sensors: %d\n", count);
    #endif



    createSensors(count);

    payload_ptr += sizeof(sensorMain);

    size_t s = sizeof(sensor_t);

    for(int i=0; i<count; ++i) 
    {
        memcpy(&sensors[i], payload_ptr, s);
        rtrim(sensors[i].name, 8);
        rtrim(sensors[i].path, 24);
    
    #ifdef SERIAL_PRINT
        Serial.printf("Sensor %d: %s from %s\n", i, sensors[i].name, sensors[i].path);
    #endif

        payload_ptr += s;
    }

    currentSensor = count - 1;
}

char *SensorMonitor::readSensor()
//*********************************************************************************
{
    HTTPClient client;
    char url[100];
    sprintf(url, "%s%s/%s", SENSOR_URL_BASE, sensorMain.prefix, sensors[currentSensor].path);

    client.begin(url);
    int httpCode = client.GET();
    Serial.printf("HTTP-Return Code: %d", httpCode);
    if (httpCode == 200)
    {
        strcpy(values[currentSensor], client.getString().c_str());
    }

    return values[currentSensor];
}

bool SensorMonitor::handleButton(uint8_t btn)
//*********************************************************************************
{

#ifdef SERIAL_PRINT
    Serial.printf("Button: %d", btn);
#endif

    if (btn == NEXT_SENSOR_BTN)
    {

        if (currentSensor < (nSensors - 1))
        {
            ++currentSensor;
        }
        else
        {
            currentSensor = 0;
        }
        
#ifdef SERIAL_PRINT
        Serial.printf("Switching to next sensor reading %d of %d!\n", currentSensor, nSensors);
#endif

        needExecution();
    }

    return true;
}

bool SensorMonitor::onExecution()
//*********************************************************************************
{

#ifdef SERIAL_PRINT
    Serial.printf("Reading  Sensor: %s\n", sensors[currentSensor].name);
#endif

    currentSensorName = sensors[currentSensor].name;
    currentSensorReading = readSensor();

    return false;
}

void SensorMonitor::onActivation()
//*********************************************************************************
{
#ifdef SERIAL_PRINT
    Serial.println("Activating Sensor Reading Mode ...");
#endif

    obtainSensors();

    updateDisplay = true;
    currentSensorReading = NULL;
    currentSensorName = NULL;
    sensorSwitch = false;
}

const char *SensorMonitor::showLine1()
//*********************************************************************************
{
    if (currentSensorName == NULL)
    {
        return "Sensoren";
    }
    else
    {
        return currentSensorName;
    }
}

const char *SensorMonitor::showLine2()
//*********************************************************************************
{
    if (currentSensorName == NULL)
    {
        return "1 - scroll";
    }
    else
    {
        return currentSensorReading;
    }
}