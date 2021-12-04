#include <stdarg.h>
#include <Wire.h>
#include <string.h>

#include "component/display.h"
#include "logger.h"
#include "config.h"

#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

#define GROUND_LINE 3
#define Y_ADVANCE 10

Adafruit_SSD1306 device = Adafruit_SSD1306(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire);

#define MODULE "DISPLAY"

bool Display::setup()
//****************************************************************************************
{
    //Wire.begin(5, 4);
    if (!device.begin())
    {
        Log.error(MODULE, "Display could not be started!");
        return false;
    }
    else
    {
        Log.info(MODULE, "Display initialized");
        device.ssd1306_command(SSD1306_DISPLAYON);
        device.clearDisplay();

        device.setTextSize(1);
        maxlines = 3;
        device.setTextWrap(false);
        device.setTextColor(SSD1306_WHITE);

        device.display();
        changed = true;
        return true;
    }
}

void Display::loop()
//****************************************************************************************
{
    if (changed)
    {
        Log.debug(MODULE, "Updating display!");
        device.display();
        device.clearDisplay();
        changed = false;
    }
}

void Display::clear()
//****************************************************************************************
{
    changed = true;
}

void Display::turnOff()
//****************************************************************************************
{
    device.ssd1306_command(SSD1306_DISPLAYOFF);
}

void Display::turnOn()
//****************************************************************************************
{
    device.ssd1306_command(SSD1306_DISPLAYON);
}

bool Display::printto(uint8_t line, const char *text, ...)
//****************************************************************************************
{
    va_list args;
    va_start(args, text);
    char buffer[255];
    vsnprintf(buffer, 254, text, args);
    va_end(args);

    Log.info(MODULE, "%d, %d, %d", GROUND_LINE + (line -1) * Y_ADVANCE, line, maxlines);

    if (line > 0 && line <= maxlines)
    {
        device.setCursor(0, GROUND_LINE + (line -1) * Y_ADVANCE);
        device.print(buffer);
        Log.info(MODULE, "Putting \"%s\" to line %d", buffer, line);
        changed = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool Display::printtoinv(uint8_t line, const char *text)
//****************************************************************************************
{
    bool result = false;
    device.fillRect(0, (line - 1) * Y_ADVANCE, device.width(), line * Y_ADVANCE +1, SSD1306_WHITE);
    device.setTextColor(SSD1306_BLACK);
    result = printto(line, text);
    device.setTextColor(SSD1306_WHITE);
    
    return result;
}

uint16_t Display::printwrap(uint8_t line, const char *text)
//****************************************************************************************
{
    char buf[strlen(text)+1];
    strcpy(buf, text);
    char *ptr = strtok(buf, " ");
    uint16_t xpos = 0;
    uint16_t cur = 0;

    while (ptr != NULL)
    {
        //increasing the cursor by one to indicate the delimiter between words
        //do this here because if the next word is already going beyond the display the
        //space should be already taken into account
        cur++;


        uint16_t w = calcTextWidth(ptr, xpos);
        if ((xpos + w) > device.width())
        {
            ++line;
            if (line > maxlines)
                break;
            xpos = 0;
        }

        device.setCursor(xpos, GROUND_LINE);
        device.print(ptr);
        xpos += (w + spaceWidth);

        cur += strlen(ptr);
        ptr = strtok(NULL, " ");
    }
    changed = true;
    //return the cursor - 1 as we have initially added a space also after the last word
    return --cur;
}

uint16_t Display::calcTextWidth(const char *text, uint16_t xpos)
//****************************************************************************************
{
    int16_t x1, y1;
    uint16_t w, h;
    
    device.getTextBounds(text, xpos, 0, &x1, &y1, &w, &h);
    return w * 1;
}

Display display;