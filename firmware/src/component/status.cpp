#include "component/status.h"

#include "logger.h"

#define MODULE "STATUS"


bool Status::setup()
//*********************************************************************************
{
    pinMode(Status::LED_CONN, OUTPUT);
    pinMode(Status::LED_LOWP, OUTPUT);

    connectionStatus = false;
    lowPowerStatus = false;

    return true;
}

void Status::afterSetup()
//*********************************************************************************
{

}

void Status::loop()
//*********************************************************************************
{
}

boolean Status::isConnected()
//*********************************************************************************
{
    return connectionStatus;
}

boolean Status::isLowPower()
//*********************************************************************************
{
    return lowPowerStatus;
}

double_t Status::getBatteryLevel()
//*********************************************************************************
{
    readVoltage();
    return percent;
}

const char* Status::getBatteryStatus()
//*********************************************************************************
{
    readVoltage();
    return batStatus;
} 

void Status::toggleConnectionLed(boolean on)
//*********************************************************************************
{
    connectionStatus = on;
    digitalWrite(Status::LED_CONN, (connectionStatus) ? HIGH : LOW);
}

void Status::toggleLowPowerLed(boolean on)
//*********************************************************************************
{
    lowPowerStatus = on;
    digitalWrite(Status::LED_LOWP, (lowPowerStatus) ? HIGH : LOW);
}

void Status::readVoltage() 
//*********************************************************************************
{
  adcValue = analogRead(A0);
  voltage = adcValue / 1023.0 * VOLTAGE_FACTOR;
  percent = (voltage - DROPOUT_VOLTAGE) / (FULL_VOLTAGE - DROPOUT_VOLTAGE) * 100;
  if (percent < 0)
  {
      sprintf(batStatus, "Bat: am Netz");
  }
  else
  {
    sprintf(batStatus, "Bat: %3.0f%%", percent);    
  }
  
  Log.debug(MODULE, "A0 value = %d", adcValue);
  Log.info(MODULE, "Battery Voltage = %.2f -- Level = %3.0f%%", voltage, percent);
}

Status statusComponent;