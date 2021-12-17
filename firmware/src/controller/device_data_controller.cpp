#include "controller/device_data_controller.h"
#include "component/button_controls.h"
#include "component/status.h"
#include "logger.h"
#include "component/display.h"
#include "component/ota.h"
#include "config.h"
#include <ESP8266WiFi.h>

#define MODULE "MSG_CTRL"
#define MAX_INDICES 6
#define BATTERY_STATUS 1
#define FIRMWARE_VERSION 2
#define IP_ADDRESS 3
#define MAC_ADDRESS 4
#define WLAN_NAME 5
#define DEVICE_ID 6

DeviceDataController::DeviceDataController(ButtonControls *buttons, ControllerBase *timeout, activation_cb_t cb)
    : ControllerBase(cb), ButtonController(buttons), TimeOutController(DEEP_SLEEP_AFTER), timeoutTarget(timeout),
      label(""), value(""), index(1), expectingOTA(false)
//*********************************************************************************
{
}

void DeviceDataController::activate()
//*********************************************************************************
{
    Log.debug(MODULE, "Device Controller activated");
    attach();
    obtainDeviceData(index);
    restartTimeout();
}

void DeviceDataController::loop()
//*********************************************************************************
{
    if ((0 != label.compareTo("")) && (!expectingOTA))
    {
        display.printtoinv(1, label.c_str());
        display.printto(2, value.c_str());
        label = "";
    }

    if ((checkTimeForTimeOut()) && (!expectingOTA))
    {
        gotoController(timeoutTarget);
    }
}

void DeviceDataController::onClick(btnStateType state)
//*********************************************************************************
{
    restartTimeout();

    if (state.modeButton)
    {
        gotoNext();
    }
    else if (state.pushButtons[0])
    {
        index = (--index == 0) ? MAX_INDICES : index;
        obtainDeviceData(index);
    }
    else if (state.pushButtons[2])
    {
        index = (++index > MAX_INDICES) ? 1 : index;
        obtainDeviceData(index);
    }
    else if (state.pushButtons[9])
    {
        expectingOTA = !expectingOTA;
        if (expectingOTA)
        {
            display.printtoinv(1, "Update");
        }
        else
        {
            obtainDeviceData(index);
        }
    }
}

void DeviceDataController::obtainDeviceData(uint8_t ix)
{
    switch (ix)
    {

    case BATTERY_STATUS:
        label = "Batterie";
        value = statusComponent.getBatteryStatus();
        break;

    case FIRMWARE_VERSION:
        label = "Firmware";
        value = VERSION;
        break;

    case IP_ADDRESS:
        label = "IP Adresse";
        value = WiFi.localIP().toString();
        break;

    case MAC_ADDRESS:
        label = "MAC Adresse";
        value = WiFi.macAddress();
        break;

    case WLAN_NAME:
        label = "WLAN";
        value = WiFi.SSID();
        break;

    case DEVICE_ID:
        label = "Name";
        value = DEVICE_NAME;
        break;
    }
}