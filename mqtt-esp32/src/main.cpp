#include <Arduino.h>
#include "config/config.h"
#include "wifi/wifi.h"
#include "sensor/sensor.h"
#include "mqtt/mqtt.h"
#include "payload/payload.h"
#include "fake/fake_device.h"
#include "audio/audio.h"
#include "lcd/lcd.h"

static String topicData = "ecosense/devices/" DEVICE_ID "/data";

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  // fake or real data device
  #if USE_FAKE_DEVICE
    Serial.println("[FAKE DEVICE] Data generated");
  #else
    Serial.println("[SENSOR] Real data read");
  #endif


  wifi_init();
  configTime(GMT_OFFSET_SEC, 0, "pool.ntp.org");
  audio_init();
  lcd_init();
  mqtt_init();

#if USE_FAKE_DEVICE
  fake_device_init();
#else
  sensor_init();
#endif
}

void loop()
{
  mqtt_loop();
  lcd_loop();

  #if USE_FAKE_DEVICE
    SensorData data = fake_device_generate();
  #else
    SensorData data = sensor_read();
  #endif
  lcd_show_sensor(data);

  char payload[512];
  size_t len = build_payload(payload, sizeof(payload), data);

  mqtt_publish(topicData.c_str(), payload, len);

  delay(10000);
}



