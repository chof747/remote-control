#include "component/wifi.h"
#include <ESP8266WiFi.h>
#include "config.h"
#include "logger.h"

#define MODULE "WIFI"

void WifiManager::setup()
{
    Log.info(MODULE,"Connecting to WiFi");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);             // Connect to the network
    Log.info(MODULE, "Connecting to %s ... ", WIFI_SSID);
    delay(100);

    while (WiFi.status() != WL_CONNECTED) 
    {   
        // Wait for the Wi-Fi to connect
        Log.warn(MODULE, "Connection failed retrying to connect to %s in 1s ...", WIFI_SSID);
        delay(1000);
    }

    Log.info(MODULE, "Connection established.");
    Log.info(MODULE, "IP address: %s", WiFi.localIP().toString().c_str());
}

void WifiManager::loop() 
{

}

WifiManager wifiManager;