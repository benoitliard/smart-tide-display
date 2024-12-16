#include "LedController.h"

LedController::LedController() {}

void LedController::init() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(_leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
}

void LedController::displayTideLevel(float currentLevel, float minLevel, float maxLevel, TideManager::TideDirection trend) {
    float percentage = (currentLevel - minLevel) / (maxLevel - minLevel);
    int ledsToLight = int(percentage * NUM_LEDS);
    
    CRGB color;
    if (currentLevel > HIGH_TIDE_MAX) {
        color = CRGB::Red;
    } else if (currentLevel > NORMAL_TIDE_MAX) {
        color = CRGB::Yellow;
    } else {
        color = CRGB::Green;
    }
    
    showGradient(color, ledsToLight, trend);
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

void LedController::showGradient(CRGB color, int numLeds, TideManager::TideDirection trend) {
    clear();
    
    // Paramètres pour l'effet de fondu
    const uint8_t MIN_BRIGHTNESS = 20;
    const uint8_t MAX_BRIGHTNESS = 255;
    const float CYCLE_DURATION = 5000.0;  // Durée d'un cycle complet en ms (5 secondes)
    
    // Calculer la phase actuelle du cycle (0.0 à 1.0)
    float phase = (millis() % (unsigned long)CYCLE_DURATION) / CYCLE_DURATION;
    
    for(int i = 0; i < numLeds && i < NUM_LEDS; i++) {
        _leds[i] = color;
        float position = float(i) / float(NUM_LEDS);  // Position relative de la LED (0.0 à 1.0)
        
        if (trend == TideManager::TIDE_RISING) {
            // Pour marée montante: vague de luminosité qui monte
            float offset = position - phase;
            if (offset < 0) offset += 1.0;  // Wrap around
            uint8_t brightness = map(offset * 255, 0, 255, MAX_BRIGHTNESS, MIN_BRIGHTNESS);
            _leds[i].nscale8(brightness);
            
        } else if (trend == TideManager::TIDE_FALLING) {
            // Pour marée descendante: vague de luminosité qui descend
            float offset = position + phase;
            if (offset > 1.0) offset -= 1.0;  // Wrap around
            uint8_t brightness = map(offset * 255, 0, 255, MAX_BRIGHTNESS, MIN_BRIGHTNESS);
            _leds[i].nscale8(brightness);
            
        } else {
            // Pas de mouvement si tendance inconnue
            _leds[i].nscale8((MIN_BRIGHTNESS + MAX_BRIGHTNESS) / 2);
        }
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