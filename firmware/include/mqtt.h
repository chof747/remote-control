#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h> // Required for MQTT

#define MQTT_SERVER ""
#define MQTT_USER ""
#define MQTT_PASSWORD ""

#define MQTT_STATS_TOPIC "stat/" MQTT_USER

#define MQTT_STATUS_MESSAGE "{ \"RC\" :  \"%s\", \"STATUS\" : \"%s\" }"

// MQTT
void initializeMQTT();
void reconnectMQTT();
void mqttPublish(char* base, const char* topic);

extern char gMqttMessageBuffer[255];
extern char mqtt_client_id[20];

extern PubSubClient client;

#endif //MQTT_H
