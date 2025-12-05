#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "time.h"
#include <DHTesp.h>


// ----- Thông tin thiết bị -----
const char* device_id = "esp32_devkit_01";

// ----- Thông tin WiFi -----
const char* ssid = "P910";
const char* password = "12345678";

// ----- Thông tin HiveMQ Cloud (username/password) -----
const char* mqtt_server = ".s1.eu.hivemq.cloud"; // ví dụ: yourname.hivemq.cloud
const int mqtt_port = 8883;                     // TLS port
const char* mqtt_user = "20251";       // điền username HiveMQ Cloud
const char* mqtt_pass = "i0251";       // điền password HiveMQ Cloud

String topic_data = "WeatherNestIOT/" + String(device_id) + "/data";      // topic gửi dữ liệu
String topic_control = "WeatherNestIOT/" + String(device_id) + "/control";        // topic nhận lệnh

// ----- Múi giờ -----
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

// ----- MQTT client -----
WiFiClientSecure espClient;
PubSubClient client(espClient);

// ----- Callback khi có message từ broker -----
void callback(char* topic, byte* payload, unsigned int length) {
  // Chuyển payload thành String
  String msg = "";
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.print("Received control JSON: ");
  Serial.println(msg);

  // Parse JSON
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, msg);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  // ----- Xử lý module trực tiếp -----
  if (doc.containsKey("led")) {
    JsonObject value = doc["led"].as<JsonObject>();
    const char* state = value["state"];
    Serial.print("LED state: "); Serial.println(state);

    if (strcmp(state, "ON") == 0) {
      //todo: bật LED
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Control LED turned ON");
    } else if (strcmp(state, "OFF") == 0) {
      //todo: tắt LED
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Control LED turned OFF");
    }
  }
  // Nếu muốn thêm module khác
  else if (doc.containsKey("dht22")) {
    JsonObject value = doc["dht22"].as<JsonObject>();
    float temperature = value["temperature_c"] | 0;
    float humidity = value["humidity_pct"] | 0;
    Serial.print("DHT22 -> Temp: "); Serial.print(temperature);
    Serial.print(", Humidity: "); Serial.println(humidity);
  }
  else {
    Serial.println("Unknown module in control JSON");
  }
}


// ----- Kết nối WiFi -----
void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("RSSI: "); Serial.println(WiFi.RSSI());
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

// ----- Kết nối MQTT -----
void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    // Sử dụng username/password để bảo mật
    if (client.connect(device_id, mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe(topic_control.c_str()); // nhận lệnh từ broker
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Kết nối WiFi
  setup_wifi();

  // Cấu hình thời gian NTP
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  } else {
    Serial.println(&timeinfo, "Current time: %Y-%m-%d %H:%M:%S");
  }

  // MQTT
  espClient.setInsecure(); // test TLS, production: dùng setCACert()
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect_mqtt();

  // Subscribe topic control
  client.subscribe(topic_control.c_str());
}

void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();

  // ----- Tạo JSON -----
  DynamicJsonDocument doc(2048);
  doc["device_id"] = device_id;

  time_t now = time(nullptr);
  doc["timestamp"] = now;

  doc["battery_v"] = 3.86;

  JsonObject wifi = doc.createNestedObject("wifi");
  wifi["wifi_ssid"] = WiFi.SSID();
  wifi["wifi_rssi_dbm"] = WiFi.RSSI();

  JsonObject dht22 = doc.createNestedObject("dht22");
  dht22["temperature_c"] = 29.4;
  dht22["humidity_pct"] = 72.1;

  JsonObject ledObj = doc.createNestedObject("led");
  ledObj["state"] = "ON";

  // ----- Gửi JSON tới HiveMQ -----
  char buffer[2048];
  serializeJson(doc, buffer);
  client.publish(topic_data.c_str(), buffer);

  // In ra Serial
  serializeJsonPretty(doc, Serial);
  Serial.println();

  delay(5000);
}
