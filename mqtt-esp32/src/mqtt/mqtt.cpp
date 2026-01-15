#include "mqtt.h"
#include "../config/config.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include "../audio/audio.h"
#include "../lcd/lcd.h"
#include "../servo/servo.h"

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

static String topicControl = "ecosense/devices/" DEVICE_ID "/control";

void mqtt_callback(char* topic, byte* payload, unsigned int length)
{ 
  Serial.print("[MQTT] Message arrived on topic: ");
  Serial.println(topic);
  Serial.print("[MQTT] Payload: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  JsonDocument doc;   // ✅ đúng cho ArduinoJson v7

  DeserializationError err = deserializeJson(doc, payload, length);
  if (err) {
    Serial.println("JSON parse failed");
    return;
  }

    // ===== LED =====
    if (doc.containsKey("led")) {
    JsonObject led = doc["led"];

    int state = led["state"] | 0;   // mặc định = 0 nếu không tồn tại
    digitalWrite(LED_BUILTIN, state ? HIGH : LOW);
    }

  // ===== AUDIO =====
  if (doc.containsKey("audio")) {
    JsonObject a = doc["audio"];

    if (a.containsKey("volume"))
      audio_set_volume(a["volume"].as<int>());

    if (a.containsKey("play"))
      audio_play(a["play"].as<int>());
  }

  // ===== LCD =====
  if (doc.containsKey("lcd")) {
    {
    const char* line1 = doc["lcd"]["line1"] | "";
    const char* line2 = doc["lcd"]["line2"] | "";

    lcd_show_alert(line1, line2);
    }
  }

  // ===== SERVO =====
  if (doc.containsKey("servo")) {
    JsonObject s = doc["servo"];

    if (s.containsKey("angle"))
      servo_run(s["angle"].as<int>());
  }
}


static void mqtt_reconnect()
{
  while (!mqttClient.connected())
  {
    Serial.print("[MQTT] Connecting... ");

    if (mqttClient.connect(DEVICE_ID, MQTT_USER, MQTT_PASS))
    {
        Serial.println("connected");
        Serial.print("[MQTT] Subscribing to topic: ");
        Serial.println(topicControl);

      mqttClient.subscribe(topicControl.c_str());
    }
    else
    {
        Serial.print("failed, rc=");
        Serial.println(mqttClient.state());
        Serial.println("Retrying in 2 seconds...");
        delay(2000);
    }
  }
}

void mqtt_init()
{
  espClient.setInsecure();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqtt_callback);
  mqttClient.setBufferSize(1024);

  mqtt_reconnect();
}

void mqtt_loop()
{
  if (!mqttClient.connected()) mqtt_reconnect();
  mqttClient.loop();
}

void mqtt_publish(const char* topic, const char* payload, size_t len)
{
    Serial.print("[MQTT] Publishing to topic: ");
    Serial.println(topic);
    Serial.print("[MQTT] Payload: ");
    Serial.println(payload);

  mqttClient.publish(topic, payload, len);
}
