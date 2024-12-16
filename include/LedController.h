#pragma once
#include "Config.h"
#include <FastLED.h>
#include "TideManager.h"

class LedController {
public:
    LedController();
    void init();
    void displayTideLevel(float currentLevel, float minLevel, float maxLevel, TideManager::TideDirection trend);
    void displaySwimCondition();
    void clear();

private:
    CRGB _leds[NUM_LEDS];
    void showGradient(CRGB color, int numLeds, TideManager::TideDirection trend);
    void pulse();
}; 