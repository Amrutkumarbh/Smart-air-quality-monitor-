🔌 Wiring Guide

> ⚠️ **Critical Safety Warning:** Do NOT connect Arduino 5V to the ESP32. The ESP32 operates at **3.3V logic** and will be permanently damaged by 5V. Only share **GND** and the two Serial data wires between the boards.

---

Phase A — Gas Sensors & DHT to Arduino UNO

All sensors are powered by the Arduino's **5V** pin. Each MQ sensor has both a digital and an analog output — we use only the **analog (A0) output** for smoother, more granular readings.

MQ135 — Air Quality / CO₂

| MQ135 Pin | Arduino Pin | Notes |
|-----------|-------------|-------|
| VCC | 5V | Power |
| GND | GND | Ground |
| A0 (Analog) | A0 | Air quality reading |

MQ7 — Carbon Monoxide (CO)

| MQ7 Pin | Arduino Pin | Notes |
|---------|-------------|-------|
| VCC | 5V | Power |
| GND | GND | Ground |
| A0 (Analog) | A1 | CO reading |

MQ2 — Smoke & Combustible Gas

| MQ2 Pin | Arduino Pin | Notes |
|---------|-------------|-------|
| VCC | 5V | Power |
| GND | GND | Ground |
| A0 (Analog) | A2 | Smoke / gas reading |

DHT11 / DHT22 — Temperature & Humidity

| DHT Pin | Arduino Pin | Notes |
|---------|-------------|-------|
| VCC (+) | 5V | Power |
| GND (-) | GND | Ground |
| DATA (Out) | Digital Pin 2 | Temp & humidity signal |

> 💡 Tip:If your DHT sensor returns `NaN` or `0.0`, add a **10kΩ pull-up resistor** between the DATA pin and VCC.

---

Phase B — Relay Module to Arduino UNO

The relay acts as an electronic switch. Arduino pulls **Pin 8 LOW** to complete the relay circuit and turn the fan on. This is **active-LOW** logic (LOW = ON, HIGH = OFF), which is standard for most relay modules.

| Relay Pin | Arduino Pin | Notes |
|-----------|-------------|-------|
| VCC | 5V | Power for relay coil |
| GND | GND | Ground |
| IN (Signal) | Digital Pin 8 | LOW = Fan ON · HIGH = Fan OFF |

> 💡 If your fan never turns on:** Your relay module may be **active-HIGH**. Try swapping `LOW` and `HIGH` in the Arduino code's relay section.

---

Phase C — Serial Bridge: Arduino ↔ ESP32

This is the most important connection in the whole system. The TX pin of one board **always connects to the RX pin of the other**.

| Wire Role | From (Arduino) | To (ESP32) | Why |
|-----------|----------------|------------|-----|
| Data (TX → RX) | Pin 11 (SoftSerial TX) | Pin 16 (RX2) | Arduino talks, ESP32 listens |
| Data (RX ← TX) | Pin 10 (SoftSerial RX) | Pin 17 (TX2) | ESP32 talks, Arduino listens |
| Common Ground | GND | GND | **MANDATORY** — shared voltage reference |

Why the common GND is mandatory

Without a shared ground, the two boards have no common voltage reference. The digital HIGH/LOW signals on the TX/RX lines are interpreted relative to GND. Without this wire, you will see garbled or no data on the ESP32's Serial Monitor.

---

Complete Pin Reference (Quick Cheat Sheet)

Arduino UNO

| Pin | Connected To |
|-----|-------------|
| 5V | MQ135 VCC, MQ7 VCC, MQ2 VCC, DHT VCC, Relay VCC |
| GND | All sensor GNDs + ESP32 GND |
| A0 | MQ135 analog output |
| A1 | MQ7 analog output |
| A2 | MQ2 analog output |
| Digital 2 | DHT DATA |
| Digital 8 | Relay IN |
| Digital 10 | SoftSerial RX (from ESP32 TX2 Pin 17) |
| Digital 11 | SoftSerial TX (to ESP32 RX2 Pin 16) |

ESP32 Dev Board

| Pin | Connected To |
|-----|-------------|
| GND | Arduino GND (**mandatory**) |
| Pin 16 (RX2) | Arduino Pin 11 (SoftSerial TX) |
| Pin 17 (TX2) | Arduino Pin 10 (SoftSerial RX) |
