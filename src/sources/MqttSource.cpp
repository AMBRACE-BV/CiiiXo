#include <ArduinoJson.h>

#include "defines.h"
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
        if (clientMQTT.connect("Ciiixo_client_source", username, password)) {
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
    #ifdef LOCAL_DEBUG
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

    String pinStr = getTopicWildcard(String(topic), '/', 2);
    String valueStr = doc["value"];
    uint32_t duration = doc["duration"].as<uint32_t>();

    if (!pinStr.length()) {
        Serial.println("Failed to extract pin from topic");
        return;
    };
    if (!valueStr.length()) {
        Serial.println("Missing json key - expected value");
        return;
    };

    int pin = pinStr.toInt();
    uint8_t value = (uint8_t) valueStr.toInt();


    if (duration) {
        setPin(pin, value, duration);
    } else {
        setPin(pin, value);
    }
}

String getTopicWildcard(String topic, char separator, int index) {
    // Given the mqtt topic, returns the wildcard found at 'index', starting from 0 for the first part of the topic.
    int startIndex = index == 0 ? 0 : -1;
    int endIndex = -1;
    int indexCount = 0;
    for (int i = 0; i < topic.length(); i++) {
        if (topic[i] != separator) {
            continue;
        } 
        indexCount += 1;
        if (indexCount == index && startIndex == -1) {
            startIndex = i;
        }
        if (indexCount == index + 1 && endIndex == -1) {
            endIndex = i;
            break;
        }
    }
    if (startIndex == -1) {
        // separator not found - return full topic
        return topic;
    }

    endIndex = endIndex > 0 ? endIndex : topic.length(); 
    return topic.substring(startIndex + 1, endIndex);
}