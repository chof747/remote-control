#ifndef MODE_H
#define MODE_H

#include <Arduino.h>
#include "Adafruit_SSD1306.h"

class ModeController {

public:

    ModeController(Adafruit_SSD1306 *display, uint8_t mode);

    void activate();
    void execute();
    bool display();
    bool btnPressed(uint8_t btn);

    uint8_t getModeNumber();
    void setModeNumber(uint8_t mode);
    virtual void onMessage(const char* topic, const char* command, const char* message);

protected: 

    virtual const char* showLine1();
    virtual const char* showLine2();
    virtual bool addGraphics(Adafruit_SSD1306 *display);
    
    virtual bool handleButton(uint8_t btn);
    virtual void onActivation();
    virtual bool onExecution();
    virtual void enableDisplay(bool enable);

    void needExecution();

    uint8_t modeNumber;
    bool    updateDisplay;

private:

    bool useDisplay;
    bool haveToExecute;
    bool activation;
    Adafruit_SSD1306 *attachedDisplay;

};

#endif //MODE_H