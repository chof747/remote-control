#ifndef STATUS_H
#define STATUS_H

#include "config.h"
#include "component.h"
#include <Arduino.h>

class Status: public Component
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

    private:

        boolean connectionStatus;
        boolean lowPowerStatus;

};

extern Status statusComponent;


#endif //STATUS_H