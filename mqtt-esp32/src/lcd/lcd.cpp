#include "lcd.h"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

static LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== State =====
enum LcdMode
{
  MODE_SENSOR,
  MODE_ALERT
};

static LcdMode currentMode = MODE_SENSOR;

// ===== Alert data =====
static String alertLine1;
static String alertLine2;

static uint8_t blinkCount = 0;
static bool lcdVisible = true;
static unsigned long lastToggle = 0;

// timing
#define ALERT_SHOW_TIME   1000   // hiển thị 5s
#define ALERT_HIDE_TIME   1000  // tắt 3s
#define ALERT_REPEAT      3


// =====================
void lcd_init()
{
  lcd.init();
  lcd.backlight();

  Serial.printf("[LCD] Initialized.\n");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EcoSense");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
}

// =====================
void lcd_show_sensor(const SensorData& data)
{
  if (currentMode != MODE_SENSOR) return;

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(data.temperature, 1);
  lcd.print(" H:");
  lcd.print(data.humidity, 1);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("D:");
  lcd.print(data.dust, 0);
  lcd.print(" L:");
  lcd.print(data.light);
  lcd.print("   ");
}

// =====================
void lcd_show_alert(const char* line1, const char* line2)
{
  alertLine1 = line1;
  alertLine2 = line2;

  // Hiển thị cảnh báo theo kiểu blocking: hiện -> delay -> tắt -> delay
  // Lặp `ALERT_REPEAT` lần rồi trở về chế độ sensor.
  currentMode = MODE_ALERT;

  for (uint8_t i = 0; i < ALERT_REPEAT; i++)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(alertLine1);
    lcd.setCursor(0, 1);
    lcd.print(alertLine2);

    delay(ALERT_SHOW_TIME);

    lcd.clear();
    delay(ALERT_HIDE_TIME);
  }

  // Sau khi nhấp nháy xong, quay về chế độ hiển thị cảm biến
  currentMode = MODE_SENSOR;
}

// =====================
void lcd_loop()
{
  // Không dùng non-blocking blinking nữa. Việc hiển thị cảnh báo
  // đã được thực hiện đồng bộ trong `lcd_show_alert()` bằng `delay()`.
  (void)0;
}

