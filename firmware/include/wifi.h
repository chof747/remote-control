#ifndef WIFI_H
#define WIFI_H

// Wifi
#define WIFI_SSID                             ""
#define WIFI_PASSWORD                         ""
#define WIFI_CONNECT_INTERVAL            500
#define WIFI_CONNECT_MAX_ATTEMPTS        10  // Number of attempts/intervals to wait

bool initWifi();

#endif //WIFI_H