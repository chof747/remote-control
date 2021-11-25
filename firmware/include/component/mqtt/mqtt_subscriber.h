#ifndef MQTT_SUBSCRIBER_H
#define MQTT_SUBSCRIBER_H
#include "component/mqtt.h"

class MqttSubscriber 
{
    public:

        virtual callback_t getCallback() = 0;
        virtual void onMessage(String topic, String payload) = 0;
};

#define CALLBACK_TRANSLATOR(className) std::bind(&className::onMessage, this, std::placeholders::_1, std::placeholders::_2)

#endif //MQTT_SUBSCRIBER_H