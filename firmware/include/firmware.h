#ifndef FIRMWARE_H
#define FIRMWARE_H

#include "mode.h"


class FirmwareController: public ModeController {

public:

    FirmwareController(Adafruit_SSD1306 *display, uint8_t mode);

protected:

    const char* showLine1() override;
    const char* showLine2() override;
    void onActivation() override;
    void onMessage(const char* topic, const char* command, const char* message) override; 
    bool handleButton(uint8_t btn) override;

private:

    void setLines();
    void provideDeviceInfo();
    bool onExecution() override;
    void performOTAUpdate(const char* url);

    uint8_t mode;
    char caption[12];
    char value[20];
    bool triggerUpload;
};

#endif //FIRMWARE_H