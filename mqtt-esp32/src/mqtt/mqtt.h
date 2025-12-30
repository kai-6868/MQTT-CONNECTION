#pragma once
#include <PubSubClient.h>

extern PubSubClient mqttClient;

void mqtt_init();
void mqtt_loop();
void mqtt_publish(const char* topic, const char* payload, size_t len);
