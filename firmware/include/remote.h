#ifndef REMOTE_H
#define REMOTE_H

#include "mode.h"

class RemoteControl: public ModeController {

public:

    RemoteControl(Adafruit_SSD1306 *display, uint8_t mode);

protected:

    virtual bool handleButton(uint8_t btn) override;
    const char* showLine1() override;
    void onActivation() override;

};

#endif //REMOTE_H