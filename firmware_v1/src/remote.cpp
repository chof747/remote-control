#include "remote.h" 
#include "config.h"
#include "mqtt.h"
#include "Arduino.h"

RemoteControl::RemoteControl(Adafruit_SSD1306 *display, uint8_t mode) : 
    ModeController::ModeController(display, mode) 
{
        enableDisplay(true);
        message = new char[24];
        strcpy(message, REMOTE_ACTIVE_MESSAGE);

}

RemoteControl::~RemoteControl() 
{
    delete message;
}

bool RemoteControl::handleButton(uint8_t btn) {

    char topic[6] = "";
    sprintf(gMqttMessageBuffer, MQTT_PAYLOAD);
    sprintf(topic, "BTN%02d", btn); 

    mqttPublish(MQTT_STATS_TOPIC, topic);
    pressMsgTimer = 0;

    sprintf(message, "%02d erkannt", btn);
    needExecution();

    return true;

}

void RemoteControl::onActivation() {
    #ifdef SERIAL_PRINT
    Serial.println("Activate Remote...");
    #endif
    
    updateDisplay = true;
}

const char* RemoteControl::showLine1() {
    return message;
}

bool RemoteControl::onExecution()
//*********************************************************************************
{
    pressMsgTimer = pressMsgTimer + LOOP_DELAY;
    pressMsgTimer = (PRESS_MSG_TIME < pressMsgTimer) ? 0 : pressMsgTimer;

    //Serial.printf("Execution with %d \n", pressMsgTimer);

    if (0 == pressMsgTimer)
    {
        strcpy(message, REMOTE_ACTIVE_MESSAGE);
        updateDisplay = true;
        return false;
    }
    else 
    {
        return true;
    }

}
