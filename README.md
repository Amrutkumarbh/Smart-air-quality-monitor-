🌬️ Smart Air Quality Monitor & Purifier

A dual-microcontroller IoT system that monitors indoor air quality in real time, automatically activates a purifier fan when dangerous gas levels are detected, and streams live data to a cloud dashboard via **Blynk IoT**.

📋 Table of Contents
- [Project Overview](#project-overview)
- [Hardware Components](#hardware-components)
- [System Architecture](#system-architecture)
- [Repository Structure](#repository-structure)
- [Getting Started](#getting-started)
- [Wiring Guide](#wiring-guide)
- [Blynk Dashboard Setup](#blynk-dashboard-setup)
- [How It Works](#how-it-works)
- [Troubleshooting](#troubleshooting)
---

📌 Project Overview

This project continuously monitors indoor air quality using three gas sensors and a temperature/humidity sensor. When any sensor reading exceeds the threshold of **400**, a relay module switches on an exhaust fan to clean the air — and all readings are streamed live to a Blynk cloud dashboard viewable from anywhere.

Why Two Microcontrollers?

| Board | Nickname | Role |
|-------|----------|------|
| **Arduino UNO** | "The Brawn" | 5V power for sensors · Reads all sensors · Controls the relay/fan |
| **ESP32 Dev Board** | "The Brain" | Receives data from Arduino · Uploads live readings to Blynk Cloud via Wi-Fi |

The MQ gas sensors require 5V power and multiple analog pins. The ESP32 runs on 3.3V logic for Wi-Fi. Splitting responsibilities across two boards keeps the design clean and avoids level-shifting complexity.

---

🛒 Hardware Components

| Component | Qty | Purpose |
|-----------|-----|---------|
| Arduino UNO | 1 | Main sensor controller (5V) |
| ESP32 Dev Board | 1 | Wi-Fi communicator (3.3V) |
| DHT11 / DHT22 | 1 | Temperature & Humidity sensor |
| MQ135 | 1 | General Air Quality / CO₂ & VOCs |
| MQ7 | 1 | Carbon Monoxide (CO) |
| MQ2 | 1 | Smoke & Combustible Gas |
| 5V Relay Module | 1 | Electronic switch for the fan |
| Exhaust Fan / Air Purifier | 1 | Purifier triggered automatically |
| Jumper Wires + Breadboard | — | Connections |

---

🏗️ System Architecture

```
SENSORS (5V)          ARDUINO UNO           ESP32             BLYNK CLOUD
──────────────        ────────────          ─────             ───────────
MQ135 · MQ7 · MQ2  →  Reads sensors    →   Receives data  →  Live readings
DHT11 / DHT22          Controls relay       Parses values     V0 – V5 pins
                       Packages data        Uploads to cloud
                       ──────────────────── ─────────────────
                           Serial (9600 baud)    Wi-Fi
```

See [`docs/architecture.md`](assets/architecture.md) for a detailed breakdown.

---
📁 Repository Structure

```
smart-air-quality-monitor/
│
├── README.md                   ← You are here
│
├── arduino/
│   └── arduino_sensor_controller.ino   ← Full Arduino sketch
│
├── esp32/
│   └── esp32_blynk_uploader.ino        ← Full ESP32 sketch
│
├── docs/
│   ├── wiring.md               ← Step-by-step wiring guide
│   ├── blynk_setup.md          ← Blynk dashboard configuration
│   └── troubleshooting.md      ← Common problems & fixes
│
└── assets/
    └── architecture.md         ← System architecture detail
```

---

🚀 Getting Started

1. Install Required Libraries (Arduino IDE)

For **Arduino UNO**:
- `DHT sensor library` by Adafruit
- `SoftwareSerial` (built-in)

For **ESP32**:
- `Blynk` by Volodymyr Shymanskyy
- `WiFi` (built-in with ESP32 board package)

2. Add ESP32 Board to Arduino IDE

Add this URL under **File → Preferences → Additional Board Manager URLs**:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
Then install **esp32 by Espressif Systems** from the Board Manager.

3. Configure Your Credentials

In `esp32/esp32_blynk_uploader.ino`, replace:
```cpp
#define BLYNK_AUTH_TOKEN  "YOUR_AUTH_TOKEN"
char ssid[] = "YOUR_WIFI_NAME";      // 2.4 GHz only!
char pass[] = "YOUR_WIFI_PASSWORD";
```

4. Upload Sketches

- Upload `arduino/arduino_sensor_controller.ino` → **Arduino UNO**
- Upload `esp32/esp32_blynk_uploader.ino` → **ESP32 Dev Board**

5. Wire Everything Up

Follow the full guide in [`docs/wiring.md`](docs/wiring.md).

---
🔌 Wiring Guide

See the detailed guide → [`docs/wiring.md`](docs/wiring.md)

> ⚠️ **Critical:** Do NOT connect Arduino 5V to ESP32. Only share **GND** and the two Serial data wires. Connecting 5V to ESP32 will permanently damage it.

---

☁️ Blynk Dashboard Setup

See the full setup guide → [`docs/blynk_setup.md`](docs/blynk_setup.md)

**Virtual Pin summary:**

| Virtual Pin | Sensor | Shows |
|-------------|--------|-------|
| V0 | MQ135 | Air Quality (CO₂ / VOCs) |
| V1 | MQ7 | Carbon Monoxide level |
| V2 | MQ2 | Smoke / Flammable Gas |
| V3 | DHT | Temperature (°C) |
| V4 | DHT | Humidity (%) |
| V5 | Relay | Fan Status (0=OFF, 1=ON) |

---

⚙️ How It Works

Every **2 seconds**, the system runs this cycle:

1. **READ** — Arduino polls MQ135, MQ7, MQ2, and DHT sensor
2. **EVALUATE** — If any MQ reading > 400 → Fan ON; else Fan OFF
3. **PACKAGE** — Values packed into a comma-separated string e.g. `"250,180,210,26.5,55.0,0"`
4. **TRANSMIT** — Arduino sends string via SoftwareSerial at 9600 baud
5. **RECEIVE** — ESP32 catches the string on Hardware Serial 2
6. **UPLOAD** — ESP32 splits the string and calls `Blynk.virtualWrite()` for V0–V5

---

🔧 Troubleshooting

See the full guide → [`docs/troubleshooting.md`](docs/troubleshooting.md)

| Problem | Quick Fix |
|---------|-----------|
| ESP32 shows gibberish | Add common GND wire between both boards |
| DHT returns 0.0 / NaN | Check DATA wire is on Digital Pin 2 |
| Fan never turns ON | Swap LOW/HIGH in relay code |
| Blynk keeps disconnecting | Increase `delay(2000)` to `delay(3000)` |
| ESP32 won't connect to Wi-Fi | Use 2.4 GHz network only |

---

📄 License

This project is open source and available under the [MIT License](LICENSE).
