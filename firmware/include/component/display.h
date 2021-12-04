#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "component/component.h"

class Display: public Component
{
    public:        
        bool setup();
        void loop();
        
        bool printto(uint8_t line, const char* text, ...);
        bool printtoinv(uint8_t line, const char* text);
        uint16_t printwrap(uint8_t lineStart, const char* text);
  

        uint8_t getMaxLines();
        void clear();

        void turnOff();
        void turnOn();

    private:
        bool changed;
        GFXfont fontData;
        uint8_t maxlines;
        uint16_t spaceWidth;

        uint16_t calcTextWidth(const char* text, uint16_t xpos);
};

extern Display display;

#endif //DISPLAY_H

