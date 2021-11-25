#include "controller/timeout_controller.h"

#include <Arduino.h>

TimeOutController::TimeOutController(unsigned long timeout)
//*********************************************************************************
{
    this->timeout = timeout;
    timerTime = 0;
    restartTimeout();
}

void TimeOutController::restartTimeout()
//*********************************************************************************
{
    referenceTimeOut = millis();
}

bool TimeOutController::checkTimeForTimeOut()
//*********************************************************************************
{
    return ((millis() - referenceTimeOut) > timeout);
}

void TimeOutController::startTimer(unsigned long time)
//*********************************************************************************
{
    timerTime = time;
    referenceTimer = millis();
}

bool TimeOutController::checkTimeForTimer()
//*********************************************************************************
{
    if ((timerTime)  && ((millis() - referenceTimer) > timerTime))
    {
        timerTime = 0;
        return true;
    }
    else
    {
        return false;
    }
}