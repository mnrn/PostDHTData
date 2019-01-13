#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class DHTSensor {
  public:
    DHTSensor() = delete;
    template<typename... Args>
    explicit DHTSensor(Args&&... args)
      : _dht{args...} {}

    void begin() {
      _dht.begin();
    }
    
    sensor_t getTemperatureSensor() {
      sensor_t sensor;
      _dht.temperature().getSensor(&sensor);
      return sensor;
    }
    sensor_t getHumiditySensor() {
      sensor_t sensor;
      _dht.humidity().getSensor(&sensor);
      return sensor;
    }

    sensors_event_t getTemperatureEvent() {
      sensors_event_t event;
      _dht.temperature().getEvent(&event);
      return event;
    }
    sensors_event_t getHumidityEvent() {
      sensors_event_t event;
      _dht.humidity().getEvent(&event);
      return event;
    }
    
    int32_t getMinDelay() const {
      return _minDelay;
    }
    void setMinDelay(int32_t minDelay) {
      _minDelay = minDelay;
    }
  private:
    DHT_Unified _dht;
    int32_t _minDelay { };
};

#endif  // end of #ifndef DHT_SENSOR_H
