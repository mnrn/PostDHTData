#include <ArduinoJson.h>
#include <DHT.h>

constexpr uint8_t dhtPin = 2;
constexpr uint8_t dhtType = DHT22;
DHT dht{dhtPin, dhtType};

// Sets the error value at errors key.
void setError(JsonObject& obj, const String& message) {
    auto& errors = obj.containsKey("errors")
      ? obj["errors"]
      : obj.createNestedArray("errors");
    auto& error = errors.createNestedObject();
    error["message"] = message;
}

void setup() {
  Serial.begin(9600);

  dht.begin();
}

void loop() {
  // Wait 10 seconds between measurements.
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  delay(10000);

  // Memory pool for JSON object tree.
  // Inside the brackets, 200 is the size of the pool in bytes.
  StaticJsonBuffer<200> jsonBuffer;

  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  auto& root = jsonBuffer.createObject();

  // Read temperature and set its value.
  const auto temperature = dht.readTemperature();
  if (isnan(temperature)) {
    setError(root, "Error reading temperature!");
  }
  else {
    root["temperature"] = temperature;
  }
  
  // Read humidity and set its value.
  const auto humidity = dht.readHumidity();
  if (isnan(humidity)) {
    setError(root, "Error reading humidity!");
  }
  else {
    root["humidity"] = humidity;
  }

  // Serialize Json.
  root.printTo(Serial);
  Serial.print("\n");  // Delimiter to use.
}
