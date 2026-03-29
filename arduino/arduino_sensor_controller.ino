#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "AIR QUALITY MONITORING"
#define BLYNK_AUTH_TOKEN    "YOUR_AUTH_TOKEN"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

#define RXp2 16
#define TXp2 17

int   mq135, mq7, mq2, fanStatus;
float temperature, humidity;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  if (Serial2.available()) {
    String incoming = Serial2.readStringUntil('\n');
    incoming.trim();
    if (incoming.length() > 5) {
      int parsed = sscanf(incoming.c_str(),
                          "%d,%d,%d,%f,%f,%d",
                          &mq135, &mq7, &mq2,
                          &temperature, &humidity, &fanStatus);
      if (parsed == 6) {
        Blynk.virtualWrite(V0, mq135);
        Blynk.virtualWrite(V1, mq7);
        Blynk.virtualWrite(V2, mq2);
        Blynk.virtualWrite(V3, temperature);
        Blynk.virtualWrite(V4, humidity);
        Blynk.virtualWrite(V5, fanStatus);
      }
    }
  }
}
