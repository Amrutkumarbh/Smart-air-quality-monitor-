#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN    2
#define DHTTYPE   DHT11
#define MQ135_PIN A0
#define MQ7_PIN   A1
#define MQ2_PIN   A2
#define RELAY_PIN 8
#define FAN_THRESHOLD 400

SoftwareSerial espSerial(10, 11);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  delay(2000);
}

void loop() {
  int   mq135Value  = analogRead(MQ135_PIN);
  int   mq7Value    = analogRead(MQ7_PIN);
  int   mq2Value    = analogRead(MQ2_PIN);
  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    humidity = 0.0; temperature = 0.0;
  }

  int fanStatus = 0;
  if (mq135Value > FAN_THRESHOLD ||
      mq7Value   > FAN_THRESHOLD ||
      mq2Value   > FAN_THRESHOLD) {
    digitalWrite(RELAY_PIN, LOW);
    fanStatus = 1;
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    fanStatus = 0;
  }

  String dataString = String(mq135Value)    + "," +
                      String(mq7Value)       + "," +
                      String(mq2Value)       + "," +
                      String(temperature, 1) + "," +
                      String(humidity, 1)    + "," +
                      String(fanStatus);

  Serial.println("Sending: " + dataString);
  espSerial.println(dataString);
  delay(2000);
}
