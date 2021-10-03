#include "MqttSink.h"
#include <ArduinoJson.h>

#define MQTT_MAX_PACKET_SIZE 256

/*--------------------------JSON--------------------------*/
StaticJsonDocument<256> json_doc;

/*--------------------------MQTT--------------------------*/
// const char* mqtt_server = "5.196.95.208";
// const char* mqtt_server = "172.16.128.166";
// const char* mqtt_server = "192.168.1.142";
// WiFiClient espClient;
// PubSubClient clientMQTT(espClient);


void MqttSink::loop() {
    if (!clientMQTT.connected()) {
        Serial.println("mqtt not connected! reconnecting...");
        reconnect();
    }
    clientMQTT.loop();
}

void  MqttSink::sendMessage(String message) {
    String topic = "ciiixo/io/message";

    char buffer[message.length() - 1];
    strcpy(buffer, message.c_str());

    if (!clientMQTT.publish(topic.c_str(), buffer, false)) {
        Serial.println("Error sending message");
    }
}

void MqttSink::sendInputsData(uint8_t pin_index, uint8_t pin_value)
{
    json_doc.clear();
    json_doc["idx"] = String(pin_index);
    json_doc["value"] = String(pin_value);

    char buffer[4096];
    serializeJson(json_doc, buffer);

    String topic = "ciiixo/io/" + String(pin_index);

    if (!clientMQTT.publish(topic.c_str(), buffer, false)) {
        Serial.println("Error sending message");
    }
}

void MqttSink::reconnect()
{
    while (!clientMQTT.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (clientMQTT.connect("Ciiixo_client", "Lynx", "LynX1234")) {
            Serial.println("connected");
        } else {
            Serial.print("MQTT connect failed, rc=");
            Serial.print(clientMQTT.state());
            delay(2000);
        }
    }
}

void MqttSink::setup()
{
    Serial.println("Init MQTT");
    clientMQTT.setServer(mqtt_server, 1883);
    Serial.println(mqtt_server);
    clientMQTT.setCallback(callback);
}

void MqttSink::callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void MqttSink::setServer(char* server) {
    mqtt_server = server;
}