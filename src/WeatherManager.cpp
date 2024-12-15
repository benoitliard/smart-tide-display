#include "WeatherManager.h"
#include "Config.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

WeatherManager::WeatherManager(WiFiClientSecure& client) 
    : _client(client), _temperature(0), _isRaining(false) {}

void WeatherManager::update() {
    fetchData();
}

float WeatherManager::getTemperature() const {
    return _temperature;
}

bool WeatherManager::isRaining() const {
    return _isRaining;
}

void WeatherManager::fetchData() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        String fullURL = String(WEATHER_URL) + WEATHER_API_KEY;
        Serial.println("\n=== Updating weather data ===");
        Serial.print("URL: ");
        Serial.println(fullURL);
        
        http.begin(_client, fullURL.c_str());
        http.addHeader("User-Agent", "ESP32");
        
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
                _temperature = doc["main"]["temp"].as<float>();
                _isRaining = doc["weather"][0]["main"].as<String>() == "Rain";
                
                Serial.println("\n=== Current weather conditions ===");
                Serial.print("Temperature: ");
                Serial.print(_temperature);
                Serial.println("°C");
                Serial.print("Conditions: ");
                Serial.println(doc["weather"][0]["description"].as<String>());
                Serial.print("Humidity: ");
                Serial.print(doc["main"]["humidity"].as<int>());
                Serial.println("%");
                Serial.print("Rain: ");
                Serial.println(_isRaining ? "Yes" : "No");
                Serial.println("==============================");
            } else {
                Serial.print("JSON Error: ");
                Serial.println(error.c_str());
            }
        } else {
            Serial.print("HTTP Error: ");
            Serial.println(httpResponseCode);
        }
        
        http.end();
        Serial.println("=== Weather data update complete ===\n");
    } else {
        Serial.println("WiFi not connected!");
    }
}

// ... reste de l'implémentation ... 