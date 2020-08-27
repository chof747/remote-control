#include "mqtt.h"
#include "config.h"

#include <ESP8266WiFi.h> // ESP8266 WiFi driver
#include "wifi.h"

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
                client.subscribe(MQTT_CMND_TOPIC);
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

void provideDeviceInfo() {
    /**
     * 
     * Mac - mac Address
     * IPAddress - ip Address
     * Version - firmware version + firmware name in paranthesis (e.g. 1.0.1(remote-control))
     * DeviceName - IT friendly name of the device
     * FriendlyName - descriptive name
     * Hardware - hardware type 
     * 
     */

    sprintf(gMqttMessageBuffer, 
      "{\"device\":{\"Mac\":\"%s\",\"IPAddress\":\"%s\",\"Version\":\"%s\",\"DeviceName\":\"%s\",\"FriendlyName\":\"%s\",\"Hardware\":\"%s\"}}",
      WiFi.macAddress().c_str(),
      WiFi.localIP().toString().c_str(),
      "1.0.1(remote-control)",
      "REMOTE-PT",
      "Remote Control Prototype",
      "ESP8266EX"
      );

    mqttPublish(MQTT_STATS_TOPIC, "STATUS5");
}

void mqttCallback(char *topic, byte *payload, uint8_t length)
{
    char message[length + 1];
    char command[50];

    memcpy(message, payload, length);
    message[length] = (char) 0;

    char * token = strtok(topic, "/");
    while( token != NULL) {
        strcpy(command,token);
        token = strtok(NULL,"/");
    }
#ifdef SERIAL_PRINT
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("], command =  ");
    Serial.print(command);
    Serial.print(":");
    Serial.println(message);
#endif

    if ((strcmp(command, "status") == 0) && (strcmp(message, "5") == 0)) {
        provideDeviceInfo();
    }
}
