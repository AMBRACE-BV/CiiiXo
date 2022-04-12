#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "Source.h"

String getTopicWildcard(String, char, int);

class MqttSource : public Source {
    public:
        void setup();
        void loop();
        void setServer(char*);
        void setCredentials(char*, char*);
    protected:
        char* server = (char*) "192.168.10.20";   
        // first topic entity should be configurable through interface (multiple ciiixo boards) - wildcard denotes the pin    
        char* topic = (char*) "ciiixo/pin/+/set"; 
        char* username = (char*) "mqtt";
        char* password = (char*) "mqtt";

        WiFiClient espClient;
        PubSubClient clientMQTT = PubSubClient(espClient);

        void reconnect();
        static void callback(char* topic, byte* payload, unsigned int length); 
};