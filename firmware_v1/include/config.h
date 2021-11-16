#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"


//WIFI Parameters
#if (ENVIRONMENT == 0)
  #define WIFI_SSID                             "apfelbaum"
  #define WIFI_2_SSID                           "apfelbaum"
#elif (ENVIRONMENT == 1)
  #define WIFI_SSID                             "apfelbaum"
  #define WIFI_2_SSID                             "apfelbaum"
#endif
#define WIFI_PASSWORD                         "Sack Zement!"
#define WIFI_2_PASSWORD                         "Sack Zement!"

//MQTT Parameters
#if (ENVIRONMENT == 0)
  #define MQTT_SERVER                           "10.0.1.26"
  #define MQTT_USER                             "remote"
  #define MQTT_PASSWORD                         "WorldWasNotEnough4Me!"
#elif (ENVIRONMENT == 1)
  #define MQTT_SERVER                           "10.0.1.39"
  #define MQTT_USER                             "remote"
  #define MQTT_PASSWORD                         "WorldWasNotEnough4Me!"
#endif 

//OTA Parameters
#if (ENVIRONMENT == 0)
  #define DEFAULT_OTA_URL                       "http://10.0.1.26:1880/remote/upgrade"
#elif (ENVIRONMENT == 1)
  #define DEFAULT_OTA_URL                       "http://10.0.1.39:1880/remote/upgrade"
#endif


//***********************************************************************************************
//Button Setup
#if (ENVIRONMENT == 0)
  #define BUTTON_COUNT      11 //always amount of push buttons for mqtt + 1 for mode button
  #define FIRST_BUTTON      6  
  #define MODE_BUTTON       0  
  #define LOOP_DELAY        10
  #define MODE_LOOP_CYCLES  50
#elif (ENVIRONMENT == 1)
  #define BUTTON_COUNT    11 //always amount of push buttons for mqtt + 1 for mode button
  #define FIRST_BUTTON    6 
  #define MODE_BUTTON     0 
  #define LOOP_DELAY     10
  #define MODE_LOOP_CYCLES  50
#endif

//***********************************************************************************************
//Controller Setup
#if (ENVIRONMENT == 0)
  #define CONN_LED_PIN     D7
  #define LOWP_LED_PIN     D6
  #define PRESS_MSG_TIME 1000
  #define ERR_MSG_TIME   2000
#elif (ENVIRONMENT == 1)
  #define CONN_LED_PIN     D6
  #define LOWP_LED_PIN     D7
  #define PRESS_MSG_TIME 1000
  #define ERR_MSG_TIME   2000
#endif


#if (ENVIRONMENT == 0)
  #define DEEP_SLEEP_AFTER 10000          //milliseconds until deep sleep
  #define WAKE_UP_WATCH_PIN 1             //the pin on the mcp that is pulled high on sleep to 
                                          //open the transistor so that a push on one of the buttons
                                          //pulls reset to GND for wakeup.
#elif (ENVIRONMENT == 1)
  #define DEEP_SLEEP_AFTER 30000          //milliseconds until deep sleep
  #define WAKE_UP_WATCH_PIN 1             //the pin on the mcp that is pulled high on sleep to 
                                          //open the transistor so that a push on one of the buttons
                                          //pulls reset to GND for wakeup.
#endif


//***********************************************************************************************
//MQTT Parameters for Messages
#define MQTT_PAYLOAD     "TOGGLE"
#define MQTT_SLEEP_TOPIC


//***********************************************************************************************
//Mode Remote
#define REMOTE_ACTIVE_MESSAGE  "Bereit..."

//***********************************************************************************************
//Mode Sensors
#if (ENVIRONMENT == 0)
  #define SENSOR_URL        "http://10.0.1.26:1880/sensor/next"
#elif (ENVIRONMENT == 1)
  #define SENSOR_URL        "http://10.0.1.39:1880/sensor/next"
#endif 


//***********************************************************************************************
//Mode Battery
#define FULL_VOLTAGE        4.15
#define DROPOUT_VOLTAGE     3.2
#define VOLTAGE_FACTOR      4.3532
#define WARNING_LEVEL       3.3


extern uint16_t gButtons[BUTTON_COUNT];

#endif //config.h

