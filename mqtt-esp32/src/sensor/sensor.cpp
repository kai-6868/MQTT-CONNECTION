#include "sensor.h"
#include "../config/config.h"
#include <DHTesp.h>
#include "SharpGP2Y10.h"

static DHTesp dht;
static SharpGP2Y10 dustSensor(PIN_DUST_OUT, PIN_DUST_LED, 3.3);

void sensor_init()
{
  dht.setup(PIN_DHT, DHTesp::DHT11);
}

SensorData sensor_read()
{
  SensorData s;
  auto data = dht.getTempAndHumidity();

  s.temperature = data.temperature;
  s.humidity = data.humidity;
  s.light = analogRead(PIN_LIGHT);
  s.rain = analogRead(PIN_RAIN);
  s.dust = dustSensor.getDustDensity() * 1000;

  return s;
}
