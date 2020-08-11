#ifndef CONFIG_H
#define CONFIG_H

//WIFI Parameters
#define WIFI_SSID                             "WIFI_ID"
#define WIFI_PASSWORD                         "WIFI_PASSWORD"

//MQTT Parameters
#define MQTT_SERVER                           "MQTT_SERVER"
#define MQTT_USER                             "MQTT_USER"
#define MQTT_PASSWORD                         "MQTT_PASSWORD"

//***********************************************************************************************
//Button Setup
#define BUTTON_COUNT    3 //always amount of push buttons for mqtt + 1 for mode button
#define FIRST_BUTTON    8 
#define MODE_BUTTON     6 

//***********************************************************************************************
//MQTT Parameters for Messages
#define MQTT_PAYLOAD     "TOGGLE"
#define MQTT_SLEEP_TOPIC

#endif //config.h