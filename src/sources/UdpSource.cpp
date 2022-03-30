#include <Arduino.h>
#include <ArduinoJson.h>
#include <ETH.h>

#include "defines.h"
#include "UdpSource.h"
#include "IOManager.h"


void UdpSource::setup(){
    // Nothing to do here
};

void UdpSource::loop() {
    if (!udpClient.listen(this->port)) {
        return;
    }
    udpClient.onPacket([](AsyncUDPPacket packet) {
        #ifdef LOCAL_DEBUG
        Serial.print("received packet from ");
        Serial.println(packet.remoteIP());
        Serial.print("message: ");
        Serial.write(packet.data(), packet.length());
        Serial.println();
        #endif
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, packet.data());
        if (error) {
            Serial.print("deserialization failed: ");
            Serial.println(error.c_str());
            return;
        }

        uint8_t pin = doc["pin"].as<uint8_t>();
        uint8_t value = doc["value"].as<uint8_t>();

        if (!pin || !value) {
            Serial.println("missing json keys - expecting pin and value");
        };

        uint32_t duration = doc["duration"].as<uint32_t>();
        if (duration) {
            setPin(pin, value, duration);
        } else {
            setPin(pin, value);
        }
    });
}

void UdpSource::setPort(int port) {
    this->port = port;
}