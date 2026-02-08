ESP32 Airplanes ✈️
A lightweight, ESP32-based flight tracking application. This project leverages the ESP32's Wi-Fi capabilities to fetch real-time aviation data, allowing users to monitor flights, altitudes, and headings directly from an embedded system.

🚀 Features
Live Data Fetching: Connects to aviation APIs to retrieve real-time flight information.

Embedded Processing: Efficiently parses JSON data using the ESP32's dual-core architecture.

Visual Output: Designed to interface with OLED or TFT displays for portable monitoring.

Low Power: Optimized for battery-operated or long-term stationary tracking.

🛠 Hardware Requirements
Microcontroller: ESP32 (WROOM-32 or similar).

Display: SSD1306 OLED (I2C) or ST7789 TFT (SPI).

Connection: Stable 2.4GHz Wi-Fi network.

💻 Software Setup
Clone the project:

Bash
git clone https://github.com/BehdadMohammadi/ESP32_Airplanes.git
Install Libraries: Ensure you have the following libraries installed in your Arduino IDE or PlatformIO:

WiFi.h & HTTPClient.h

ArduinoJson

Adafruit_GFX & Adafruit_SSD1306 (for OLED)

Configure Wi-Fi: Update the SSID and Password in the main sketch to match your local network credentials.

📂 Project Structure
/src - Main source code and logic.

/include - Header files and configuration.

README.md - Project documentation.

📝 License
This project is open-source. Feel free to fork, modify, and share!
