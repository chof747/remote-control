#include <Arduino.h>
#include <ESP8266HTTPClient.h>

#include "sensors.h" 
#include "config.h"

#define NEXT_SENSOR_BTN     1
#define REMOTE_MODE_BTN     2


SensorMonitor::SensorMonitor(Adafruit_SSD1306 *display, uint8_t mode) : 
    ModeController::ModeController(display, mode) {

        enableDisplay(true);

        currentSensorName = NULL;
        currentSensorReading = NULL;
        firstSensor = NULL;
        
        sensorSwitch = false;
}

void SensorMonitor::clearSensors() {

    while (firstSensor != NULL) {
        sensor_t* ix = firstSensor;
        firstSensor = firstSensor->next;

        delete ix->name;
        delete ix->url;
        delete ix->value;
        delete ix;
    }

    firstSensor = NULL;

}

void SensorMonitor::obtainSensors() {

    clearSensors();

    Serial.println("Getting Sensor List ...");

    HTTPClient client;
    client.begin(SENSOR_LIST_URL);
    int httpCode = client.GET();            //Send the request
    WiFiClient* stream = client.getStreamPtr();

    char line[80];
    char* l;
    char* token;

    sensor_t* ix = NULL;

    while (int size = stream->readBytesUntil(char(10), line, 80)) {
        line[size] = '\0';
        l = line;
        char* sensorName = NULL;
        char* sensorUrl = NULL;

        while((token = strsep(&l , "|")) != NULL) {

            if (sensorName == NULL) {
                //first comes the sensor name
                sensorName = strdup(token);
            } else {
                sensorUrl = strdup(token);

            }
        }

        sensor_t* sensor = new sensor_t();
        sensor->name = sensorName;
        sensor->url = sensorUrl;
        sensor->next = NULL;

        if (ix == NULL) {
            ix = sensor;
            firstSensor = ix;
        } else {
            ix->next = sensor;
            ix = sensor;
        }
    }

    iterator = NULL;

    Serial.println(httpCode);
}

char* SensorMonitor::readSensor() {
    HTTPClient client;
    char url[100];
    sprintf(url, "%s%s", SENSOR_URL_BASE, iterator->url);

    client.begin(url);
    int httpCode = client.GET();
    Serial.printf("HTTP-Return Code: %d", httpCode);
    if (httpCode == 200) {
        iterator->value = strdup(client.getString().c_str());
    }

    return iterator->value;
}

bool SensorMonitor::handleButton(uint8_t btn) {

    Serial.printf("Button: %d\n", btn);

    if (btn == NEXT_SENSOR_BTN) {
        #ifdef SERIAL_PRINT
        Serial.print("Switching to next sensor reading!");
        #endif

        if ((iterator == NULL) || (iterator->next == NULL)) {
            iterator = firstSensor;
        } else {
            iterator = iterator->next;
        }

        needExecution(); 
    }

    return true;
}

bool SensorMonitor::onExecution() {

    #ifdef SERIAL_PRINT
    Serial.printf("Reading  Sensor: %s", iterator->name);
    #endif

    currentSensorName = iterator->name;
    currentSensorReading = readSensor();

    return false;
}

void SensorMonitor::onActivation() {
    #ifdef SERIAL_PRINT
    Serial.println("Activating Sensor Reading Mode ...");
    #endif

    obtainSensors();

    updateDisplay = true;
    currentSensorReading = NULL;
    currentSensorName = NULL;
    sensorSwitch = false;
}

const char* SensorMonitor::showLine1() {
    if (currentSensorName == NULL) {
        return "1 naechste";
    } else {
        return currentSensorName;
    }
}

const char* SensorMonitor::showLine2() {
    if (currentSensorName == NULL) {
        return "2 Remote";
    } else {
        return currentSensorReading;
    }
}