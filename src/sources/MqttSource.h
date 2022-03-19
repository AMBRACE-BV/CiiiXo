#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

class MqttSource {
    public:
        void setup();
        void loop();
        void setServer(char*);
    protected:
        char* server = "192.168.1.142";      
        char* topic = (char*) "peripheral/+/action";
        char* username = (char*) "";
        char* password = (char*) "";

        WiFiClient espClient;
        PubSubClient clientMQTT = PubSubClient(espClient);

        void reconnect();
        static void callback(char* topic, byte* payload, unsigned int length); 
};