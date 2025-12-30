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

  currentMode = MODE_ALERT;
  blinkCount = 0;
  lcdVisible = true;
  lastToggle = millis();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(alertLine1);
  lcd.setCursor(0, 1);
  lcd.print(alertLine2);
}

// =====================
void lcd_loop()
{
  if (currentMode != MODE_ALERT) return;

  unsigned long now = millis();

  // ===== ĐANG HIỂN THỊ =====
  if (lcdVisible && (now - lastToggle >= ALERT_SHOW_TIME))
  {
    lcd.clear();              // tắt LCD
    lcdVisible = false;
    lastToggle = now;
    blinkCount++;             // đếm số lần hiển thị xong
  }

  // ===== ĐANG TẮT =====
  else if (!lcdVisible && (now - lastToggle >= ALERT_HIDE_TIME))
  {
    // nếu chưa đủ số lần → hiển lại
    if (blinkCount < ALERT_REPEAT)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(alertLine1);
      lcd.setCursor(0, 1);
      lcd.print(alertLine2);

      lcdVisible = true;
      lastToggle = now;
    }
    else
    {
      // đủ số lần → quay về sensor
      currentMode = MODE_SENSOR;
      lcd.clear();
    }
  }
}

