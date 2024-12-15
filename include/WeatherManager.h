#pragma once
#include <Arduino.h>
#include <WiFiClientSecure.h>

class WeatherManager {
public:
    WeatherManager(WiFiClientSecure& client);
    void update();
    float getTemperature() const;
    bool isRaining() const;

private:
    WiFiClientSecure& _client;
    float _temperature;
    bool _isRaining;
    void fetchData();
}; 