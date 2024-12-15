#include "LedController.h"

LedController::LedController() {}

void LedController::init() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(_leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
}

void LedController::displayTideLevel(float currentLevel, float minLevel, float maxLevel) {
    // Calculer le nombre de LEDs à allumer
    float percentage = (currentLevel - minLevel) / (maxLevel - minLevel);
    int ledsToLight = int(percentage * NUM_LEDS);
    
    // Déterminer la couleur en fonction du niveau
    CRGB color;
    if (currentLevel > HIGH_TIDE_MAX) {
        color = CRGB::Red;
    } else if (currentLevel > NORMAL_TIDE_MAX) {
        color = CRGB::Yellow;
    } else {
        color = CRGB::Green;
    }
    
    showGradient(color, ledsToLight);
}

void LedController::displaySwimCondition() {
    static uint8_t pulseValue = 0;
    static bool increasing = true;
    
    pulse();
}

void LedController::clear() {
    fill_solid(_leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
}

void LedController::showGradient(CRGB color, int numLeds) {
    // Éteindre toutes les LEDs
    clear();
    
    // Allumer les LEDs avec gradient
    for(int i = 0; i < numLeds && i < NUM_LEDS; i++) {
        uint8_t brightness = map(i, 0, NUM_LEDS, 10, 80);
        _leds[i] = color;
        _leds[i].nscale8(brightness);
    }
    
    FastLED.show();
}

void LedController::pulse() {
    static uint8_t pulseValue = 0;
    static bool increasing = true;
    
    // Calculer la luminosité de pulsation
    if (increasing) {
        pulseValue = pulseValue + PULSE_SPEED;
        if (pulseValue >= 80) increasing = false;
    } else {
        pulseValue = pulseValue - PULSE_SPEED;
        if (pulseValue <= 10) increasing = true;
    }
    
    // Appliquer la pulsation
    fill_solid(_leds, NUM_LEDS, CRGB::Blue);
    for(int i = 0; i < NUM_LEDS; i++) {
        _leds[i].nscale8(pulseValue);
    }
    
    FastLED.show();
}

// ... reste de l'implémentation ... 