#ifndef CONFIG_H
#define CONFIG_H


#define WIFI_SSID       ""
#define WIFI_PASSWORD   ""

#if ENVIRONMENT == 1 //production
  #define MQTT_BROKER     "10.0.1.42"
  #define MQTT_USER       ""
  #define MQTT_PASSWORD   ""
#elif ENVIRONMENT == 0 //development
  #define MQTT_BROKER     "127.0.0.1"
  #define MQTT_USER       ""
  #define MQTT_PASSWORD   ""
#else
#endif
#define MQTT_SUBSCRIPTIONS 2


#if ENVIRONMENT == 1 //production
  #define MESSAGE_API_ENDPOINT ""
#elif ENVIRONMENT == 0 //development
  #define MESSAGE_API_ENDPOINT "http://127.0.0.1:3000/api/status/"
#else
#endif

#ifndef CONTROLLER_TIMEOUT 
    #define CONTROLLER_TIMEOUT 30000
#endif


#endif //CONFIG_H