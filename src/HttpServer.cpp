#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "defines.h"
#include "config.h"

/******************Web server*******************/
AsyncWebServer server(80);

void setupWebservice()
{
    // Initialize SPIFFS
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    } else {
        Serial.println("Succesfully initialized SPIFFS");
    }

    // Route web page handling
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", String(), false);
    });
    server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.css", "text/css");
    });
    server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.js", String());
    });

    // Config sources
    server.on("/config/sources", HTTP_POST, [](AsyncWebServerRequest *request){

        MqttSourceConfig mqtt_config;
        UdpSourceConfig udp_config;

        Serial.println("setting sources");
        //List all parameters
        int params = request->params();
        for(int i=0;i<params;i++){
            AsyncWebParameter* p = request->getParam(i);
            if(p->isFile()){
                Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
            } else if(p->isPost()){
                Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            } else {
                Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }

            // TODO - switch based on param name

            // switch (String(p->name().c_str())) {
            //     case "mqtt_broker":
            //         break;
            // }
        }
        request->send(200, "text/plain", "Ok!");
    });

    // Start server
    server.begin();
}
