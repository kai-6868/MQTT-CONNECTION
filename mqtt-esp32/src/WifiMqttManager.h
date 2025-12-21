#ifndef WIFI_MQTT_MANAGER_H
#define WIFI_MQTT_MANAGER_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class WifiMqttManager
{
public:
    WifiMqttManager(const char *ssid, const char *password, const char *mqtt_server, int mqtt_port, const char *mqtt_user, const char *mqtt_pass, const char *device_id, String topic_control, WiFiClientSecure *espClient, PubSubClient *client);
    void setupWifi();
    void setupMqtt(void (*callback)(char *, byte *, unsigned int));
    void reconnectMqtt();

private:
    const char *_ssid;
    const char *_password;
    const char *_mqtt_server;
    int _mqtt_port;
    const char *_mqtt_user;
    const char *_mqtt_pass;
    const char *_device_id;
    String _topic_control;
    WiFiClientSecure *_espClient;
    PubSubClient *_client;
};

#endif // WIFI_MQTT_MANAGER_H
