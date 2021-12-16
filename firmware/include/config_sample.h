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

#define MQTT_SUBSCRIPTIONS 0
#define MQTT_ACTIVATION_MSG "active"

//***********************************************************************************************
//Controller Setup
#if (ENVIRONMENT == 0)
  #define CONN_LED_PIN     D7
  #define LOWP_LED_PIN     D6
  #define PRESS_MSG_TIME 1000
  #define ERR_MSG_TIME  10000
#elif (ENVIRONMENT == 1)
  #define CONN_LED_PIN     D6
  #define LOWP_LED_PIN     D7
  #define PRESS_MSG_TIME 1000
  #define ERR_MSG_TIME   2000
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
//Battery Management
#define FULL_VOLTAGE        4.15
#define DROPOUT_VOLTAGE     3.2
#define VOLTAGE_FACTOR      4.3532
#define WARNING_LEVEL       3.3

#endif //CONFIG_H