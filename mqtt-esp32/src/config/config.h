#pragma once

// ===== Device =====
#define DEVICE_ID "esp32_devkit_06"

// real/fake device mode
#define USE_FAKE_DEVICE 0   // 1 = fake, 0 = sensor thật

// ===== WiFi =====
#define WIFI_SSID "Nguyen Thanh Huyen"
#define WIFI_PASS "Huyen1978"

// ===== MQTT =====
#define MQTT_SERVER "connection.ducbinh203.tech"
#define MQTT_PORT 8883
#define MQTT_USER "iot-mqtt"
#define MQTT_PASS "abcd1234"

// ===== Time =====
#define GMT_OFFSET_SEC (7 * 3600)

// ===== Pins =====
#define PIN_BUTTON 0
#define PIN_LIGHT 32
#define PIN_RAIN 35
#define PIN_DHT 19
#define PIN_DUST_LED 33
#define PIN_DUST_OUT 34
#define PIN_MP3_TX 26 // ESP32 TX → DFPlayer RX
#define PIN_MP3_RX 27 // ESP32 RX ← DFPlayer TX
