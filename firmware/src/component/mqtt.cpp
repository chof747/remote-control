#include "component/mqtt.h"
#include "component/wifi.h"
#include "component/status.h"
#include "component/display.h"
#include <ESP8266WiFi.h> // ESP8266 WiFi driver
#include "logger.h"

#define MODULE "MQTT"

WiFiClient esp_client;
MqttClient mqttClient;

#define TELE_TOPIC_BASE "tele/" DEVICE_NAME
#define CMND_TOPIC_BASE "cmnd/" DEVICE_NAME
#define STAT_TOPIC_BASE "stat/" DEVICE_NAME

#include "component/mqtt/topic_matcher.h"

MqttClient::MqttClient() : client(esp_client), mqttClientId()
//****************************************************************************************
{

}

bool MqttClient::setup()
//****************************************************************************************
{
    MQTT_CALLBACK_SIGNATURE = std::bind(&MqttClient::subscriptionCallback, 
                                        this, 
                                        std::placeholders::_1, 
                                        std::placeholders::_2, std
                                        ::placeholders::_3);

    client.setServer(MQTT_BROKER, 1883);
    client.setCallback(callback);
    client.setBufferSize(255);

    Log.info(MODULE, "Initializing MQTT connection to: %s", MQTT_BROKER);
    return reconnect();
}

void MqttClient::loop()
//****************************************************************************************
{
    if (reconnect())
    {
        client.loop();
    }

}

void MqttClient::tele(const char* topic, const char* message, bool retain)
//****************************************************************************************
{
    char topicbuf[255];
    sprintf(topicbuf,"%s/%s", TELE_TOPIC_BASE, topic);
    publish(topicbuf,message,retain);
}

void MqttClient::stat(const char* topic, const char* message, bool retain)
//****************************************************************************************
{
    char topicbuf[255];
    sprintf(topicbuf,"%s/%s", STAT_TOPIC_BASE, topic);
    publish(topicbuf,message,retain);
}

void MqttClient::publish(const char* topic, const char* message, bool retain)
//****************************************************************************************
{
    Log.debug(MODULE, "Publishing for topic: %s", topic);
    Log.debug(MODULE, "Content: %s", message);
    client.publish(topic, message);
}

bool MqttClient::subscribe(const char* topic, callback_t callback)
//****************************************************************************************
{

    if (freeSubscriptionIx<MQTT_SUBSCRIPTIONS)
    {
        char topicbuf[255];

        sprintf(topicbuf,"%s/%s", CMND_TOPIC_BASE, topic);
        subscriptions[freeSubscriptionIx].topic = topicbuf;
        subscriptions[freeSubscriptionIx].callback = callback;
        freeSubscriptionIx += 1;

        client.subscribe(topicbuf);
        Log.info(MODULE, "Subscribing to topic %s", topicbuf);
        return true;
    }
    else
    {
        return false;
    }

}

bool MqttClient::reconnect()
//****************************************************************************************
{
    if (!client.connected())
    {
        //set the client ID
        sprintf(mqttClientId, "alarmc-%x", ESP.getChipId());
        int counter = 0;

        int errcode = 0;

        while ((!client.connected()) && (counter < 3))
        {
            display.clear();
            display.printto(1, "Verbinde MQTT ...");
            display.show();
            bool connected;
            if (MQTT_USER != NULL)
            {
                Log.info(MODULE, "Attempting MQTT connection to %s as %s ... ", 
                    MQTT_BROKER, MQTT_USER);
                connected = client.connect(mqttClientId,  MQTT_USER, MQTT_PASSWORD);
   }
            else
            {
                Log.info(MODULE, "Attempting MQTT connection to %s as anonymous ... ", MQTT_BROKER);
                connected = client.connect(mqttClientId);
            }
            if (connected)
            {
                Log.info(MODULE, "connected to %s", MQTT_BROKER);
                stat("status", "connected");
            }
            else
            {
                errcode = client.state();
                Log.warn(MODULE, "failed to connect to %s, rc was %d", 
                         MQTT_BROKER, errcode);
                delay(5000);
            }

            ++counter;
        }

        if (!client.connected())
        {
            Log.error(MODULE, 
                      "Could not reconnect to MQTT broker %s - please check the connections and reboot", 
                      MQTT_BROKER);

            display.clear();
            display.printtoinv(1, "MQTT Error");
            display.printto(2, "Err. Code: %d", errcode);
            statusComponent.toggleLowPowerLed(true); 
            statusComponent.toggleConnectionLed(false);
            return false;
        }
        else
        {
            display.clear();
            display.show();
            return true;
        }
    }

    statusComponent.toggleConnectionLed(client.connected());

    return true;
}

void MqttClient::subscriptionCallback(char* topic, uint8_t* payload, unsigned int length)
//****************************************************************************************
{
    //the payload needs to be zero terminated for the following procedure
    //NOTE: If this leads to runtime errors, replace with copying the payload into a 
    //      real char array of the given length
    payload[length] = 0;

    Log.debug(MODULE, "Received: %s", topic);
    Log.debug(MODULE, "Content (length:%d): %s", length,(char*)payload);

    for(int i=0;i<MQTT_SUBSCRIPTIONS;++i)
    {
        if (topic_match(topic, subscriptions[i].topic.c_str()))
        {
            //trim the standard command topic base name but leave the leading /
            topic += strlen(CMND_TOPIC_BASE);
            String t = String(topic);
            String m = String((char*)payload);
            subscriptions->callback(t,m);
        }
    }
}