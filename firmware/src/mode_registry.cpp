#include "mode_registry.h"
#include "Adafruit_SSD1306.h"
#include "Arduino.h"

#include "remote.h"

uint8_t gCurrentMode;
ModeController** gModes;

void initializeModes(Adafruit_SSD1306* display) {

    gModes = new ModeController*[NUM_MODES];

    gModes[0] = new RemoteControl(display, 0);

}

ModeController* currentMode() {
    return gModes[gCurrentMode];
}

uint8_t nextMode() {
    gCurrentMode++;
    if (gCurrentMode == NUM_MODES) {
        gCurrentMode = 0;
    }

    #ifdef SERIAL_PRINT
    Serial.printf("Switching to Mode %d\n", gCurrentMode);
    #endif

    return gCurrentMode;
}
