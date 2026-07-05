# ESP32-Bluetooth-DHT11-LED-OLED

# IoT Control System - ESP32

Real-time temperature and humidity monitoring with Bluetooth LED control
and OLED display. Built with object-oriented programming (C++ classes),
FreeRTOS tasks running in parallel, and non-blocking code using millis()
instead of delay().

## Features
- Real-time temperature and humidity monitoring via DHT11 sensor
- Bluetooth control of LED brightness from any mobile device
- Auto mode: LED fades in and out automatically
- Manual mode: brightness controlled via Bluetooth commands
- OLED display showing current mode and brightness percentage
- FreeRTOS — 3 parallel tasks running simultaneously on dual-core ESP32
- Non-blocking code — no delay() used in main logic

## Commands (via Bluetooth)
| Command | Action |
|---------|--------|
| ON | LED at maximum brightness |
| OFF | LED off |
| AUTO | Automatic fade mode |
| 0-255 | Set specific brightness value |

## Components
- ESP32
- DHT11 temperature and humidity sensor
- HC-05 Bluetooth module
- LED + resistor
- OLED display SSD1306 0.96" I2C

## Pin Connections
| Component | ESP32 Pin |
|-----------|-----------|
| DHT11 DATA | 4 |
| LED | 5 |
| HC-05 RX | 17 (TX1) |
| HC-05 TX | 16 (RX1) |
| OLED SDA | 21 |
| OLED SCL | 22 |

## How it works
The system uses two classes: LED and Sensor.
Three FreeRTOS tasks run in parallel on the ESP32 dual-core processor:
- tareaSensor (Core 0): reads DHT11 every 2 seconds and sends data via Bluetooth
- tareaLED (Core 1): receives Bluetooth commands and controls LED brightness
- tareaOLED (Core 1): updates display with current mode and brightness

## Libraries Required
- Adafruit SSD1306
- Adafruit GFX
- DHT sensor library (Adafruit)

## Video
[Watch here](https://youtu.be/9QNKH52WMH0?si=twLiq8GxPrmGNPWh)

## Known Issues
- HC-05 TX outputs 3.3V (verified with multimeter) making direct
  ESP32 connection safe without voltage divider
