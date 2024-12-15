#pragma once
#include <Arduino.h>
#include <WiFiClientSecure.h>

class TideManager {
public:
    TideManager(WiFiClientSecure& client);
    void update();
    float getCurrentLevel() const;
    float getMinLevel() const;
    float getMaxLevel() const;
    bool isHighTide() const;

private:
    WiFiClientSecure& _client;
    float _currentLevel;
    float _minLevel;
    float _maxLevel;
    void fetchData();
}; 