using namespace std;

#include "defines.h"
#include "UdpSource.h"
#include "MqttSource.h"

MqttSource mqtt_source;
UdpSource udp_source;

void setupSources() {
    #ifdef UDP_SOURCE
    udp_source.setEnabled(true);
    udp_source.setup();
    #endif
    #ifdef MQTT_SOURCE
    mqtt_source.setEnabled(true);
    mqtt_source.setup();
    #endif
}

void loopSources() {
    // TODO: see if this can be simplified by putting all sources in a list?    
    if (mqtt_source.enabled) mqtt_source.loop();
    if (udp_source.enabled) udp_source.loop();
}

void setMqttSourceEnabled(bool enabled) {
    mqtt_source.setEnabled(enabled);
}
void setUdpSourceEnabled(bool enabled) {
    udp_source.setEnabled(enabled);
}
