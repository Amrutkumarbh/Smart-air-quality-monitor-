🔧 Troubleshooting Guide

---

ESP32 shows gibberish or receives no data from Arduino

Most likely cause:Missing common GND wire between the two boards.

Fix:
- Add a wire from Arduino GND to ESP32 GND
- This is mandatory. Without it, the two boards have no shared voltage reference and serial communication will fail or produce garbage data.
- Double-check TX/RX are not swapped: Arduino Pin 11 (TX) → ESP32 Pin 16 (RX2)

---

DHT sensor returns `0.0` or `NaN`

Most likely cause: Wrong pin connection or a faulty sensor.
Fix:
1. Confirm the DATA wire is connected to **Digital Pin 2** on the Arduino
2. If the problem persists, add a **10kΩ pull-up resistor** between the DATA pin and VCC
3. Try swapping in a different DHT sensor to rule out hardware failure
4. Check you have `#define DHTTYPE DHT11` (or `DHT22`) matching your actual sensor model

---

Fan never turns ON

Most likely cause: Relay logic is inverted, or the air is genuinely clean.

Fix:
1. Hold a lighter flame (no spark) or blow smoke near the **MQ2** sensor to artificially raise the reading above 400
2. Watch the Arduino Serial Monitor — confirm the sensor values are rising
3. If values exceed 400 but the fan still won't turn on, your relay module may be **active-HIGH** instead of active-LOW
4. In `arduino_sensor_controller.ino`, swap the relay logic:
   ```cpp
   // If relay is active-HIGH, change these lines:
   digitalWrite(RELAY_PIN, HIGH);  // Fan ON  (was LOW)
   digitalWrite(RELAY_PIN, LOW);   // Fan OFF (was HIGH)
   ```

---

Fan is always ON / reads high values even in clean air

Most likely cause: MQ sensors need burn-in time, or threshold is set too low.

Fix:
1. MQ sensors require **24–48 hours of continuous power** to burn in and stabilise. Initial readings will be artificially high.
2. Let the sensors run powered for 48 hours before trusting the values.
3. If readings remain high after burn-in, raise the threshold in the Arduino code:
   ```cpp
   #define FAN_THRESHOLD 400   // Try increasing to 500 or 600
   ```

---
Blynk keeps disconnecting

Most likely cause: Arduino is sending data too fast for the Blynk free plan.

Fix:
- Increase the delay in the Arduino loop:
  ```cpp
  delay(2000);   // Change to 3000 or higher
  ```
- The Blynk free plan allows roughly **1 API call per second per virtual pin**. With 6 pins, sending every 2 seconds can sometimes overwhelm the server.

---

ESP32 won't connect to Wi-Fi

Most likely cause: Using a 5 GHz network, or wrong credentials.

Fix:
1. The ESP32 supports 2.4 GHz Wi-Fi only — it cannot connect to 5 GHz networks
2. On your router, ensure the 2.4 GHz band is enabled (some routers use separate SSIDs for each band)
3. Double-check the `ssid` and `pass` values in `esp32_blynk_uploader.ino`
4. Make sure there are no extra spaces in the credentials

---

Arduino Serial Monitor shows sensor values but ESP32 shows nothing

Most likely cause: SoftwareSerial pins are swapped, or baud rates don't match.

Fix:
1. Confirm `espSerial(10, 11)` in Arduino code — Pin 10 = RX, Pin 11 = TX
2. Confirm `Serial2.begin(9600, SERIAL_8N1, 16, 17)` in ESP32 code — Pin 16 = RX, Pin 17 = TX
3. Both sides must use **9600 baud** — confirm no mismatch
4. Confirm the common GND wire is in place

---

Blynk device shows as offline even though ESP32 is powered

Most likely cause: Auth token mismatch or template not activated.

Fix:
1. Double-check `BLYNK_AUTH_TOKEN` in the ESP32 code matches the token on your Blynk template page exactly
2. Make sure you also set `BLYNK_TEMPLATE_ID` and `BLYNK_TEMPLATE_NAME` correctly
3. On Blynk, go to **Devices** and check the device is properly linked to your template
