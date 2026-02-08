# ESP32 Airplanes ✈️

A real-time flight tracking system developed for the **ESP32** platform. This project connects to live aviation APIs to fetch, parse, and display flight data for aircraft in your vicinity.

## 🌟 Overview
This project transforms an ESP32 microcontroller into a portable aviation monitor. It leverages Wi-Fi to pull data from flight tracking services and presents real-time telemetry such as flight numbers, altitudes, and headings.

## 🚀 Features
* **Live Data:** Real-time fetching of flight vectors via API (e.g., OpenSky Network).
* **Efficient Parsing:** Optimized for embedded systems using `ArduinoJson`.
* **Dual-Core Utilization:** Leverages ESP32 power for simultaneous data fetching and UI rendering.

## 🛠️ Hardware Requirements
* **Microcontroller:** ESP32 Development Board.
* **Connection:** 2.4GHz Wi-Fi.

## 💻 Installation & Setup
**Clone the Repository**
    ```bash
    git clone [https://github.com/BehdadMohammadi/ESP32_Airplanes.git](https://github.com/BehdadMohammadi/ESP32_Airplanes.git)
    ```
2.  **Configure Credentials**
    You need to create a file named `project_config.h` in your project folder and define the following variables with your specific data:

    ```cpp
    #define MY_SSID       "Your_WiFi_Name"
    #define MY_PASSWORD   "Your_WiFi_Password"

    // Home Location
    #define MY_HOME_LAT    00.0000 
    #define MY_HOME_LON    00.0000 

    // API Bounding Box (Roughly 50km around home)
    #define LAT_MIN       "00.00"
    #define LAT_MAX       "00.00"
    #define LON_MIN       "00.00"
    #define LON_MAX       "00.00"

    #define TELEGRAM_TOKEN   "your_bot_token"
    #define TELEGRAM_CHAT_ID "your_chat_id"
    ```
    
    3.  **Upload**
    Use the ESP-IDF to flash the firmware to your ESP32.

## 📄 License
This project is open-source. Feel free to contribute or modify for your own tracking needs.
