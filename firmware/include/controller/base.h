#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

class ControllerBase;
typedef void(*activation_cb_t)(ControllerBase* active);

class ControllerBase 
{
    public:
    
    ControllerBase(activation_cb_t cb);
    
    virtual void activate() = 0;
    virtual void loop() = 0;
    virtual void deactivate() {}

    void setNext(ControllerBase* c);
    void setAlternateNext(ControllerBase* c);


    protected:

    void gotoNext();
    void gotoAlternateNext();
    void gotoController(ControllerBase* controller);

    private:

    ControllerBase* next;
    ControllerBase* alternateNext;

    activation_cb_t activationCallBack;

};

#endif //CONTROLLER_H