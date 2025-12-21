#ifndef JSON_PUBLISHER_H
#define JSON_PUBLISHER_H

#include <ArduinoJson.h>

class JsonPublisher {
public:
    static void buildPayload(DynamicJsonDocument& doc,
                            const char* device_id,
                            float temperature,
                            float humidity,
                            float dustDensity,
                            int lightValue,
                            int rainValue);
};

#endif // JSON_PUBLISHER_H
