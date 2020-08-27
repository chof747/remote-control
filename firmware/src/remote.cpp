#include "remote.h" 
#include "config.h"
#include "mqtt.h"
#include "Arduino.h"

RemoteControl::RemoteControl(Adafruit_SSD1306 *display, uint8_t mode) : 
    ModeController::ModeController(display, mode) {

        enableDisplay(true);

}

bool RemoteControl::handleButton(uint8_t btn) {

    char topic[6] = "";
    sprintf(gMqttMessageBuffer, MQTT_PAYLOAD);
    sprintf(topic, "BTN%02d", btn); 

    mqttPublish(MQTT_STATS_TOPIC, topic);

    return true;

}

void RemoteControl::onActivation() {
    #ifdef SERIAL_PRINT
    Serial.println("Activate Remote...");
    #endif
    
    updateDisplay = true;
}

const char* RemoteControl::showLine1() {
    return REMOTE_ACTIVE_MESSAGE;
}