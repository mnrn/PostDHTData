#include <ArduinoJson.h>
#include "DHTSensor.h"

constexpr uint8_t dhtPin = 2;
constexpr uint8_t dhtType = DHT22;
DHTSensor dht{dhtPin, dhtType};

void setError(JsonObject& obj, const String& message) {
    auto& errors = obj.containsKey("errors")
      ? obj["errors"]
      : obj.createNestedArray("errors");
    auto& error = errors.createNestedObject();
    error["message"] = message;
}

void setup() {
  // Initilialize Serial port.
  Serial.begin(9600);

  // Initialize DHT.
  dht.begin();
}

void loop() {
  // Delay between measurements.
  constexpr int32_t expectDelay = 10000; // 10seconds
  delay(static_cast<unsigned long>(max(expectDelay, dht.getMinDelay())));

  // Memory pool for JSON object tree.
  // Inside the brackets, 200 is the size of the pool in bytes.
  StaticJsonBuffer<200> jsonBuffer;

  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  auto& root = jsonBuffer.createObject();

  // Get temperature event and set its value.
  const auto temperatureEvent = dht.getTemperatureEvent();
  if (isnan(temperatureEvent.temperature)) {
    setError(root, "Error reading temperature!");
  }
  else {
    root["temperature"] = temperatureEvent.temperature;
  }
  
  // Get humidity event and set its value.
  const auto humidityEvent = dht.getHumidityEvent();
  if (isnan(humidityEvent.relative_humidity)) {
    setError(root, "Error reading humidity!");
  }
  else {
    root["humidity"] = humidityEvent.relative_humidity;
  }

  // Send Json.
  root.prettyPrintTo(Serial);
}
