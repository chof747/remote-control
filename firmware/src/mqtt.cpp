#include "mqtt.h"
#include "config.h"

#include <ESP8266WiFi.h> // ESP8266 WiFi driver

void mqttCallback(char *topic, byte *payload, uint8_t length);

char topicstr[100];
char gMqttMessageBuffer[255];
char mqtt_client_id[20];

WiFiClient esp_client;
PubSubClient client(esp_client);

void initializeMQTT()
//*********************************************************************************
{

    client.setServer(MQTT_SERVER, 1883);
    client.setCallback(mqttCallback);
    client.setBufferSize(255);
}

char* mqttTopic(const char* base, const char* topic) {
//*********************************************************************************
    memset(topicstr, 0, sizeof(topicstr));
    sprintf(topicstr, "%s/%s", base, topic);
    return topicstr;
}

void mqttPublish(const char* base, const char* topic) {
//*********************************************************************************
    client.publish(mqttTopic(base, topic), gMqttMessageBuffer);
}

void reconnectMQTT()
{
    if (!client.connected())
    {

        //set the client ID
        sprintf(mqtt_client_id, "esp8266-%x", ESP.getChipId());

        while (!client.connected())
        {
            #ifdef SERIAL_PRINT
              Serial.printf("Attempting MQTT connection to %s as %s ... ", MQTT_SERVER, MQTT_USER);
            #endif
            if (client.connect(mqtt_client_id,  MQTT_USER, MQTT_PASSWORD))
            {
            #ifdef SERIAL_PRINT
                Serial.println("connected");
            #endif
                // Once connected, publish an announcement
                sprintf(gMqttMessageBuffer, MQTT_STATUS_MESSAGE, mqtt_client_id, "active");
                client.publish(mqttTopic(MQTT_STATS_TOPIC, "STATUS"), gMqttMessageBuffer);
                // Resubscribe
                //client.subscribe(g_command_topic);
            }
            else
            {
            #ifdef SERIAL_PRINT
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.println(" try again in 5 seconds");
            #endif
                // Wait 5 seconds before retrying
            
                delay(5000);
            }
        }
    }
}

/**
  This callback is invoked when an MQTT message is received. It's not important
  right now for this project because we don't receive commands via MQTT. You
  can modify this function to make the device act on commands that you send it.
*/
void mqttCallback(char *topic, byte *payload, uint8_t length)
{
#ifdef SERIAL_PRINT
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
#endif
}