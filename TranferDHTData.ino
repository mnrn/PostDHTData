#include "DHTSensor.h"

constexpr uint8_t dhtPin = 2;
constexpr uint8_t dhtType = DHT22;
DHTSensor dht{dhtPin, dhtType};

void setup() {
  constexpr int serialBps = 9600;
  Serial.begin(serialBps);

  dht.begin();
  // Print temperature sensor details.
  const auto tempSensor = dht.getTemperatureSensor();
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(tempSensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(tempSensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(tempSensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(tempSensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(tempSensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(tempSensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  const auto humidSensor = dht.getHumiditySensor();
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(humidSensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(humidSensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(humidSensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(humidSensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(humidSensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(humidSensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));

  dht.setMinDelay(humidSensor.min_delay / 1000);
}

void loop() {
  // put your main code here, to run repeatedly:

}
