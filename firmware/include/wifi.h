#ifndef WIFI_H
#define WIFI_H


/**
 * CONFIGURATION NOTE:
 * 
 * WIFI_SSID, WIFI_PASSWORD must be defined in config.h
 * please copy the vc'ed file config_sample.h and enter the parameters of your config
 * 
 */


// Wifi
#define WIFI_CONNECT_INTERVAL            500
#define WIFI_CONNECT_MAX_ATTEMPTS        10  // Number of attempts/intervals to wait

bool initWifi();

#endif //WIFI_H