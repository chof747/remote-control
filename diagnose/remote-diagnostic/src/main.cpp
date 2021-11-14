#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp;

volatile bool pressed;
uint8_t val;
uint8_t pin;
void ICACHE_RAM_ATTR intHandlerButton();

void setup() {

    pressed = false;
    Serial.begin(9600);
    Serial.println("Testing GPIO Expander");
    mcp.begin();
    mcp.setupInterrupts(true, false, LOW);
    
    int b = 0;

    mcp.digitalWrite(b, HIGH);
    mcp.pinMode(b, INPUT);
    mcp.pullUp(b, HIGH);

    mcp.setupInterruptPin(b, FALLING);
    attachInterrupt(digitalPinToInterrupt(D5), intHandlerButton, FALLING);

    Serial.printf("%d\n", mcp.digitalRead(b));
}

void intHandlerButton()
//*********************************************************************************
{
  // Get more information from the MCP from the INT

  pressed = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (pressed)
  {
    detachInterrupt(digitalPinToInterrupt(D5));

    pin = mcp.getLastInterruptPin();
    val = mcp.getLastInterruptPinValue();
    Serial.printf("Pressed: %d with value %d\n", pin, val);
    pressed = false;

    while(!mcp.digitalRead(pin));
    
    //Clear GPIO Interrupt Status
    int gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
    attachInterrupt(digitalPinToInterrupt(D5), intHandlerButton, FALLING);
  }

  delay(50);
}