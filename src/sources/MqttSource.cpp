#include "defines.h"
#include <ArduinoJson.h>

#include "MqttSource.h"
#include "IOManager.h"

#define MQTT_MAX_PACKET_SIZE 256


void MqttSource::setServer(char* server) {
    server = server;
}

void MqttSource::setCredentials(char* username, char* password) {
    username = username;
    password = password;
}

void MqttSource::loop() {

    if (this->enabled) {
        return;
    }

    if (!clientMQTT.connected()) {
        Serial.println("MQTT not connected! reconnecting...");
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
    Serial.print("Init MQTT source to ");
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
        Serial.print("Deserialization failed: ");
        Serial.println(error.c_str());
        return;
    }

    uint8_t pin = getTopicWildcard(String(topic), '/', 2).toInt();
    // uint8_t pin = doc["pin"].as<uint8_t>();
    uint8_t value = doc["value"].as<uint8_t>();
    uint32_t duration = doc["duration"].as<uint32_t>();

    if (!pin) {
        Serial.println("Failed to extract pin from topic");
        return;
    };
    if (!value) {
        Serial.println("Missing json key - expected value");
        return;
    };

    if (duration) {
        setPin(pin, value, duration);
    } else {
        setPin(pin, value);
    }
}

String getTopicWildcard(String topic, char separator, int index) {
    int startIndex = topic.indexOf(separator) + 1;
    if (startIndex == -1) {
        // separator not found - return full topic
        return topic;
    }

    int endIndex = topic.indexOf(separator) - 1;
    endIndex = endIndex > 0 ? endIndex : topic.length(); 

    return topic.substring(startIndex, endIndex);

}