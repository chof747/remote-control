#ifndef IDLE_CONTROLLER_H
#define IDLE_CONTROLLER_H

#include "controller/base.h"
#include "controller/button_controller.h"

class IdleController: public ControllerBase
{
    public:

    IdleController(activation_cb_t cb);

    void activate();
    void loop();
};

#endif //IDLE_CONTROLLER_H