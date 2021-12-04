#include "component/status.h"

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


Status statusComponent;