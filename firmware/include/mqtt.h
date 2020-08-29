#ifndef MQTT_H
#define MQTT_H


#include <PubSubClient.h> // Required for MQTT


/**
 * CONFIGURATION NOTE:
 * 
 * MQTTSERVER, MQTTUSER, MQTT_PASSWORD must be defined in config.h
 * please copy the vc'ed file config_sample.h and enter the parameters of your config
 * 
 */

#define MQTT_STATS_TOPIC "stat/" MQTT_USER
#define MQTT_COMMAND_TOPIC "cmnd/" MQTT_USER 
#define MQTT_TELE_TOPIC "tele/" MQTT_USER 
#define MQTT_COMMAND_TOPIC_SUBSCRIBE MQTT_COMMAND_TOPIC "/#"

#define MQTT_STATUS_MESSAGE "{ \"RC\" :  \"%s\", \"STATUS\" : \"%s\" }"

// MQTT
void initializeMQTT();
bool reconnectMQTT();
void mqttPublish(const char* base, const char* topic);

extern char gMqttMessageBuffer[255];
extern char mqtt_client_id[20];

extern PubSubClient client;

#endif //MQTT_H
