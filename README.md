ESP32 Airplanes
A real-time flight tracking system powered by the ESP32. This project fetches and processes live aviation data to monitor aircraft in your vicinity or globally, displaying key flight metrics on a compact interface.

Features
Real-time Tracking: Connects to Wi-Fi to pull live flight data (via ADS-B or OpenSky API).

Detailed Metrics: Displays flight identifiers, altitude, ground speed, and heading.

Hardware Optimized: Designed for the ESP32 architecture, utilizing its dual-core processing for seamless data fetching and display updates.

Modular Display Support: Compatible with common SPI/I2C displays (e.g., SSD1306 OLED or ST7789 TFT).

Hardware Requirements
ESP32 Development Board (e.g., ESP32-WROOM-32)

Display: 0.96" OLED or 1.8" TFT (optional but recommended)

Power Supply: Micro-USB or Li-Po battery

Software & Libraries
To compile this project, ensure you have the following installed in your Arduino IDE or PlatformIO environment:

Arduino-ESP32

HTTPClient & WiFi: For data retrieval.

ArduinoJson: For parsing flight data packets.

Adafruit GFX & SSD1306/ST7735: For display rendering.

Installation
Clone the Repository:

Bash
git clone https://github.com/BehdadMohammadi/ESP32_Airplanes.git
Configuration: Open the project in your IDE and update the credentials.h (or main sketch) with your Wi-Fi settings:

C++
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
Upload: Connect your ESP32 and hit Upload.

Usage
Once powered on, the ESP32 will connect to the local network and begin polling for aircraft data. If a display is connected, you will see a scrolling list or a detailed view of the nearest flights.

Contributing
Contributions are welcome! If you have ideas for new features or hardware support, feel free to fork the repo and submit a pull request.
