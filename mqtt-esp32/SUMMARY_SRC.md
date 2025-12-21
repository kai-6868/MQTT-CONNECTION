# Tóm tắt code các file trong src

---

## main.cpp
- **Biến cấu hình:**
  - `buttonPin`, `lightSensorPin`, `rainSensorPin`, `dhtPin`: Chân kết nối cảm biến.
  - `device_id`, `ssid`, `password`, `mqtt_server`, `mqtt_port`, `mqtt_user`, `mqtt_pass`: Thông tin thiết bị, WiFi, MQTT.
  - `topic_data`, `topic_control`: Chủ đề MQTT.
- **Hàm:**
  - `callback(char *topic, byte *payload, unsigned int length)`: Xử lý lệnh điều khiển từ MQTT.
  - `publishSensorData()`: Đọc cảm biến, hiển thị LCD, tạo JSON, gửi MQTT.
  - `setup()`, `loop()`: Khởi tạo và vòng lặp chính.

---

## WifiMqttManager.cpp/h
- **Biến:**
  - Lưu thông tin WiFi, MQTT, client.
- **Hàm:**
  - `WifiMqttManager(...)`: Hàm khởi tạo.
  - `setupWifi()`: Kết nối WiFi.
  - `setupMqtt(callback)`: Thiết lập MQTT client và callback.
  - `reconnectMqtt()`: Đảm bảo kết nối lại MQTT nếu mất kết nối.

---

## SensorManager.cpp/h
- **Biến:**
  - Chân cảm biến DHT, ánh sáng, mưa.
- **Hàm:**
  - `SensorManager(int dhtPin, int lightPin, int rainPin)`: Hàm khởi tạo.
  - `begin()`: Khởi tạo cảm biến DHT.
  - `readTemperature()`: Đọc nhiệt độ.
  - `readHumidity()`: Đọc độ ẩm.
  - `readLight()`: Đọc giá trị cảm biến ánh sáng.
  - `readRain()`: Đọc giá trị cảm biến mưa.

---

## SharpGP2Y10.cpp/h
- **Biến:**
  - `voPin`, `ledPin`, `vccVol`, `volMeasured`, `calcVoltage`, ...
- **Hàm:**
  - `SharpGP2Y10(int voPin, int ledPin, float vccVol)`: Hàm khởi tạo.
  - `calc()`: Đọc và tính toán giá trị bụi mịn.
  - `getDustDensity()`: Trả về nồng độ bụi mịn.
  - `getVotage()`, `getADC()`: Trả về giá trị điện áp, ADC.
  - `getDustDensityField()`, ...: Trả về giá trị đã lưu.

---

## LcdDisplay.cpp/h
- **Biến:**
  - Đối tượng LCD.
- **Hàm:**
  - `begin()`: Khởi tạo LCD.
  - `show(temperature, humidity, dustDensity, lightValue)`: Hiển thị dữ liệu lên LCD.

---

## JsonPublisher.cpp/h
- **Hàm:**
  - `buildPayload(doc, temperature, humidity, dustDensity, lightValue, rainValue)`: Tạo nội dung JSON từ dữ liệu cảm biến để gửi MQTT.

---

# Ghi chú
- Mỗi file đảm nhiệm một chức năng riêng biệt, giúp code rõ ràng, dễ bảo trì.
- Các biến toàn cục chủ yếu nằm ở main.cpp, các module chỉ lưu biến liên quan đến chức năng của mình.