#include "UdpSource.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ETH.h>

#include "IOManager.h"

void UdpSource::setup(){
    // Nothing to do here
};

void UdpSource::loop() {
    if (!udpClient.listen(1235)) {
        return;
    }
    udpClient.onPacket([](AsyncUDPPacket packet) {
        Serial.print("received packet from ");
        Serial.println(packet.remoteIP());
        Serial.print("message: ");
        Serial.write(packet.data(), packet.length());
        Serial.println();

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

        uint duration = doc["duration"].as<uint>();
        if (duration) {
            Serial.print("setting pin ");
            Serial.print(pin);
            Serial.print(" to value ");
            Serial.print(value);
            Serial.print(" for ");
            Serial.print(duration);
            Serial.println(" ms");

            // setPin(pin, value);

            // TODO - digitalwrite pin
            // need to extend PCA9698 class -> add function with args pin, value, duration (optional))
            // Function will then determine the ADDRESS -> all address logic hidden from other components!
        } else {
            Serial.print("setting pin ");
            Serial.print(pin);
            Serial.print(" to value ");
            Serial.print(value);

            setPin(pin, value);

            // TODO - see above, non-empty duration
        }
    });
}