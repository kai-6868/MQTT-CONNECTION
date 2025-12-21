#include "SensorManager.h"
#include <Arduino.h>

SensorManager::SensorManager(int dhtPin, int lightPin, int rainPin)
    : _dhtPin(dhtPin), _lightPin(lightPin), _rainPin(rainPin) {}

void SensorManager::begin() {
    _dht.setup(_dhtPin, DHTesp::DHT11);
}

float SensorManager::readTemperature() {
    return _dht.getTempAndHumidity().temperature;
}

float SensorManager::readHumidity() {
    return _dht.getTempAndHumidity().humidity;
}

int SensorManager::readLight() {
    return analogRead(_lightPin);
}

int SensorManager::readRain() {
    return analogRead(_rainPin);
}
