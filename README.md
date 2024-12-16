# 🌊 Smart Tide Display

An ESP32-based LED strip project that displays real-time tide levels and beach conditions for Baie-Saint-Paul, Quebec. Perfect for beach lovers and water sports enthusiasts!

![Tide Display Demo](demo.gif) <!-- Vous pourriez ajouter une image/gif de démonstration -->

## ✨ Features

- 🌊 Real-time tide level visualization using LED strip
- 🌡️ Weather conditions monitoring
- 🏊‍♂️ Smart swimming conditions indicator
- 🎨 Color-coded tide levels:
  - 🟢 Normal tide (Green)
  - 🟡 High tide (Yellow)
  - 🔴 Very high tide (Red)
- 💫 Dynamic tide direction indication:
  - Rising tide: Smooth upward light animation
  - Falling tide: Smooth downward light animation
  - Initial direction detection using historical data
- 💫 Special pulsing effect when conditions are perfect for swimming

## 🛠️ Hardware Requirements

- ESP32 development board
- WS2812B LED strip (60 LEDs)
- 5V power supply
- Basic wiring components

## 📋 Prerequisites

- [PlatformIO](https://platformio.org/) with VS Code
- [OpenWeatherMap API key](https://openweathermap.org/api)
- WiFi connection

## 🚀 Getting Started

1. Clone this repository:

## 📍 Location Configuration

### Changing Location

The project is configured by default for Baie-Saint-Paul, Quebec, but you can adapt it for any coastal location:

1. Find your station ID for tide data:
   - Visit [DFO Water Level Station Search](https://www.tides.gc.ca/en/stations)
   - Search for your nearest station
   - Note the station ID from the URL (e.g., "2995" for Saint-Joseph-de-la-Rive)

2. Update coordinates for weather data:
   - Find your latitude and longitude (e.g., using Google Maps)
   - Update both APIs URLs in `Config.h`:
