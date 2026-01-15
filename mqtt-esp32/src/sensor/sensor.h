#pragma once

struct SensorData
{
  float temperature;
  float humidity;
  float dust;
  int light;
  int rain;
};

void sensor_init();
SensorData sensor_read();
