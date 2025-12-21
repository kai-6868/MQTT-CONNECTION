#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <DHTesp.h>

class SensorManager {
public:
    SensorManager(int dhtPin, int lightPin, int rainPin);
    void begin();
    float readTemperature();
    float readHumidity();
    int readLight();
    int readRain();
private:
    int _dhtPin;
    int _lightPin;
    int _rainPin;
    DHTesp _dht;
};

#endif // SENSOR_MANAGER_H
