#include "component/ota.h"
#include "logger.h"
#include "component/display.h"
#include "config.h"

bool Ota::setup()
//*********************************************************************************
{
    ArduinoOTA.begin();
    ArduinoOTA.onStart(std::bind(&Ota::start, this));
    ArduinoOTA.onEnd(std::bind(&Ota::end, this));
    ArduinoOTA.onProgress(std::bind(&Ota::progress, this, std::placeholders::_1, std::placeholders::_2));
    return true;
}

void Ota::loop()
//*********************************************************************************
{
    ArduinoOTA.handle();
}

void Ota::progress(unsigned int progress, unsigned int total)
//*********************************************************************************
{
    uint8_t p = 10 * progress / total;
    memset(progressBar, '.', p);
    progressBar[p] = '\0';

    display.printtoinv(1, "OTA Update");
    display.printto(2, progressBar);
    display.show();

}

void Ota::start()
//*********************************************************************************
{
    display.printtoinv(1, "OTA Update");
    display.printto(2, "startet");
    display.show();
}

void Ota::end()
//*********************************************************************************
{
    display.printtoinv(1, "OTA Update");
    display.printto(2, "beendet");
    display.show();
    delay(PRESS_MSG_TIME);
    display.clear();
    display.show();
}

Ota ota;