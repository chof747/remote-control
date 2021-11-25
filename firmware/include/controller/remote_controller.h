#ifndef REMOTE_CONTROLLER_H
#define REMOTE_CONTROLLER_H

#include "./base.h"
#include "./button_controller.h"
#include "./timeout_controller.h"
#include <String.h>

class RemoteController: public ControllerBase, ButtonController, TimeOutController
{
    public:

    RemoteController(ButtonControls* buttons, ControllerBase* timeout, activation_cb_t cb);
    void activate();
    void loop();

    void onClick(btnStateType state);
    
    private:

    ControllerBase* timeoutTarget;
};

#endif //MESSAGE_CONTROLLER_H

