#include "payload.h"
#include "../config/config.h"
#include <WiFi.h>
#include <time.h>

size_t build_payload(char* buffer, size_t size, const SensorData& data)
{
  DynamicJsonDocument doc(512);

  doc["device_id"] = DEVICE_ID;
  doc["timestamp"] = time(nullptr);
  doc["wifi_name"] = WiFi.SSID();
  doc["wifi_rssi"] = WiFi.RSSI();

  doc["temp"] = data.temperature;
  doc["hum"] = data.humidity;
  doc["dust"] = data.dust;
  doc["light"] = data.light;
  doc["rain"] = data.rain;

  return serializeJson(doc, buffer, size);
}
