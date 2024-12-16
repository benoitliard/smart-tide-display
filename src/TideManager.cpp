#include "TideManager.h"
#include "Config.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

TideManager::TideManager(WiFiClientSecure& client) 
    : _client(client), _currentLevel(0), _previousLevel(0),
      _minLevel(-2.0), _maxLevel(7.0), _trend(TIDE_UNKNOWN),
      _lastUpdateTime(0) {}

void TideManager::update() {
    fetchData();
}

float TideManager::getCurrentLevel() const {
    return _currentLevel;
}

float TideManager::getMinLevel() const {
    return _minLevel;
}

float TideManager::getMaxLevel() const {
    return _maxLevel;
}

bool TideManager::isHighTide() const {
    return _currentLevel >= HIGH_TIDE_MIN;
}

void TideManager::fetchData() {
    if (WiFi.status() == WL_CONNECTED) {
        // Obtenir le niveau actuel
        float currentLevel = fetchLevelAtTime(0);  // 0 = maintenant
        
        // Si c'est le premier appel, obtenir aussi le niveau précédent
        if (_lastUpdateTime == 0) {
            Serial.println("\nInitializing tide direction...");
            // Obtenir le niveau 15 minutes avant
            float previousLevel = fetchLevelAtTime(-15);
            _previousLevel = previousLevel;
            _currentLevel = currentLevel;
            
            float difference = _currentLevel - _previousLevel;
            Serial.println("\n=== Initial Tide Status ===");
            Serial.printf("Level 15min ago: %.2f m\n", _previousLevel);
            Serial.printf("Current level:   %.2f m\n", _currentLevel);
            Serial.printf("Change:          %.2f m\n", difference);
            
            if (difference > 0.01) {
                _trend = TIDE_RISING;
                Serial.println("Initial Status: 🌊 TIDE IS RISING ⬆️");
            } else if (difference < -0.01) {
                _trend = TIDE_FALLING;
                Serial.println("Initial Status: 🌊 TIDE IS FALLING ⬇️");
            } else {
                _trend = TIDE_UNKNOWN;
                Serial.println("Initial Status: 🌊 TIDE IS STABLE ➡️");
            }
            Serial.println("========================\n");
        } else {
            _previousLevel = _currentLevel;
            _currentLevel = currentLevel;
            // ... reste du code existant pour l'affichage du statut ...
        }
        
        _lastUpdateTime = millis();
    }
}

float TideManager::fetchLevelAtTime(int minutesOffset) {
    char fullURL[200];
    time_t now;
    time(&now);
    now += minutesOffset * 60;  // Ajouter l'offset en secondes
    
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    char timeStr[30];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H%%3A%M%%3A00Z", &timeinfo);
    
    sprintf(fullURL, "%s%s", TIDE_URL, timeStr);
    
    Serial.print("Fetching tide level for: ");
    Serial.println(timeStr);
    
    HTTPClient http;
    http.begin(_client, fullURL);
    
    float level = 0.0;
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
        String payload = http.getString();
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, payload);
        
        if (!error && doc["responseItems"][0]["status"] == "OK") {
            level = doc["responseItems"][0]["waterLevel"].as<float>();
        }
    }
    
    http.end();
    return level;
}

TideManager::TideDirection TideManager::getTideTrend() const {
    return _trend;
}