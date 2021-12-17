#ifndef OTA_H
#define OTA_H

#include "component.h"
#include <ArduinoOTA.h>

class Ota: public Component
{
    public:

        bool setup();
        void loop();

    private:

        char progressBar[11];
        void progress(unsigned int progress, unsigned int total);
        void start();
        void end();
};

extern Ota ota;

#endif //STATUS_INDICATOR_H