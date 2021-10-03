
#include <Arduino.h>
#include "Sink.h"
#include "UdpSink.h"
#include "MqttSink.h"
using namespace std;

// TODO - Append all sinks to a list of sink-pointers.
// This way we can create/remove sinks on the fly as well as have multiple sinks of the same type
// Doesn't work due to loadprohibited exception?
// std::vector<Sink*> sinks;

// For the time being - instead of vector of pointers, work with plain clients. Less dynamic though...
MqttSink mqtt_sink;
UdpSink udp_sink;
bool mqtt_enabled  = false;
bool udp_enabled = false;


void setupMqtt() {
    Serial.println("setup mqtt");
    mqtt_sink.setup();
    mqtt_enabled = true;
}

void setupUdp() {
    Serial.println("setup udp");
    udp_sink.setup();
    udp_enabled = true;
}

void disableMqtt() {
    mqtt_enabled = false;
}

void disableUdp() {
    udp_enabled = false;
}


void loopSinks() {
    if (mqtt_enabled) {
        mqtt_sink.loop();
    }
    if (udp_enabled) {
        udp_sink.loop();
    }
}
void sendMessage(String message) {
    if (mqtt_enabled) {
        mqtt_sink.sendMessage(message);
    }
    if (udp_enabled) {
        udp_sink.sendMessage(message);
    }
}
void sendInputsData(uint8_t pin_index, uint8_t pin_value){
    if (mqtt_enabled) {
        mqtt_sink.sendInputsData(pin_index, pin_value);
    }
    if (udp_enabled) {
        udp_sink.sendInputsData(pin_index, pin_value);
    }
}

