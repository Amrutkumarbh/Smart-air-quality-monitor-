☁️ Blynk IoT Dashboard Setup

Blynk is a free IoT platform that lets you build a live dashboard in minutes. The ESP32 sends **6 sensor values** to 6 Virtual Pins (called Datastreams) in the Blynk app.

---

step-by-Step Setup

1. Create a Free Account
Go to [blynk.cloud](https://blynk.cloud) and register for a free account.

2. Create a New Template
- Click **"New Template"**
- Give it a name (e.g. `AIR QUALITY MONITORING`)
- Select **ESP32** as the hardware
- Select **Wi-Fi** as the connection type

3. Add the 6 Datastreams

Navigate to the **Datastreams** tab in your template and create one entry for each row below:

| Virtual Pin | Name | Data Type | Min | Max | Units |
|-------------|------|-----------|-----|-----|-------|
| V0 | Air Quality (MQ135) | Integer | 0 | 500 | — |
| V1 | Carbon Monoxide (MQ7) | Integer | 0 | 500 | — |
| V2 | Smoke / Gas (MQ2) | Integer | 0 | 500 | — |
| V3 | Temperature | Double | 0 | 60 | °C |
| V4 | Humidity | Double | 0 | 100 | % |
| V5 | Fan Status | Integer | 0 | 1 | — |

4. Copy Your Auth Token
- Go to your template's **Info** tab
- Copy the `BLYNK_TEMPLATE_ID` and `BLYNK_AUTH_TOKEN`
- Paste both into `esp32/esp32_blynk_uploader.ino`

```cpp
#define BLYNK_TEMPLATE_ID   "TMPL_XXXXXXXXX"
#define BLYNK_AUTH_TOKEN    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
```

5. Build Your Dashboard
- Open the **Web Dashboard** tab in your template
- Drag and drop widgets and link them to virtual pins:

| Widget Type | Link to | Suggested Label |
|-------------|---------|-----------------|
| Gauge | V0 | Air Quality |
| Gauge | V1 | CO Level |
| Gauge | V2 | Smoke Level |
| Labeled Value | V3 | Temperature (°C) |
| Labeled Value | V4 | Humidity (%) |
| LED / Label | V5 | Fan Status |

6. Activate Your Device
- Click **Save** on the template
- Go to **Devices → New Device** and link it to your template
- Power on your ESP32 — it should appear as **Online** in the device list

---

Important Blynk Free Plan Limits

> ⚠️ The Blynk free plan limits API calls to roughly **1 per second per virtual pin**. With 6 pins, you should send data no faster than once every **6 seconds** to be safe.
>
> The current Arduino `delay(2000)` is set to 2 seconds. If Blynk keeps disconnecting, increase it to `delay(3000)` or higher.

---

Blynk Virtual Pin Summary

| Virtual Pin | Source | What It Shows |
|-------------|--------|---------------|
| V0 | MQ135 | General air quality (CO₂ / VOCs) — 0 is clean, 500 is very poor |
| V1 | MQ7 | Carbon monoxide concentration |
| V2 | MQ2 | Smoke and flammable gas level |
| V3 | DHT | Temperature in °C |
| V4 | DHT | Relative humidity in % |
| V5 | Relay | Fan state — `0` = OFF, `1` = ON |
