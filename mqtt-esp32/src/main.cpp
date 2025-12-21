#include <Arduino.h>
#include <ArduinoJson.h>
#include "JsonPublisher.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "WifiMqttManager.h"
#include "time.h"
#include <DHTesp.h>
#include "SensorManager.h"
#include "LcdDisplay.h"
#include "SharpGP2Y10.h"
#include <DFRobotDFPlayerMini.h>

// ===== Cảm biến =====

const int buttonPin = 0;
const int lightSensorPin = 32;
const int rainSensorPin = 35;
const int dhtPin = 19;

SensorManager sensorManager(dhtPin, lightSensorPin, rainSensorPin);

SharpGP2Y10 dustSensor(34, 33, 3.3);


LcdDisplay lcdDisplay(0x27, 16, 2);

// ===== Thông tin thiết bị =====
const char *device_id = "esp32_devkit_01";

// ===== WiFi =====
const char *ssid = "Nguyen Thanh Huyen";
const char *password = "Huyen1978";

// ===== MQTT Server =====
const char *mqtt_server = "connection.ducbinh203.tech";
const int mqtt_port = 8883; // TLS
const char *mqtt_user = "iot-mqtt";
const char *mqtt_pass = "abcd1234";

String topic_data = "ecosense/devices/" + String(device_id) + "/data";
String topic_control = "ecosense/devices/" + String(device_id) + "/control";

// ===== Time =====
const long gmtOffset_sec = 7 * 3600;

WiFiClientSecure espClient;
PubSubClient client(espClient);
WifiMqttManager wifiMqttManager(
    ssid, password, mqtt_server, mqtt_port, mqtt_user, mqtt_pass, device_id, topic_control, &espClient, &client);

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
  for (unsigned int i = 0; i < length; i++)
    msg += (char)payload[i];

  Serial.print("Received JSON: ");
  Serial.println(msg);

  DynamicJsonDocument doc(256);
  if (deserializeJson(doc, msg))
    return;

  if (doc.containsKey("led"))
  {
    const char *state = doc["led"]["state"];
    digitalWrite(LED_BUILTIN, strcmp(state, "ON") == 0);
    Serial.println(String("LED -> ") + state);
  }
}


// ======================
//   PUBLISH SENSOR DATA
// ======================
void publishSensorData() {
  // Đọc cảm biến
  float temperature = sensorManager.readTemperature();
  float humidity = sensorManager.readHumidity();
  int lightValue = sensorManager.readLight();
  int rainValue = sensorManager.readRain();
  float dustDensity = dustSensor.getDustDensity() * 1000;

  // Hiển thị LCD
  lcdDisplay.show(temperature, humidity, dustDensity, lightValue);
  Serial.print("Dust Density: ");
  Serial.print(dustDensity);
  Serial.println(" ug/m3");

  // JSON
  DynamicJsonDocument doc(512);
  JsonPublisher::buildPayload(doc, device_id, temperature, humidity, dustDensity, lightValue, rainValue);

  // Serialize & Publish
  char buffer[512];
  size_t len = serializeJson(doc, buffer);

  bool success = client.publish(topic_data.c_str(), buffer, len);
  if (success) {
    Serial.println("===== MQTT PUBLISH SUCCESS =====");
    Serial.print("Topic: ");
    Serial.println(topic_data);
    Serial.print("Timestamp: ");
    Serial.println(time(nullptr));
    Serial.println("Payload sent:");
    serializeJsonPretty(doc, Serial);
    Serial.println("================================");
  } else {
    Serial.println("MQTT publish FAIL");
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  sensorManager.begin();

  lcdDisplay.begin();

  wifiMqttManager.setupWifi();

  configTime(gmtOffset_sec, 0, "pool.ntp.org");

  wifiMqttManager.setupMqtt(callback);
  wifiMqttManager.reconnectMqtt();
}

void loop()
{
  if (!client.connected())
    wifiMqttManager.reconnectMqtt();
  client.loop();

<<<<<<< HEAD
  publishSensorData();
  delay(10000); // gửi mỗi 10 giây
=======
  // ===== Đọc cảm biến =====
  TempAndHumidity data = dht.getTempAndHumidity();
  float temperature = data.temperature;
  float humidity = data.humidity;

  int lightValue = analogRead(lightSensorPin);
  int rainValue = analogRead(rainSensorPin);
  float dustDensity = dustSensor.getDustDensity()* 1000;

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
>>>>>>> 89b475d8291e507aca4f5a62b60f78f39f00744e
}
