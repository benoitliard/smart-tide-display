#pragma once

// WiFi configuration
const char* const WIFI_SSID = "YOUR_WIFI_SSID";
const char* const WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// LED configuration
#define LED_PIN     13
#define NUM_LEDS    60
#define BRIGHTNESS  25

// Threshold configuration
#define NORMAL_TIDE_MAX 5.0    // Maximum level for normal tide
#define HIGH_TIDE_MAX 6.0      // Maximum level for high tide
#define SWIM_TEMP_MIN 20.0     // Minimum temperature for swimming
#define HIGH_TIDE_MIN 4.5      // Minimum level to consider high tide
#define PULSE_SPEED 3          // Pulse speed

// API configuration
const char* const WEATHER_API_KEY = "YOUR_API_KEY";
const char* const WEATHER_URL = "https://api.openweathermap.org/data/2.5/weather?lat=47.4500&lon=-70.3667&units=metric&appid=";
const char* const TIDE_URL = "https://api-spine.azure.cloud-nuage.dfo-mpo.gc.ca/rest/v1/waterLevel?lat=47.4500&lon=-70.3667&t="; 