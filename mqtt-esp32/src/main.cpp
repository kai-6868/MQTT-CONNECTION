#include <Arduino.h>
<<<<<<< HEAD
#include "config/config.h"
#include "wifi/wifi.h"
#include "sensor/sensor.h"
#include "mqtt/mqtt.h"
#include "payload/payload.h"
#include "fake/fake_device.h"
#include "audio/audio.h"
#include "lcd/lcd.h"
#include "servo/servo.h"

static String topicData = "ecosense/devices/" DEVICE_ID "/data";
=======
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "time.h"
#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>
#include "SharpGP2Y10.h"
#include <DFRobotDFPlayerMini.h>

// ===== Cảm biến =====
const int buttonPin = 0;
const int lightSensorPin = 32;
const int rainSensorPin = 35;
const int dhtPin = 19;

DHTesp dht;

SharpGP2Y10 dustSensor(34, 33, 3.3);

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== Thông tin thiết bị =====
const char *device_id = "esp32_devkit_02";

// ===== WiFi =====
const char *ssid = "Nguyen Thanh Huyen";
const char *password = "Huyen1978";

// ===== MQTT Server =====
const char *mqtt_server = "connection.ducbinh203.tech";
const int mqtt_port = 8883;   // TLS
const char *mqtt_user = "iot-mqtt";
const char *mqtt_pass = "abcd1234";

String topic_data = "ecosense/devices/" + String(device_id) + "/data";
String topic_control = "ecosense/devices/" + String(device_id) + "/control";

// ===== Time =====
const long gmtOffset_sec = 7 * 3600;

// ===== MQTT =====
WiFiClientSecure espClient;
PubSubClient client(espClient);

// ===== Hàm khai báo trước =====
void playTrack(uint8_t track);
unsigned long waitTrackFinished();

// ======================
//      CALLBACK MQTT
// ======================
void callback(char *topic, byte *payload, unsigned int length)
{
  String msg;
  msg.reserve(length);
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];

  Serial.print("Received JSON: ");
  Serial.println(msg);

  DynamicJsonDocument doc(256);
  if (deserializeJson(doc, msg)) return;

  if (doc.containsKey("led"))
  {
    const char *state = doc["led"]["state"];
    digitalWrite(LED_BUILTIN, strcmp(state, "ON") == 0);
    Serial.println(String("LED -> ") + state);
  }
}

// ======================
//         WIFI
// ======================
void setup_wifi()
{
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi connected!");
}

// ======================
//        MQTT
// ======================
void reconnect_mqtt()
{
  while (!client.connected())
  {
    Serial.print("Connecting to MQTT...");
    if (client.connect(device_id, mqtt_user, mqtt_pass))
    {
      Serial.println("connected");
      client.subscribe(topic_control.c_str());
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}
>>>>>>> main

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
  servo_init();
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

<<<<<<< HEAD
  mqtt_publish(topicData.c_str(), payload, len);

  delay(60000); // 1 phút
=======
  // ===== Hiển thị LCD =====
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print(" H:");
  lcd.print(humidity, 1);

  lcd.setCursor(0, 1);
  lcd.print("D:");
  lcd.print(dustDensity, 1);
  lcd.print(" L:");
  lcd.print(lightValue);
  Serial.print("Dust Density: ");
  Serial.print(dustDensity);
  Serial.println(" ug/m3");
  delay(1000);
  // ===== JSON =====
  DynamicJsonDocument doc(512);

  doc["device_id"] = device_id;
  doc["timestamp"] = time(nullptr);

  doc["wifi_name"] = WiFi.SSID();
  doc["wifi_rssi"] = WiFi.RSSI();

  doc["temp"] = temperature;
  doc["hum"] = humidity;

  doc["dust"] = dustDensity;
  doc["light"] = lightValue;
  doc["rain"] = rainValue;

  // ===== Serialize & Publish =====
  char buffer[512];
  size_t len = serializeJson(doc, buffer);

  bool success = client.publish(topic_data.c_str(), buffer, len);
  if(success)
  {
    Serial.println("===== MQTT PUBLISH SUCCESS =====");
    Serial.print("Topic: "); Serial.println(topic_data);
    Serial.print("Timestamp: "); Serial.println(time(nullptr));
    Serial.println("Payload sent:"); serializeJsonPretty(doc, Serial);
    Serial.println("================================");
  }
  else
  {
    Serial.println("MQTT publish FAIL");
  }

  delay(3000); // gửi mỗi 10 giây
>>>>>>> main
}


<<<<<<< HEAD

=======
>>>>>>> cba0e27 (cập nhật logic sinh dữ liệu thời tiết theo thời gian thực)
