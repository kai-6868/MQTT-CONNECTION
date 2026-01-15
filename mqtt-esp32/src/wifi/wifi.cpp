#include "wifi.h"
#include "../config/config.h"
#include <Arduino.h>

void wifi_init()
{
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nWiFi connected!");
}
