#ifndef CONFIG_H
#define CONFIG_H

//WIFI Parameters
#define WIFI_SSID                             "WLAN_NAME"
#define WIFI_PASSWORD                         "WLAN_PASSWD"

//MQTT Parameters
#define MQTT_SERVER                           "SERVER_IP"
#define MQTT_USER                             "MQTT_USER"
#define MQTT_PASSWORD                         "MQTT_PASSWORD"

//***********************************************************************************************
//Button Setup
#define BUTTON_COUNT    4 //always amount of push buttons for mqtt + 1 for mode button
#define FIRST_BUTTON    8 
#define MODE_BUTTON     6 

//***********************************************************************************************
//MQTT Parameters for Messages
#define MQTT_PAYLOAD     "TOGGLE"
#define MQTT_SLEEP_TOPIC


//***********************************************************************************************
//Mode Remote
#define REMOTE_ACTIVE_MESSAGE  "Bereit..."

//***********************************************************************************************
//Mode Sensors
#define SENSOR_LIST_URL        "http://sensorserver:1880/sensor/list"
#define SENSOR_URL_BASE        "http://sensorserver:1880/"

//***********************************************************************************************
//Mode Battery
#define FULL_VOLTAGE        4.2
#define DROPOUT_VOLTAGE     3.4
#define VOLTAGE_FACTOR      4.3532

#endif //config.h

