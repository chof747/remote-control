#include "mode.h"

ModeController::ModeController(Adafruit_SSD1306 *display, uint8_t mode) {
    attachedDisplay = display;
    modeNumber = mode;
}

void ModeController::setModeNumber(uint8_t mode) {
    modeNumber = mode;
}

void ModeController::enableDisplay(bool enable) {
    useDisplay = enable;
}

uint8_t ModeController::getModeNumber() {
    return modeNumber;
}

void ModeController::activate() {
    if (useDisplay) {
        attachedDisplay->ssd1306_command(SSD1306_DISPLAYON);
    } else {
        attachedDisplay->ssd1306_command(SSD1306_DISPLAYOFF);
    }
}
 
bool ModeController::display() {
    if (useDisplay) {
        attachedDisplay->clearDisplay();

        attachedDisplay->setCursor(0,0);
        attachedDisplay->print(showLine1());

        attachedDisplay->setCursor(0,17);
        attachedDisplay->print(showLine2());

        addGraphics(attachedDisplay);

        attachedDisplay->display();

        return true;

    } else {

        return false;
    }
}

bool ModeController::btnPressed(uint8_t btn){

    bool handled = handleButton(btn);
    display();

    return handled;
}


const char* ModeController::showLine1() {
    return "";
}

const char* ModeController::showLine2() {
    return "";
}
    
bool ModeController::addGraphics(Adafruit_SSD1306 *display) {
    return false;
}

bool ModeController::handleButton(uint8_t btn) {
    return false;
}
