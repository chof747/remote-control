#ifndef REMOTE_H
#define REMOTE_H

#include "config.h"
#include "mode.h"

class RemoteControl: public ModeController {

public:

    RemoteControl(Adafruit_SSD1306 *display, uint8_t mode);
    ~RemoteControl();

protected:

    virtual bool handleButton(uint8_t btn) override;
    const char* showLine1() override;
    void onActivation() override;
    bool onExecution() override;

private:
    char* message;
    int   pressMsgTimer;

};

#endif //REMOTE_H