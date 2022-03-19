#include "MqttSource.h"
#include <ArduinoJson.h>
#include "IOManager.h"

#define MQTT_MAX_PACKET_SIZE 256

/*--------------------------JSON--------------------------*/
StaticJsonDocument<256> json_doc;

void MqttSource::loop() {

    if (server == "") {
        return;
    }

    if (!clientMQTT.connected()) {
        Serial.println("mqtt not connected! reconnecting...");
        reconnect();
    }
    clientMQTT.loop();
}

void MqttSource::reconnect()
{
    while (!clientMQTT.connected()) {
        Serial.print("Attempting MQTT connection for the mqtt-source...");
        // Attempt to connect
        if (clientMQTT.connect("Ciiixo_client", username, password)) {
            Serial.print("Connected! Subscribing to mqtt topic ");
            Serial.println(topic);
            clientMQTT.subscribe(topic);
        } else {
            Serial.print("MQTT connect failed, rc=");
            Serial.print(clientMQTT.state());
            delay(2000);
        }
    }
}

void MqttSource::setup()
{
    Serial.print("Init mqtt-source to ");
    Serial.println(server);
    clientMQTT.setServer(server, 1883);
    clientMQTT.setCallback(callback);
}

void MqttSource::callback(char* topic, byte* payload, unsigned int length)
{
    #ifdef DEBUG
    Serial.print("Mqtt-source received message from topic: ");
    Serial.print(topic);
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    #endif

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
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
}

void MqttSource::setServer(char* server) {
    server = server;
}