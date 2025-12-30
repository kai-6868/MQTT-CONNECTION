#pragma once
#include "../sensor/sensor.h"

// Khởi tạo fake device
void fake_device_init();

// Sinh dữ liệu giả (thay đổi đều)
SensorData fake_device_generate();
