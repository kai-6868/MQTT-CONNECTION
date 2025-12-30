#include "fake_device.h"
#include <Arduino.h>

// Giá trị ban đầu
static float temp = 25.0;
static float hum  = 60.0;
static float dust = 120.0;

static int light = 500;
static int rain  = 0;

static int direction = 1;

void fake_device_init()
{
  randomSeed(esp_random());
}

SensorData fake_device_generate()
{
  // Dao động nhiệt độ 23 → 30
  temp += 0.2 * direction;
  if (temp > 30 || temp < 23) direction *= -1;

  // Độ ẩm trôi chậm
  hum += random(-5, 6) * 0.1;
  hum = constrain(hum, 40, 80);

  // Bụi dao động
  dust += random(-10, 11);
  dust = constrain(dust, 50, 300);

  // Ánh sáng thay đổi chậm
  light += random(-30, 31);
  light = constrain(light, 100, 900);

  // Mưa (thỉnh thoảng có)
  rain = random(0, 100) > 90 ? 800 : 0;

  SensorData s;
  s.temperature = temp;
  s.humidity = hum;
  s.dust = dust;
  s.light = light;
  s.rain = rain;

  return s;
}
