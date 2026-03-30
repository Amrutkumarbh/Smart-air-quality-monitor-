🔬 How MQ Gas Sensors Work — Internal Guide

A step-by-step explanation of how the MQ135, MQ7, and MQ2 sensors detect gases, how they produce an electrical signal, and how your Arduino reads that signal.

---

Table of Contents

- [What is an MQ Sensor?](#what-is-an-mq-sensor)
- [Internal Structure (Cross-Section)](#internal-structure-cross-section)
- [The Sensing Material: SnO₂ (Tin Dioxide)](#the-sensing-material-sno₂-tin-dioxide)
- [The Heater Element](#the-heater-element)
- [How Gas Detection Works (Step by Step)](#how-gas-detection-works-step-by-step)
- [The Voltage Divider Circuit](#the-voltage-divider-circuit)
- [Reading the Signal on Arduino](#reading-the-signal-on-arduino)
- [MQ135 vs MQ7 vs MQ2 — What Each Detects](#mq135-vs-mq7-vs-mq2--what-each-detects)
- [Sensor Pin Diagram](#sensor-pin-diagram)
- [Why Sensors Need Burn-In Time](#why-sensors-need-burn-in-time)
- [Raw Value Meaning](#raw-value-meaning)

---

What is an MQ Sensor?

MQ sensors are a family of low-cost electrochemical gas sensors commonly used in Arduino and IoT projects. The "MQ" stands for **Metal oxide (M) with a heater coil (Q)**. They work by measuring how much a target gas changes the **electrical resistance** of a metal oxide material.

The three sensors used in this project:

| Sensor | Primary Target | Secondary Targets | Arduino Pin |
|--------|---------------|-------------------|-------------|
| MQ135 | CO₂ / Air Quality | Ammonia, Benzene, Alcohol, VOCs | A0 |
| MQ7 | Carbon Monoxide (CO) | Hydrogen (H₂), LPG | A1 |
| MQ2 | Smoke & LPG | Methane, Propane, Butane, Hydrogen | A2 |

All three work on **exactly the same internal principle** — only the sensing material chemistry differs.

---

Internal Structure (Cross-Section)

```
              ┌─────────────────────────────┐
              │   Stainless Steel Mesh      │  ← Allows air/gas in
              │ ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ │     blocks dust & debris
              ├─────────────────────────────┤
              │                             │
              │   ┌─────────────────────┐   │
              │   │   SnO₂ Ceramic      │   │  ← Sensing element
              │   │   Sensing Element   │   │     (tin dioxide)
              │   │  ~~~~~~~~~~~~~~~~~~~│   │
              │   │  Heater coil (NiCr) │   │  ← Keeps element at ~300°C
              │   │  ~~~~~~~~~~~~~~~~~~~│   │
              │   └─────────────────────┘   │
              │                             │
              │   Metal housing (can)       │
              └────────┬──────────┬─────────┘
                       │          │
               ┌───────┴──────────┴───────┐
               │  Bakelite pin base       │
               │  A  A  B  B  H  H        │
               └──┬──┬──┬──┬──┬──┬────────┘
                  │  │  │  │  │  │
                 Sig   Sig  Heater supply
                pins   pins    (5V / GND)
```

Labeled Components

| Component | Material | Purpose |
|-----------|----------|---------|
| Outer mesh | Stainless steel | Protects sensing element, allows gas to pass through |
| Metal can (housing) | Aluminium / steel | Physical protection, heat containment |
| Sensing element | SnO₂ (Tin Dioxide) | Changes resistance when it contacts target gases |
| Heater coil | Nickel-Chromium (NiCr) wire | Heats sensing element to ~300°C for chemical reactions |
| Pin base | Bakelite | Mounts the 6 output pins |
| A, B pins | Copper | Signal pins — measure resistance of sensing element |
| H, H pins | Copper | Heater pins — supply 5V to heat the coil |

---

The Sensing Material: SnO₂ (Tin Dioxide)

The sensing element is a small cylinder or bead of **tin dioxide (SnO₂)** — a semiconductor metal oxide. In clean air, oxygen molecules from the air adsorb (stick) onto the SnO₂ surface and **capture electrons** from the material:

```
CLEAN AIR:

    O₂ + e⁻ → O⁻  (oxygen captures electrons from SnO₂)

    SnO₂ surface:  [O⁻][O⁻][O⁻][O⁻][O⁻][O⁻][O⁻]
                    ──────────────────────────────
                    Very HIGH resistance → low voltage output
```

When target gas molecules enter:

```
WITH TARGET GAS (e.g. CO):

    CO + O⁻ → CO₂ + e⁻  (gas reacts with O⁻, releases electron back)

    SnO₂ surface:  [  ][  ][  ][  ][  ][  ][  ]
                    ──────────────────────────────
                    Electrons freed → LOWER resistance → higher voltage output
```

**The key insight:** More gas = more electrons freed = lower resistance = higher voltage = higher Arduino analogRead() value.

---

The Heater Element

Inside the sensing element is a tightly wound coil of **Nickel-Chromium (NiCr) wire** — the same material used in toaster elements. It is powered separately through the **H pins** (5V supply, ~150–900 mA depending on model).

```
Purpose of the heater:

    Room temperature (~25°C) → SnO₂ too cold → reactions too slow to measure
    
    Heater raises surface to ~300°C → chemical reactions happen fast and reliably
    
    This is why MQ sensors consume more current than most sensors.
    It is also why they need 24–48 hours of burn-in on first use.
```

> ⚠️ The MQ7 sensor uses a special two-phase heating cycle (5V for 60 seconds, then 1.4V for 90 seconds) for accurate CO readings. In this project we use 5V continuous which gives directional readings suitable for threshold alerting.

---

How Gas Detection Works (Step by Step)

```
Step 1 — PREHEAT
─────────────────
5V applied to H pins
     │
     ▼
Heater coil reaches ~300°C
     │
     ▼
SnO₂ surface activates (O₂ from air adsorbs as O⁻ ions)
     │
     ▼
Resistance = HIGH (several MΩ) → analogRead ≈ 100–200


Step 2 — GAS ENTERS THROUGH MESH
──────────────────────────────────
Target gas molecules (CO, smoke, etc.) pass through the steel mesh
     │
     ▼
Gas molecules contact the hot SnO₂ surface
     │
     ▼
Gas oxidises the O⁻ ions → electrons released back into SnO₂


Step 3 — RESISTANCE DROPS
──────────────────────────
More free electrons → material becomes more conductive
     │
     ▼
Resistance drops (from MΩ down to kΩ)
     │
     ▼
Higher current flows through the voltage divider


Step 4 — VOLTAGE CHANGES
──────────────────────────
Voltage divider converts resistance change into voltage change
     │
     ▼
Arduino analogRead() returns a value between 0 and 1023
     │
     ▼
Higher gas concentration → higher analogRead() number
```

---

The Voltage Divider Circuit

MQ sensors cannot connect directly to an Arduino analog pin. The sensing element's resistance change must be converted to a **voltage** the Arduino can read. This is done with a built-in **voltage divider** on the sensor module's PCB:

```
         5V
          │
          │
     ┌────┴────┐
     │   Rs    │   ← Sensor resistance (changes with gas)
     │ (sensor)│
     └────┬────┘
          │
          ├──────────→  A0 (Arduino analog pin reads this voltage)
          │
     ┌────┴────┐
     │   RL    │   ← Load resistor (fixed, usually 1kΩ or 10kΩ)
     │ (fixed) │
     └────┬────┘
          │
         GND
```

The equation

```
Vout = 5V × RL ÷ (Rs + RL)
```

| Condition | Rs | Vout | analogRead() |
|-----------|-------|------|--------------|
| Clean air | Very HIGH (MΩ) | Very LOW (≈ 0.1V) | ≈ 20–100 |
| Moderate gas | Medium (kΩ) | Medium (≈ 1–2V) | ≈ 200–400 |
| High gas concentration | LOW (few hundred Ω) | HIGH (≈ 3–4V) | ≈ 600–900 |

> The sensor module PCB includes a built-in load resistor (the blue potentiometer on many boards). Turning this potentiometer adjusts RL and therefore calibrates the sensitivity.

---

Reading the Signal on Arduino

The Arduino's analog pins have a **10-bit ADC (Analog to Digital Converter)**. It maps 0V–5V to a number from 0 to 1023.

```
analogRead() scale:

  0V ──────────────────────────── 5V
  │                                │
  0                              1023

  Clean air:    ≈ 100–200
  Moderate:     ≈ 200–400
  Threshold:        400     ← Fan turns ON in this project
  Poor air:     ≈ 400–700
  Very poor:    ≈ 700–1023
```

In the Arduino code:

```cpp
int mq135Value = analogRead(A0);   // Returns 0–1023

if (mq135Value > 400) {
    digitalWrite(RELAY_PIN, LOW);  // Fan ON
}
```

The value `400` is the threshold. It is **not in ppm (parts per million)** — it is a raw ADC reading. For accurate ppm conversion, sensor-specific calibration curves are needed (beyond the scope of this project).

---

MQ135 vs MQ7 vs MQ2 — What Each Detects

```
MQ135 — General Air Quality
────────────────────────────
Sensing material: SnO₂ doped for broad gas range
Primary target:   CO₂, VOCs (Volatile Organic Compounds)
Also detects:     Ammonia (NH₃), Benzene, Alcohol, Smoke
Arduino pin:      A0
Typical clean air reading: 100–200
Trigger reading:  >400

                  sensitivity
                      ▲
              high    │ ██
                      │ ██  ██
              medium  │ ██  ██  ██
                      │ ██  ██  ██  ██
               low    └──────────────────→ gas type
                       CO₂  NH₃  Benz Alc


MQ7 — Carbon Monoxide
───────────────────────
Sensing material: SnO₂ optimised for CO selectivity
Primary target:   CO (Carbon Monoxide)
Also detects:     Hydrogen (H₂), some LPG
Arduino pin:      A1
Typical clean air reading: 50–100
Dangerous level:  >200 ppm CO (headache risk for humans)

⚠️  CO is colourless and odourless — this sensor provides
    critical safety alerting that humans cannot do themselves.

                  sensitivity
                      ▲
              high    │ ████
                      │ ████
              medium  │ ████      ██
                      │ ████      ██    ██
               low    └──────────────────→ gas type
                        CO        H₂    LPG


MQ2 — Smoke & Combustible Gas
───────────────────────────────
Sensing material: SnO₂ broad sensitivity to combustibles
Primary target:   LPG, Smoke, Propane
Also detects:     Methane (CH₄), Butane, Hydrogen, Alcohol
Arduino pin:      A2
Typical clean air reading: 100–150
Fire / gas leak trigger:   >400

                  sensitivity
                      ▲
              high    │ ██  ██  ██
                      │ ██  ██  ██  ██
              medium  │ ██  ██  ██  ██  ██
                      │ ██  ██  ██  ██  ██  ██
               low    └──────────────────────→ gas type
                       LPG  Prp  Mth  Smk  H₂  Alc
```

---

Sensor Pin Diagram

All three MQ sensors share the same 4-pin module layout:

```
         ┌───────────────────┐
         │   MQ sensor PCB   │
         │                   │
         │   ┌───────────┐   │
         │   │ Sensor can│   │
         │   └───────────┘   │
         │   [POT] [LED]     │   ← Sensitivity pot & power LED
         └─────────────┬─────┘
                       │
              ┌────────┴────────┐
              │                 │
             VCC               GND      ← Power (5V / GND)
              │                 │
             AOUT             DOUT      ← Analog out / Digital out
              │                 │
              │                 └──→ Digital pin (HIGH/LOW threshold)
              └──────────────────────→ Analog pin (0–1023 value) ← we use this
```

In this project we only use AOUT (analog output)** because it gives a smooth 0–1023 range rather than just ON/OFF.

| Pin | Used? | Connected to |
|-----|-------|-------------|
| VCC | ✅ Yes | Arduino 5V |
| GND | ✅ Yes | Arduino GND |
| AOUT | ✅ Yes | Arduino A0 / A1 / A2 |
| DOUT | ❌ No | (Not connected) |

---
Why Sensors Need Burn-In Time

When MQ sensors are first powered on after manufacture or a long period of storage:

```
Problem:
    SnO₂ surface is "cold" and contaminated from storage
         │
         ▼
    O⁻ ions have not properly adsorbed yet
         │
         ▼
    Readings are artificially HIGH (false alerts)


Solution — burn-in period:
    Power the sensor continuously for 24–48 hours
         │
         ▼
    Heater stabilises the SnO₂ surface chemistry
         │
         ▼
    O⁻ ions reach steady state in clean air
         │
         ▼
    Baseline reading stabilises (clean air ≈ 100–200)
         │
         ▼
    Sensor is now reliable for threshold detection
```

> This is why the fan may run constantly for the first day or two after building the project — the sensors are still stabilising. After 48 hours of continuous power, readings in clean air should drop well below 400.

---

Raw Value Meaning

The table below gives a rough guide to what analogRead() values mean in practice. These are indicative only — exact values depend on calibration, humidity, temperature, and sensor age.

| analogRead() | Air Quality | Fan Status |
|-------------|-------------|-----------|
| 0 – 100 | Very clean / sensor warming up | OFF |
| 100 – 200 | Clean indoor air (normal) | OFF |
| 200 – 350 | Slightly elevated (cooking, humidity) | OFF |
| 350 – 400 | Borderline — minor pollutants present | OFF |
| 400 – 600 | Poor air quality / detectable gas | **ON** |
| 600 – 800 | High concentration / possible leak | **ON** |
| 800 – 1023 | Very high / sensor near saturation | **ON** |

---

*Document prepared as part of the Smart Air Quality Monitor & Purifier project.*
*Refer to [`docs/wiring.md`](wiring.md) for hardware connections and [`docs/troubleshooting.md`](troubleshooting.md) for sensor issues.*
