#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal_I2C.h>

class LcdDisplay {
public:
    LcdDisplay(uint8_t addr, uint8_t cols, uint8_t rows);
    void begin();
    void show(float temperature, float humidity, float dustDensity, int lightValue);
private:
    LiquidCrystal_I2C _lcd;
};

#endif // LCD_DISPLAY_H
