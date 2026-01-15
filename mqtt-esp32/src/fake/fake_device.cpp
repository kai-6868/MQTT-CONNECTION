#include "fake_device.h"
#include <Arduino.h>
#include <time.h>

void fake_device_init() {
  // Khởi tạo seed ngẫu nhiên từ phần cứng
  randomSeed(esp_random());

  // Cấu hình thời gian thực qua NTP (Giao thức thời gian mạng)
  // 7 * 3600: Múi giờ Việt Nam (GMT+7)
  // 0: Không sử dụng giờ mùa hè (Daylight saving)
  configTime(7 * 3600, 0, "pool.ntp.org", "time.google.com");

  Serial.println("Đang đồng bộ thời gian thực từ Internet...");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    delay(500);
    Serial.print(".");
  }
  Serial.print(" Thời gian hiện tại: ");
  Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n",
                timeinfo.tm_year + 1900,
                timeinfo.tm_mon + 1,
                timeinfo.tm_mday,
                timeinfo.tm_hour,
                timeinfo.tm_min,
                timeinfo.tm_sec);
  Serial.println("\n[OK] Đã xác định thời gian thực!");
}

SensorData fake_device_generate() {
  // 1. Lấy cấu trúc thời gian hiện tại
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Lỗi: Không lấy được thời gian thực!");
  }

  // Chuyển đổi sang số thực (Ví dụ: 16h20p = 16.33) để tính toán toán học mượt mà
  float current_hour = timeinfo.tm_hour + (timeinfo.tm_min / 60.0);

  // 2. NHIỆT ĐỘ: Logic hình sin theo chu kỳ mặt trời
  // T-min lúc 05:00, T-max lúc 14:00. Biên độ Hà Nội mùa này: 17 - 27 độ C.
  float day_progress = (current_hour - 5.0) * (M_PI / 12.0);
  float temp = 22.0 + 5.0 * sin(day_progress - (M_PI / 2.0));
  temp += (random(-5, 6) * 0.05); // Nhiễu nhẹ 0.05 độ

  // 3. ĐỘ ẨM: Nghịch biến với nhiệt độ (Rất sát thực tế)
  float hum = 135.0 - (temp * 3.6); 
  hum = constrain(hum + (random(-10, 11) * 0.1), 40, 95);

  // 4. ÁNH SÁNG: Mô phỏng cường độ tia cực tím/ánh sáng theo vị trí mặt trời
  int light = 0;
  if (current_hour > 6.0 && current_hour < 18.2) { // Từ 6h sáng đến 6h20 chiều
    float sun_intensity = sin((current_hour - 6.0) * (M_PI / 12.2));
    light = (int)(950 * sun_intensity) + random(-30, 31);
  } else {
    light = random(0, 10); // Ánh sáng nhiễu ban đêm
  }

  // 5. BỤI MỊN (PM2.5): Logic giờ cao điểm Quận Hoàng Mai
  float dust;
  // Sáng: 07:30 - 09:00 | Chiều: 17:00 - 19:30 (Kẹt xe và khí thải tăng cao)
  if ((current_hour >= 7.5 && current_hour <= 9.0) || (current_hour >= 17.0 && current_hour <= 19.5)) {
    dust = 180.0 + random(20, 110); // Ngưỡng ô nhiễm
  } else {
    dust = 55.0 + random(0, 45);    // Mức trung bình/tốt
  }

  // 6. MƯA: Mô phỏng xác suất ngẫu nhiên nhưng giữ trạng thái
  static int rain_state = 0;
  if (rain_state == 0) {
    if (random(0, 5000) > 4998) rain_state = 600; // Xác suất bắt đầu mưa thấp
  } else {
    if (random(0, 100) < 4) rain_state = 0; // Tỷ lệ tạnh mưa sau mỗi phút
  }

  // Đóng gói dữ liệu vào Struct
  SensorData s;
  s.temperature = temp;
  s.humidity = hum;
  s.dust = dust;
  s.light = constrain(light, 0, 1000);
  s.rain = rain_state;

  return s;
}