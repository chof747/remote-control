#ifndef FIRMWARE_H
#define FIRMWARE_H

#include "mode.h"

class FirmwareController: public ModeController {

public:

    FirmwareController(Adafruit_SSD1306 *display, uint8_t mode);

protected:

    const char* showLine1() override;
    void onActivation() override;
    void onMessage(const char* topic, const char* command, const char* message) override; 

private:

    void provideDeviceInfo();
    void performOTAUpdate();
};

#endif //FIRMWARE_H