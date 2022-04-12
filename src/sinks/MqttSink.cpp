#include "defines.h"

#include <ArduinoJson.h>
#include "MqttSink.h"

#define MQTT_MAX_PACKET_SIZE 256

/*--------------------------JSON--------------------------*/
StaticJsonDocument<256> json_doc;

void MqttSink::setup()
{
    Serial.print("Init MQTT sink to ");
    Serial.println(server);
    clientMQTT.setServer(server, 1883);
    clientMQTT.setCallback(callback);
}

void MqttSink::loop() {
    if (!clientMQTT.connected()) {
        Serial.println("MQTT sink not connected! reconnecting...");
        
        reconnect();
    }
    clientMQTT.loop();
}

void  MqttSink::sendMessage(String message) {
    char buffer[message.length() - 1];
    strcpy(buffer, message.c_str());

    if (!clientMQTT.publish(topic, buffer, false)) {
        Serial.println("Error sending message");
    }
}

void MqttSink::sendInputsData(uint8_t pin_index, uint8_t pin_value)
{
    json_doc.clear();
    if (pin_value == 0) {
        json_doc["state"] = "OFF";
    } else {
        json_doc["state"] = "ON";
    }

    char buffer[4096];
    serializeJson(json_doc, buffer);

    String topic = topicTemplate + String("input/") + String(pin_index);

    if (!clientMQTT.publish(topic.c_str(), buffer, false)) {
        Serial.println("Error sending message");
    }
}

void MqttSink::sendOutputData(uint8_t pin_index, uint8_t pin_value)
{
    json_doc.clear();
    if (pin_value == 0) {
        json_doc["state"] = "OFF";
    } else {
        json_doc["state"] = "ON";
    }

    char buffer[4096];
    serializeJson(json_doc, buffer);
    String topic = topicTemplate + String("output/") + String(pin_index);
    
    #ifdef LOCAL_DEBUG
    Serial.print("Sending output data on ");
    Serial.println(topic);
    #endif

    if (!clientMQTT.publish(topic.c_str(), buffer, false)) {
        Serial.println("Error sending message");
    }
}

void MqttSink::reconnect()
{
    while (!clientMQTT.connected()) {
        Serial.println("Attempting MQTT connection...");
        // Attempt to connect
        if (clientMQTT.connect("Ciiixo_client_sink", username, password)) {
            Serial.println("connected");
        } else {
            Serial.print("MQTT connect failed, rc=");
            Serial.print(clientMQTT.state());
            delay(2000);
        }
    }
}

void MqttSink::callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("MQTT sink received message from topic: ");
    Serial.print(topic);
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void MqttSink::setServer(char* server) {
    server = server;
}