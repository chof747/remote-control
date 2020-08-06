#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include "wifi.h"
#include "config.h"

bool initWifi()
{
  // Clean up any old auto-connections
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi.disconnect();
  }
  WiFi.setAutoConnect(false);

  if (sizeof(WIFI_SSID) == 1)
  {
    return false;
  }

  #ifdef SERIAL_PRINT

    Serial.print("Trying to connect to: ");
    Serial.println(WIFI_SSID);

  #endif

  WiFi.mode(WIFI_STA); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for connection set amount of intervals
  int num_attempts = 0;

  while (WiFi.status() != WL_CONNECTED && num_attempts <= WIFI_CONNECT_MAX_ATTEMPTS)
  {
    delay(WIFI_CONNECT_INTERVAL);
    num_attempts++;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    #ifdef SERIAL_PRINT
      Serial.println("Failed to connect to WIFI!");
    #endif
    return false;
  } else {
  
  #ifdef SERIAL_PRINT

    Serial.print("Connected to: ");
    Serial.print(WIFI_SSID);
    Serial.print(" by ");
    Serial.print(WiFi.localIP());
    Serial.print(" with ");
    Serial.println(WiFi.macAddress());

  #endif

    return true;
  }
}
