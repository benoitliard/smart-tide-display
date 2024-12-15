#pragma once
#include "Config.h"
#include <FastLED.h>

class LedController {
public:
    LedController();
    void init();
    void displayTideLevel(float currentLevel, float minLevel, float maxLevel);
    void displaySwimCondition();
    void clear();

private:
    CRGB _leds[NUM_LEDS];
    void showGradient(CRGB color, int numLeds);
    void pulse();
}; 