#include "LcdDisplay.h"

LcdDisplay::LcdDisplay(uint8_t addr, uint8_t cols, uint8_t rows)
    : _lcd(addr, cols, rows) {}

void LcdDisplay::begin() {
    _lcd.init();
    _lcd.backlight();
}

void LcdDisplay::show(float temperature, float humidity, float dustDensity, int lightValue) {
    // _lcd.clear(); // Uncomment if you want to clear every update
    _lcd.setCursor(0, 0);
    _lcd.print("T:");
    _lcd.print(temperature, 1);
    _lcd.print(" H:");
    _lcd.print(humidity, 1);

    _lcd.setCursor(0, 1);
    _lcd.print("D:");
    _lcd.print(dustDensity, 1);
    _lcd.print(" L:");
    _lcd.print(lightValue);
}
