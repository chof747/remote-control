#ifndef MODE_H
#define MODE_H

#include "Adafruit_SSD1306.h"

class ModeController {

public:

    ModeController(Adafruit_SSD1306 *display, uint8_t mode);

    void activate();
    bool display();
    bool btnPressed(uint8_t btn);

    uint8_t getModeNumber();
    void setModeNumber(uint8_t mode);

protected: 

    virtual const char* showLine1();
    virtual const char* showLine2();
    virtual bool addGraphics(Adafruit_SSD1306 *display);
    virtual bool handleButton(uint8_t btn);
    virtual void enableDisplay(bool enable);

    uint8_t modeNumber;

private:

    bool    useDisplay;
    Adafruit_SSD1306 *attachedDisplay;

};

#endif //MODE_H