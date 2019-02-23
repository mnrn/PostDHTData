#include <ArduinoJson.h>
#include <DHT.h>
#include <Ethernet.h>
#include <SPI.h>

constexpr uint8_t dhtPin = 2;       // Pin connected to the DHT sensor.
constexpr uint8_t dhtType = DHT22;  // Use DHT 22 (AM2302)
DHT dht{dhtPin, dhtType};

void setup() {
  // Initialize Serial port.
  Serial.begin(9600);

  // Initialize DHT.
  dht.begin();

  // Initialize Ethernet library
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  if (!Ethernet.begin(mac)) {
    Serial.println(F("Failed to configure Ethernet"));
  }
}

void loop() {
  // Wait 5 seconds between measurements.
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  delay(5000);

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

  // Http request.
  EthernetClient client;
  client.setTimeout(2000);
  if (client.connect("192.168.1.6", 2888)) {
    Serial.println("Connected to server.");
    client.println("POST /dht HTTP/1.1");
    client.println("Host: 192.168.1.6");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(root.measureLength());
    client.println();
    root.printTo(client);

    client.flush();
    client.stop();
  }
  else {
    Serial.println("Failed to connected to server.");
  }
}

// Sets the error value at errors key.
void setError(JsonObject& obj, const String& message) {
    auto& errors = obj.containsKey("errors")
      ? obj["errors"]
      : obj.createNestedArray("errors");
    auto& error = errors.createNestedObject();
    error["message"] = message;
}
