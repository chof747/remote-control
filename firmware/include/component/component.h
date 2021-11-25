#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{

    public:
        virtual void setup() = 0;
        virtual void afterSetup() {}
        virtual void loop()  = 0;

        virtual bool powerOnTest() { return true; }

};

#endif //COMPONENT_G