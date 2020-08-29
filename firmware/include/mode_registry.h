#ifndef MODE_REGISTRY_H
#define MODE_REGISTRY_H

#include "mode.h"

#define NUM_MODES   3

class Adafruit_SSD1306;

void initializeModes(Adafruit_SSD1306* display);
ModeController* currentMode();
uint8_t nextMode();

#endif //MODE_REGISTRY_H