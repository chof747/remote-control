#ifndef BATTERY_H
#define BATTERY_H

#include "mode.h"

class BatteryControl: public ModeController {

public:

    BatteryControl(Adafruit_SSD1306 *display, uint8_t mode);
    ~BatteryControl();
    void onLoop() override;

protected:

    //virtual bool handleButton(uint8_t btn) override;
    const char* showLine1() override;
    const char* showLine2() override;
    void onActivation() override;

private:

    double_t voltage;
    int      adcValue;
    double_t percent;

    char* batPercentLine;
    char* voltageLine;

    void readVoltage();

};

#endif //BATTERY_H