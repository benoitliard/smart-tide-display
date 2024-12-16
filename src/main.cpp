#include <Arduino.h>
#include "Config.h"
#include "TideManager.h"
#include "WeatherManager.h"
#include "LedController.h"

WiFiClientSecure client;
TideManager tideManager(client);
WeatherManager weatherManager(client);
LedController ledController;

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== Starting ===");
    
    // WiFi configuration
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
    
    // SSL and time configuration
    client.setInsecure();
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    
    // Wait for NTP sync
    Serial.print("Waiting for NTP sync");
    while (time(nullptr) < 1000000000) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nNTP Synchronized!");
    
    // LED initialization
    ledController.init();
    Serial.println("LEDs initialized");
    
    // First API calls
    Serial.println("First API calls...");
    tideManager.update();
    weatherManager.update();
    Serial.println("=== Startup complete ===\n");
}

void loop() {
    static unsigned long lastUpdate = 0;
    static unsigned long lastDisplay = 0;
    unsigned long currentMillis = millis();
    
    // Mise à jour des données toutes les minutes
    if (currentMillis - lastUpdate > 60000) {
        Serial.println("\n=== Periodic Update ===");
        tideManager.update();
        weatherManager.update();
        lastUpdate = currentMillis;
        Serial.println("=== Update Complete ===\n");
    }
    
    // Mise à jour de l'affichage plus fréquemment (toutes les 20ms)
    if (currentMillis - lastDisplay > 20) {
        if (weatherManager.getTemperature() >= SWIM_TEMP_MIN && 
            tideManager.isHighTide() && 
            !weatherManager.isRaining()) {
            ledController.displaySwimCondition();
        } else {
            ledController.displayTideLevel(
                tideManager.getCurrentLevel(),
                tideManager.getMinLevel(),
                tideManager.getMaxLevel(),
                tideManager.getTideTrend()
            );
        }
        lastDisplay = currentMillis;
    }
    
    delay(10);
} 