#include <Arduino.h>

#include "defines.h"
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

void setupSinks() {
    #ifdef UDP_SINK
    udp_sink.setEnabled(true);
    udp_sink.setup();
    #endif
    #ifdef MQTT_SINK
    mqtt_sink.setEnabled(true);
    mqtt_sink.setup();
    #endif
}

void loopSinks() {
    if (mqtt_sink.enabled) mqtt_sink.loop();
    if (udp_sink.enabled) udp_sink.loop();
}

void sendMessage(String message) {
    if (mqtt_sink.enabled) mqtt_sink.sendMessage(message);
    if (udp_sink.enabled) udp_sink.sendMessage(message);
}
void sendInputsData(uint8_t pin_index, uint8_t pin_value){
        if (mqtt_sink.enabled) mqtt_sink.sendInputsData(pin_index, pin_value);
    if (udp_sink.enabled) udp_sink.sendInputsData(pin_index, pin_value);
}

