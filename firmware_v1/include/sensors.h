#ifndef SENSOR_H
#define SENSOR_H

#include "mode.h"

class SensorMonitor: public ModeController {

public:

    SensorMonitor(Adafruit_SSD1306 *display, uint8_t mode);

protected:

    bool handleButton(uint8_t btn) override;
    void onActivation() override;
    bool onExecution() override;

    const char* showLine1() override;
    const char* showLine2() override;
    const char* showLine3() override;

private:

    bool sensorSwitch;

    int nSensors;
    char lines[3][80];

    void readSensor();

};


#endif //SENSOR_H