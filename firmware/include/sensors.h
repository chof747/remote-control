#ifndef SENSOR_H
#define SENSOR_H

#include "mode.h"

struct sensor_t {
    char* name;
    const char* url;
    char* value;

    sensor_t* next;
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

    sensor_t* firstSensor;
    sensor_t* iterator;

    void clearSensors();
    void obtainSensors();
    char* readSensor();

};


#endif //SENSOR_H