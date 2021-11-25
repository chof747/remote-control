#ifndef BUTTON_CONTROLLER_H
#define BUTTON_CONTROLLER_H

#include <Arduino.h>
#include "component/button_controls.h"

class ButtonController
{
    public:

    ButtonController(ButtonControls* buttons);

    virtual void onClick(btnStateType state) {}
    virtual void onLongPress(btnStateType state) {}
    virtual void onDblClick(btnStateType state) {}

    protected:

    void attach();

    private:

    ButtonControls* buttons;
};


#endif //BUTTON_CONTROLLER_H