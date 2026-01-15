#pragma once
#include "../sensor/sensor.h"

void lcd_init();
void lcd_show_sensor(const SensorData& data);

// control
void lcd_show_alert(const char* line1, const char* line2);
void lcd_loop();   // state machine
