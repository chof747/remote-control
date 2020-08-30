#include <Arduino.h>

#include "battery.h"
#include "config.h"
#include "mqtt.h"


BatteryControl::BatteryControl(Adafruit_SSD1306 *display, uint8_t mode):
    ModeController::ModeController(display, mode)
//*********************************************************************************
{
  enableDisplay(true);
  batPercentLine = NULL;
  voltageLine = NULL;

  onActivation();
}

BatteryControl::~BatteryControl()
//*********************************************************************************
{
    if (batPercentLine != NULL) 
    {
        free(batPercentLine);
    }

    if (voltageLine != NULL) 
    {
        free(voltageLine);
    }

}

const char* BatteryControl::showLine1()
//*********************************************************************************
{
    if (batPercentLine != NULL) 
    {
        free(batPercentLine);
    }
    batPercentLine = (char*) malloc(sizeof(char) * 11);

    sprintf(batPercentLine, "Bat: %3.0f%%", percent);
    return batPercentLine;
}

const char* BatteryControl::showLine2()
//*********************************************************************************
{
   if (voltageLine != NULL) 
    {
        free(voltageLine);
    }
    voltageLine = (char*) malloc(sizeof(char) * 11);

    sprintf(voltageLine, "%8.1f V", voltage);
    return voltageLine;
}

void BatteryControl::onActivation() 
{
    readVoltage();

    sprintf(gMqttMessageBuffer, "{\"BatteryStatus\":{\"LoadPercent\": %3.0f, \"Voltage\": %.2f, \"AdcValue\": %d}}",
      percent, voltage, adcValue);

    mqttPublish(MQTT_TELE_TOPIC, "BATTERY");
    updateDisplay = true;
}

void BatteryControl::readVoltage() 
//*********************************************************************************
{
  adcValue = analogRead(A0);
  voltage = adcValue / 1023.0 * VOLTAGE_FACTOR;
  percent = (voltage - DROPOUT_VOLTAGE) / (FULL_VOLTAGE - DROPOUT_VOLTAGE) * 100;
}
