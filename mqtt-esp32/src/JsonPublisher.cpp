#include "JsonPublisher.h"
#include <WiFi.h>
#include <time.h>

void JsonPublisher::buildPayload(DynamicJsonDocument& doc,
                                const char* device_id,
                                float temperature,
                                float humidity,
                                float dustDensity,
                                int lightValue,
                                int rainValue) {
    doc["device_id"] = device_id;
    doc["timestamp"] = time(nullptr);
    doc["wifi_name"] = WiFi.SSID();
    doc["wifi_rssi"] = WiFi.RSSI();
    doc["temp"] = temperature;
    doc["hum"] = humidity;
    doc["dust"] = dustDensity;
    doc["light"] = lightValue;
    doc["rain"] = rainValue;
}
