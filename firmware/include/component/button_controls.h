#ifndef BUTTON_CONTROLS_H
#define BUTTON_CONTROLS_H

#include <Arduino.h>
#include "component/component.h"
#include "config.h"

class ButtonController;

struct btnStateType {
  bool pushButtons[BUTTON_COUNT - 1];
  bool modeButton;
};

class ButtonControls: public Component
{
  public:
    void setup();
    void loop();

    void setController(ButtonController* controller);

  private:
  
    uint16_t state;
    uint16_t eventState;
    unsigned long eventTime;
    uint8_t clicks;
    bool longPress;

    uint16_t gButtonMask = 0;
    uint16_t gButtons[BUTTON_COUNT];

    ButtonController* controller;

    void setupButtons();
    uint16_t readButtons();
    btnStateType buttonStates(uint16_t state);
};

extern ButtonControls buttonControls;

#endif //BUTTON_CONTROLS_H
