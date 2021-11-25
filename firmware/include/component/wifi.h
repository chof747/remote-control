#ifndef WIFI_H
#define WIFI_H

#include "component.h"

class WifiManager: public Component
{

public:
    void setup();
    void loop();

};

extern WifiManager wifiManager;

#endif //WIFI_H