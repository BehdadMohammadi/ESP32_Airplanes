# ESP32 Airplanes ✈️

A real-time flight tracking system developed for the **ESP32** platform. This project connects to live aviation APIs to fetch, parse, and display flight data for aircraft in your vicinity.

## 🌟 Overview
This project transforms an ESP32 microcontroller into a portable aviation monitor. It leverages Wi-Fi to pull data from flight tracking services and presents real-time telemetry such as flight numbers, altitudes, and headings.

## 🚀 Features
* **Live Data:** Real-time fetching of flight vectors via API (e.g., OpenSky Network).
* **Efficient Parsing:** Optimized for embedded systems using `ArduinoJson`.
* **Dual-Core Utilization:** Leverages ESP32 power for simultaneous data fetching and UI rendering.
* **Compact Display:** Support for OLED (SSD1306) and TFT displays.

## 🛠️ Hardware Requirements
* **Microcontroller:** ESP32 Development Board.
* **Display:** I2C OLED or SPI TFT screen.
* **Connection:** 2.4GHz Wi-Fi.

## 💻 Installation & Setup
1.  **Clone the Repository**
    ```bash
    git clone [https://github.com/BehdadMohammadi/ESP32_Airplanes.git](https://github.com/BehdadMohammadi/ESP32_Airplanes.git)
    ```
2.  **Configure Credentials**
    Update the Wi-Fi credentials in your code:
    ```cpp
    const char* ssid = "YOUR_SSID";
    const char* password = "YOUR_PASSWORD";
    ```
3.  **Upload**
    Use the Arduino IDE or PlatformIO to flash the firmware to your ESP32.

## 📦 Dependencies
* [ArduinoJson](https://arduinojson.org/)
* [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
* [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)

## 📄 License
This project is open-source. Feel free to contribute or modify for your own tracking needs.
