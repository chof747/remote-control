#include "remote.h" 
#include "config.h"
#include "mqtt.h"
#include "Arduino.h"

RemoteControl::RemoteControl(Adafruit_SSD1306 *display, uint8_t mode) : 
    ModeController::ModeController(display, mode) {

        enableDisplay(false);

}

bool RemoteControl::handleButton(uint8_t btn) {

    char topic[6] = "";
    sprintf(gMqttMessageBuffer, MQTT_PAYLOAD);
    sprintf(topic, "BTN%02d", btn); 

    mqttPublish(MQTT_STATS_TOPIC, topic);

    return true;

}