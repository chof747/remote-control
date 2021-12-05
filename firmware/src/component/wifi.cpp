#include "component/wifi.h"
#include "component/status.h"
#include "component/display.h"
#include <ESP8266WiFi.h>
#include "config.h"
#include "logger.h"

#define MODULE "WIFI"

bool WifiManager::setup()
{
    Log.info(MODULE,"Connecting to WiFi");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);             // Connect to the network
    Log.info(MODULE, "Connecting to %s ... ", WIFI_SSID);
    display.clear();
    display.printto(1, "Verbinde WLAN ...");
    display.show();
    delay(100);

    uint8_t count = 0;

    while ((WiFi.status() != WL_CONNECTED) && (count < 20))
    {   
        // Wait for the Wi-Fi to connect
        Log.warn(MODULE, "Connection failed retrying to connect to %s in 1s ...", WIFI_SSID);
        delay(1000);
        count++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Log.info(MODULE, "Connection established.");
        Log.info(MODULE, "IP address: %s", WiFi.localIP().toString().c_str());
        return true;
    }
    else
    {
        display.clear();
        display.printtoinv(1, "Connection Error");
        statusComponent.toggleConnectionLed(false);
        statusComponent.toggleLowPowerLed(true);
        return false;
    }

}

void WifiManager::loop() 
{

}

WifiManager wifiManager;