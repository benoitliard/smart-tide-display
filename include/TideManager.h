#pragma once
#include <Arduino.h>
#include <WiFiClientSecure.h>

class TideManager {
public:
    enum TideDirection {
        TIDE_RISING,
        TIDE_FALLING,
        TIDE_UNKNOWN
    };

    TideManager(WiFiClientSecure& client);
    void update();
    float getCurrentLevel() const;
    float getMinLevel() const;
    float getMaxLevel() const;
    bool isHighTide() const;
    TideDirection getTideTrend() const;

private:
    WiFiClientSecure& _client;
    float _currentLevel;
    float _previousLevel;
    float _minLevel;
    float _maxLevel;
    TideDirection _trend;
    unsigned long _lastUpdateTime;
    void fetchData();
    float fetchLevelAtTime(int minutesOffset);
}; 