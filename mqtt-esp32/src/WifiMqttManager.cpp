#include "WifiMqttManager.h"

WifiMqttManager::WifiMqttManager(const char *ssid, const char *password, const char *mqtt_server, int mqtt_port, const char *mqtt_user, const char *mqtt_pass, const char *device_id, String topic_control, WiFiClientSecure *espClient, PubSubClient *client)
    : _ssid(ssid), _password(password), _mqtt_server(mqtt_server), _mqtt_port(mqtt_port), _mqtt_user(mqtt_user), _mqtt_pass(mqtt_pass), _device_id(device_id), _topic_control(topic_control), _espClient(espClient), _client(client) {}

void WifiMqttManager::setupWifi()
{
    Serial.print("Connecting to WiFi");
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println("\nWiFi connected!");
}

void WifiMqttManager::setupMqtt(void (*callback)(char *, byte *, unsigned int))
{
    _espClient->setInsecure();
    _client->setServer(_mqtt_server, _mqtt_port);
    _client->setCallback(callback);
    _client->setBufferSize(1024);
}

void WifiMqttManager::reconnectMqtt()
{
    while (!_client->connected())
    {
        Serial.print("Connecting to MQTT...");
        if (_client->connect(_device_id, _mqtt_user, _mqtt_pass))
        {
            Serial.println("connected");
            _client->subscribe(_topic_control.c_str());
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.println(_client->state());
            delay(2000);
        }
    }
}
