#include "defines.h"

#include <Arduino.h>
#include <ETH.h>
#include "UdpSink.h"

void UdpSink::setup() {

    IPAddress ip = ETH.localIP();
    if (!broadcast) {
        ip = this->remoteIp;
    }

    if (udpClient.connect(ip, this->port)) {
        Serial.println("succesfully connected udp sink!");
    }
}

void UdpSink::loop() {
    if (!udpClient.connected()) {
        this->setup();
    }
}

void UdpSink::sendMessage(String message) {
    if (!udpClient.connected()) {
        Serial.println("udp client unavailable!");
        return;
    }
    
    if (this->broadcast) {
        if (!udpClient.broadcastTo(message.c_str(), this->port)) {
            Serial.println("failed to broadcast udp message");
        }
    } else {
        if (!udpClient.print(message)) {
            Serial.println("failed sending udp message");
        }
    }
}

void UdpSink::sendInputsData(uint8_t pin_index, uint8_t pin_value){
    // NOTE - inverted pin value !
    String msg = "pin " + String(pin_index) + " - " + (1 - pin_value);
    this->sendMessage(msg);

}

void UdpSink::setRemote(IPAddress ip, int port) {
    remoteIp = ip;
    this->port = port;
}
