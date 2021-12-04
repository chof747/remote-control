#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include "component/component.h"
#include "config.h"

typedef std::function<void(String topic, String payload)> callback_t;

struct mqttSubscription_t 
{
    String topic;
    callback_t callback;
};

class MqttClient: public Component
{
    public:
        MqttClient();

        bool setup();
        void loop();

        void tele(const char* topic, const char* message, bool retain = false);
        void stat(const char* topic, const char* message, bool retain = false);

        bool subscribe(const char* topic, callback_t callback);

    private:
        PubSubClient client;  

        char mqttClientId[20];
        mqttSubscription_t subscriptions[MQTT_SUBSCRIPTIONS];
        int freeSubscriptionIx = 0;

        bool reconnect();
        void subscriptionCallback(char* topic, uint8_t* payload, unsigned int length);
        void publish(const char* topic, const char* message, bool retain = false);
};

extern MqttClient mqttClient;

#endif //MQTT_H