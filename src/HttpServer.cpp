#include "HttpServer.h"
#include <WebServer.h>
#include <ESPmDNS.h>


/******************Web server*******************/
WebServer server(80);

// HTTP handlers
void handleRoot() {
    server.send(200, "text/plain", "Hello from CiiiXo!\n");
}

void handleNotFound() {
    server.send(404, "text/plain", String("No ") + server.uri() + " here!\n");
}

void handleSourceConfig() {
    server.send(200, "text/plain", "Source config - TODO!");
}

void handleSinkConfig() {
    server.send(200, "text/plain", "Sink config - TODO!");
}

void setupWebservice()
{
    MDNS.begin("CiiiXo");

    // Bind HTTP handler
    server.on("/", handleRoot);
    server.on("/config/sources", handleSourceConfig);
    server.on("/config/sinks", handleSinkConfig);
    server.onNotFound(handleNotFound);

    // Start the Ethernet web server
    server.begin();
    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
}
