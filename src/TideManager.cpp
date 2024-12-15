#include "TideManager.h"
#include "Config.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

TideManager::TideManager(WiFiClientSecure& client) 
    : _client(client), _currentLevel(0), _minLevel(-2.0), _maxLevel(7.0) {}

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
        HTTPClient http;
        
        // Create URL with current timestamp
        char fullURL[200];
        time_t now;
        time(&now);
        struct tm timeinfo;
        gmtime_r(&now, &timeinfo);
        char timeStr[30];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H%%3A%M%%3A00Z", &timeinfo);
        
        sprintf(fullURL, "%s%s", TIDE_URL, timeStr);
        
        Serial.println("\n=== Updating tide data ===");
        Serial.print("URL: ");
        Serial.println(fullURL);
        
        http.begin(_client, fullURL);
        
        int httpResponseCode = http.GET();
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        
        if (httpResponseCode > 0) {
            String payload = http.getString();
            Serial.println("Response received:");
            Serial.println(payload);
            
            DynamicJsonDocument doc(2048);
            DeserializationError error = deserializeJson(doc, payload);
            
            if (!error) {
                if (doc["responseItems"][0]["status"] == "OK") {
                    _currentLevel = doc["responseItems"][0]["waterLevel"].as<float>();
                    Serial.print("Current tide level: ");
                    Serial.println(_currentLevel);
                } else {
                    Serial.println("Non-OK status in response");
                    Serial.println(doc["responseItems"][0]["status"].as<const char*>());
                }
            } else {
                Serial.print("JSON Error: ");
                Serial.println(error.c_str());
            }
        } else {
            Serial.print("HTTP Error: ");
            Serial.println(httpResponseCode);
        }
        
        http.end();
        Serial.println("=== Tide data update complete ===\n");
    } else {
        Serial.println("WiFi not connected!");
    }
}