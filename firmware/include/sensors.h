#ifndef SENSOR_H
#define SENSOR_H

#include "mode.h"


struct sensor_description_main_t {
    char prefix[15]; //the prefix after the server url before the specific part of the sensor URL
    char number[2];  //the number of sensors 
};

struct sensor_t {
    char name[8];
    char path[24];
};

class SensorMonitor: public ModeController {

public:

    SensorMonitor(Adafruit_SSD1306 *display, uint8_t mode);

protected:

    bool handleButton(uint8_t btn) override;
    void onActivation() override;
    bool onExecution() override;

    const char* showLine1() override;
    const char* showLine2() override;

private:

    bool sensorSwitch;
    char* currentSensorName;
    char* currentSensorReading;
    int currentSensor;

    sensor_description_main_t sensorMain;
    sensor_t* sensors;
    char** values;

  
    void clearSensors();
    void createSensors(int count);
    sensor_t* readSensorDefinition(char* line, sensor_t* pre);
    void obtainSensors();

    char* readSensor();

};


#endif //SENSOR_H