#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_MCP23017.h"
#include "wifi.h"
#include "mqtt.h"
#include "config.h"


//***********************************************************************************************
//Controller Setup
#define CONN_LED_PIN     D6
#define DEEP_SLEEP_AFTER 10000          //milliseconds until deep sleep
#define WAKE_UP_WATCH_PIN 7             //the pin on the mcp that is pulled high on sleep to 
                                        //open the transistor so that a push on one of the buttons
                                        //pulls reset to GND for wakeup.

//***********************************************************************************************
//Button Setup
#define BUTTON_COUNT    2
#define FIRST_BUTTON    8 
const uint16_t gButtons[BUTTON_COUNT] = {8, 9};
uint16_t gButtonMask = 0;

//***********************************************************************************************
//MQTT Parameters for Messages
#define MQTT_PAYLOAD     "TOGGLE"
#define MQTT_SLEEP_TOPIC

//***********************************************************************************************
//State Variables
uint16_t millisSinceLastAction = 0;


#define activateInts() attachInterrupt(digitalPinToInterrupt(D5),intHandlerButton,FALLING)
#define deactivateInts() detachInterrupt(digitalPinToInterrupt(D5))
#define clearInterrupts()  while(  mcp.getLastInterruptPinValue() != 255) 

Adafruit_MCP23017 mcp;

void setupGPIOExtender();
void ICACHE_RAM_ATTR intHandlerButton();


void rest(uint16_t activeDelay) {

  millisSinceLastAction += activeDelay;

  if (millisSinceLastAction >= DEEP_SLEEP_AFTER) {
    #ifdef SERIAL_PRINT
    Serial.println("Going to Sleep!");
    #endif

    sprintf(gMqttMessageBuffer, MQTT_STATUS_MESSAGE, mqtt_client_id, "inactive");
    mqttPublish(MQTT_STATS_TOPIC, "STATUS");

    digitalWrite(CONN_LED_PIN, LOW);
    mcp.digitalWrite(WAKE_UP_WATCH_PIN, HIGH);

    delay(10);
    ESP.deepSleep(0);
  } else {
    delay(activeDelay);
  }

}

void setupGPIOExtender() {

  /**
   *  Description of the approach as it caused some headheache during development:
   * 
   *  For all defined buttons on the extender  
   */

  // General Chip Setup

  mcp.begin();
  mcp.pinMode(WAKE_UP_WATCH_PIN, OUTPUT);
  mcp.digitalWrite(WAKE_UP_WATCH_PIN, LOW);

  mcp.setupInterrupts(true,false,LOW);

  //BUTTONS
  for(int i=0;i<BUTTON_COUNT;i++) {

    uint16_t b = gButtons[i];

    mcp.digitalWrite(b, HIGH);
    mcp.pinMode(b, INPUT);
    mcp.pullUp(b, HIGH);
    mcp.setupInterruptPin(b, FALLING);
    //while (mcp.digitalRead(b) != HIGH);
    gButtonMask += 1 << b;

  }

  #ifdef SERIAL_PRINT
  Serial.printf("Button Mask : %d\n", gButtonMask);
  #endif
  
  activateInts();


 }   

void broadcastButtonPress(uint8_t button) {

  char topic[6] = "";
  sprintf(gMqttMessageBuffer, MQTT_PAYLOAD);
  sprintf(topic, "BTN%02d", button); 

  millisSinceLastAction = 0;
  mqttPublish(MQTT_STATS_TOPIC, topic);
}
 
void intHandlerButton() {
  deactivateInts();
  noInterrupts();

  // Get more information from the MCP from the INT
  uint8_t pin=mcp.getLastInterruptPin();
  uint8_t val=mcp.getLastInterruptPinValue();


  if (val == 0) {
    broadcastButtonPress(pin - FIRST_BUTTON + 1) ;
  }

  #ifdef SERIAL_PRINT
  Serial.printf("Button %d pressed (value = %d)\n", pin, val);
  #endif

  clearInterrupts();
  interrupts();
  activateInts();
}

void setup()
{
#ifdef SERIAL_PRINT
  Serial.begin(9600);
  Serial.println("Initializing ...");
#endif 

  //setup power on led
  pinMode(CONN_LED_PIN, OUTPUT);
 
  setupGPIOExtender();

  initWifi();
  initializeMQTT();

  while (mcp.getLastInterruptPinValue() != 255) {
    delay(0);
  }
 }

void loop()
{
  reconnectMQTT();
  //indicate readiness
  digitalWrite(CONN_LED_PIN, HIGH);
  client.loop();
  //mcp.digitalRead(8);
  //digitalWrite(CONN_LED_PIN, HIGH);
  
  rest(1000);
}
