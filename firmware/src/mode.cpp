#include "mode.h"

ModeController::ModeController(Adafruit_SSD1306 *display, uint8_t mode) {
    attachedDisplay = display;
    modeNumber = mode;

    activation = false;
    haveToExecute = false;
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
        updateDisplay = false;
    }
    activation = true;
}

void ModeController::execute() {

    if (activation == true) {
        onActivation();
        activation = false;
    }

    if (haveToExecute == true) {
        haveToExecute = onExecution();
    }

}
 
bool ModeController::display() {
    if ((useDisplay) && (updateDisplay))  {
        attachedDisplay->clearDisplay();

        const char* l1 = showLine1();
        const char* l2 = showLine2();
        const char* l3 = showLine3();
        const char* l4 = showLine4();

        attachedDisplay->setCursor(0,0);
        attachedDisplay->print(l1);
        attachedDisplay->setCursor(0,8);
        attachedDisplay->print(l2);
        attachedDisplay->setCursor(0,16);
        attachedDisplay->print(l3);
        attachedDisplay->setCursor(0,24);
        attachedDisplay->print(l4);

        //addGraphics(attachedDisplay);
        //Serial.println("Added Art");

        attachedDisplay->display();

        updateDisplay = false;
        return true;

    } else {

        return false;
    }
}

bool ModeController::btnPressed(uint8_t btn){

    bool handled = handleButton(btn);
    updateDisplay = true;

    return handled;
}


const char* ModeController::showLine1() {
    return "";
}

const char* ModeController::showLine2() {
    return "";
}

const char* ModeController::showLine3() {
    return "";
}

const char* ModeController::showLine4() {
    return "";
}

void ModeController::needExecution() {
    haveToExecute = true;
}

void ModeController::onActivation() {

};

bool ModeController::onExecution() {
    return false;
}

void ModeController::onLoop() {
    
}

void ModeController::onMessage(const char* topic, const char* command, const char* message) {

}
    
bool ModeController::addGraphics(Adafruit_SSD1306 *display) {
    return false;
}

bool ModeController::handleButton(uint8_t btn) {
    return false;
}
