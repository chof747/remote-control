#ifndef MODE_REGISTRY_H
#define MODE_REGISTRY_H

#include "mode.h"

#define NUM_MODES   4

class Adafruit_SSD1306;

void initializeModes(Adafruit_SSD1306* display);
ModeController* currentMode();
uint8_t nextMode();
void mqttCallback(char *topic, byte *payload, uint8_t length);


#endif //MODE_REGISTRY_H