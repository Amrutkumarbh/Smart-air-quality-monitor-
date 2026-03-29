🏗️ System Architecture

Overview

This project uses a **dual-microcontroller architecture** to cleanly separate sensor reading (which needs 5V) from Wi-Fi communication (which needs 3.3V logic).

```
┌──────────────────────────────────────────────────────────────────┐
│                        SENSOR LAYER (5V)                         │
│   MQ135 (CO₂/VOCs)  ──┐                                          │
│   MQ7   (CO)        ──┤──→ Arduino A0, A1, A2                    │
│   MQ2   (Smoke)     ──┘                                          │
│   DHT11/22 (Temp/Hum) ──→ Arduino Digital Pin 2                  │
└───────────────────────────────┬──────────────────────────────────┘
                                │
                                ▼
┌──────────────────────────────────────────────────────────────────┐
│                     ARDUINO UNO — "The Brawn"                    │
│                                                                  │
│  1. Reads analog/digital sensor values every 2 seconds           │
│  2. Compares each MQ value against threshold (400)               │
│  3. Triggers relay HIGH/LOW → Fan ON/OFF                         │
│  4. Packages 6 values into CSV string:                           │
│     "mq135,mq7,mq2,temp,humidity,fanStatus"                      │
│  5. Sends string via SoftwareSerial (Pin 11) at 9600 baud        │
│                                                                  │
│  Controls → RELAY MODULE → EXHAUST FAN / AIR PURIFIER           │
└───────────────────────────────┬──────────────────────────────────┘
                                │
                    Serial TX/RX + common GND
                    (Pins 10/11 ↔ Pins 16/17)
                                │
                                ▼
┌──────────────────────────────────────────────────────────────────┐
│                     ESP32 DEV BOARD — "The Brain"                │
│                                                                  │
│  1. Maintains connection to Blynk cloud via Wi-Fi (2.4 GHz)     │
│  2. Listens on Hardware Serial 2 (Pin 16) for Arduino data       │
│  3. Parses CSV string into 6 typed variables using sscanf()      │
│  4. Calls Blynk.virtualWrite() for V0–V5                         │
│  5. Calls Blynk.run() every loop to keep connection alive        │
└───────────────────────────────┬──────────────────────────────────┘
                                │
                              Wi-Fi
                                │
                                ▼
┌──────────────────────────────────────────────────────────────────┐
│                         BLYNK CLOUD                              │
│                                                                  │
│  V0 → MQ135 Air Quality     V3 → Temperature (°C)               │
│  V1 → MQ7 CO Level          V4 → Humidity (%)                    │
│  V2 → MQ2 Smoke Level       V5 → Fan Status (0/1)               │
│                                                                  │
│  Accessible from any device via Blynk app or web dashboard       │
└──────────────────────────────────────────────────────────────────┘
```

---

Data Flow — Step by Step

Every 2 seconds, the system completes one full cycle:

| Step | Actor | Action |
|------|-------|--------|
| 1 | Arduino | Reads MQ135 (A0), MQ7 (A1), MQ2 (A2), DHT11 temp & humidity |
| 2 | Arduino | If any MQ value > 400 → Pin 8 LOW (Fan ON); else Pin 8 HIGH (Fan OFF) |
| 3 | Arduino | Packs all 6 values into string: `"250,180,210,26.5,55.0,0"` |
| 4 | Arduino | Sends string via SoftwareSerial Pin 11 at 9600 baud |
| 5 | ESP32 | Receives string on Hardware Serial 2 Pin 16 |
| 6 | ESP32 | Splits string with `sscanf()` into 6 typed variables |
| 7 | ESP32 | Calls `Blynk.virtualWrite()` for V0 through V5 |

---

Why Serial Communication?

The Arduino and ESP32 communicate using UART serial (TX/RX wires) because:
- It requires only 2 data wires + GND — minimal hardware
- It's universally supported on all microcontrollers
- The message format (CSV string) is human-readable and easy to debug
- No extra libraries or protocols needed beyond built-in `Serial`

---

Fan Trigger Logic

```
Every loop:
  IF (MQ135 > 400) OR (MQ7 > 400) OR (MQ2 > 400)
    → digitalWrite(RELAY_PIN, LOW)   ← Fan ON
    → fanStatus = 1
  ELSE
    → digitalWrite(RELAY_PIN, HIGH)  ← Fan OFF
    → fanStatus = 0
```

The threshold of 400 is adjustable via `#define FAN_THRESHOLD 400` in the Arduino sketch. MQ sensor raw values range from 0–1023 (10-bit ADC). After 24–48 hours of burn-in, clean indoor air typically reads 100–250.
