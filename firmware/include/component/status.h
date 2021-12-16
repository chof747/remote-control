#ifndef STATUS_H
#define STATUS_H

#include "config.h"
#include "component.h"
#include <Arduino.h>

class Status : public Component
{
public:
    static const int LED_CONN = CONN_LED_PIN;
    static const int LED_LOWP = LOWP_LED_PIN;

    bool setup();
    void afterSetup();
    void loop();

    void toggleConnectionLed(boolean on);
    void toggleLowPowerLed(boolean on);

    boolean isConnected();
    boolean isLowPower();

    double_t getBatteryLevel();
    const char* getBatteryStatus();

private:
    boolean connectionStatus;
    boolean lowPowerStatus;

    double_t voltage;
    int adcValue;
    double_t percent;
    char batStatus[16];

    void readVoltage();
};

extern Status statusComponent;

#endif //STATUS_H