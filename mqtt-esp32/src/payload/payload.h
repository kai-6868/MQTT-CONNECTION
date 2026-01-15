#pragma once
#include <ArduinoJson.h>
#include "../sensor/sensor.h"

size_t build_payload(char* buffer, size_t size, const SensorData& data);
