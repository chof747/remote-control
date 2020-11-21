#include <Arduino.h>
#include <Wire.h>
#include <WiFiManager.h>

#include "Adafruit_MCP23017.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "mqtt.h"
#include "config.h"

#include "mode_registry.h"

//***********************************************************************************************
// Buttons
uint16_t gButtonMask = 0;
uint16_t gButtons[BUTTON_COUNT];

//***********************************************************************************************
//State Variables
uint16_t millisSinceLastAction = 0;
uint16_t loopcount = 0;

#define activateInts() attachInterrupt(digitalPinToInterrupt(D5), intHandlerButton, FALLING)
#define deactivateInts() detachInterrupt(digitalPinToInterrupt(D5))
#define clearInterrupts() while (mcp.getLastInterruptPinValue() != 255)

Adafruit_MCP23017 mcp;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

void ICACHE_RAM_ATTR intHandlerButton();

void rest(uint16_t activeDelay)
//*********************************************************************************
{

  millisSinceLastAction += activeDelay;

  if (millisSinceLastAction >= DEEP_SLEEP_AFTER)
  {
#ifdef SERIAL_PRINT
    Serial.println("Going to Sleep!");
#endif

    sprintf(gMqttMessageBuffer, MQTT_STATUS_MESSAGE, mqtt_client_id, "inactive");
    mqttPublish(MQTT_STATS_TOPIC, "STATUS");

    digitalWrite(CONN_LED_PIN, LOW);
    mcp.digitalWrite(WAKE_UP_WATCH_PIN, HIGH);
    display.ssd1306_command(SSD1306_DISPLAYOFF);

    delay(10);
    ESP.deepSleep(0);
  }
  else
  {
    delay(activeDelay);
  }
}

void setupButtonArray()
{
  for (int i = 0; i < BUTTON_COUNT - 1; i++)
  {
    gButtons[i] = FIRST_BUTTON + i;
  }

  //mode button comes last:
  gButtons[BUTTON_COUNT - 1] = MODE_BUTTON;
}

void setupGPIOExtender()
//*********************************************************************************
{

  /**
   *  Description of the approach as it caused some headheache during development:
   * 
   *  For all defined buttons on the extender  
   */

  // General Chip Setup

  mcp.begin();
  mcp.pinMode(WAKE_UP_WATCH_PIN, OUTPUT);
  mcp.digitalWrite(WAKE_UP_WATCH_PIN, LOW);

  mcp.setupInterrupts(true, false, LOW);

  setupButtonArray();

  //BUTTONS
  for (int i = 0; i < BUTTON_COUNT; i++)
  {

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

void printError(const char *head, const char *msg, bool sleep = false, const char *details = NULL)
//*********************************************************************************
{

#ifdef SERIAL_PRINT
  if (NULL == details)
  {
    Serial.printf("%s %s\n", head, msg);
  }
  else
  {
    Serial.println(details);
  }
#endif

  display.clearDisplay();
  display.println(head);
  display.println(msg);
  display.display();
  digitalWrite(LOWP_LED_PIN, HIGH);

  delay(ERR_MSG_TIME);
  if (sleep)
  {
    rest(DEEP_SLEEP_AFTER);
  }
  else
  {
    ESP.reset();
  }
}

void setupNetwork()
//*********************************************************************************
{
  WiFiManager wifiManager;

#ifdef SERIAL_PRINT
  wifiManager.setDebugOutput(true);
#else
  wifiManager.setDebugOutput(false);
#endif

  if (!wifiManager.autoConnect())
  {
    //reset and try again, or maybe put it to deep sleep
    printError("Error:", "No WLAN", "could not connect to network");
  }

  initializeMQTT(mqttCallback);
  if (!reconnectMQTT())
  {
    printError("Error:", "No Mqtt Con", true);
  }
}

void setupDisplay()
//*********************************************************************************
{
  display.begin();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextWrap(false);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void setup()
//*********************************************************************************
{
#ifdef SERIAL_PRINT
  Serial.begin(9600);
  Serial.println("Initializing ...");
#endif

  //setup power on led
  pinMode(CONN_LED_PIN, OUTPUT);
  pinMode(LOWP_LED_PIN, OUTPUT);
  pinMode(A0, INPUT);

  digitalWrite(CONN_LED_PIN, LOW);
  digitalWrite(LOWP_LED_PIN, LOW);

  setupGPIOExtender();
  setupDisplay();

  //clear the interrupt

#ifndef NO_INTERRUPTS
  while (mcp.getLastInterruptPinValue() != 255)
  {
    delay(0);
  }
#endif

  setupNetwork();

  initializeModes(&display);
  currentMode()->activate();
}

void updateDisplay()
//*********************************************************************************
{
  currentMode()->display();
}

void loop()
//*********************************************************************************
{
  reconnectMQTT();
  //indicate readiness
  digitalWrite(CONN_LED_PIN, HIGH);

  client.loop();
  if (0 == loopcount % MODE_LOOP_CYCLES)
  {
    loopModes();
  }

  currentMode()->execute();
  updateDisplay();

  ++loopcount;
  rest(LOOP_DELAY);
}

void intHandlerButton()
//*********************************************************************************
{
  deactivateInts();
  noInterrupts();

  //reset the sleep timer
  millisSinceLastAction = 0;

  // Get more information from the MCP from the INT
  uint8_t pin = mcp.getLastInterruptPin();
  uint8_t val = mcp.getLastInterruptPinValue();

  if (val == 0)
  {
    if (pin == MODE_BUTTON)
    {
      nextMode();
    }
    else
    {
      currentMode()->btnPressed(pin - FIRST_BUTTON + 1);
    }
  }

#ifdef SERIAL_PRINT
  Serial.printf("Button %d pressed (value = %d)\n", pin, val);
#endif

  clearInterrupts();
  interrupts();
  activateInts();
}
