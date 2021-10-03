#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12
#include <ETH.h>
#include <WiFiManager.h>
#include <FastLED.h>
#include <Ethernet.h>


#include "Leds.h"

/******************FIXED IP ADDRESS********/
// IPAddress local_IP(192, 168, 1, 27);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);
// IPAddress primaryDNS(8, 8, 8, 8); //optional
// IPAddress secondaryDNS(8, 8, 4, 4); //optional

static bool eth_connected = false;
WiFiManager wifiManager;


void WiFiEvent(WiFiEvent_t event)
{
    switch (event) {
    case SYSTEM_EVENT_ETH_START:
        Serial.println("ETH Started");
        //set eth hostname here
        ETH.setHostname("esp32-ethernet");
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        Serial.println("ETH Connected");
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:

        Serial.print("ETH connected! MAC: ");
        Serial.print(ETH.macAddress());
        Serial.print(", IPv4: ");
        Serial.print(ETH.localIP());
        if (ETH.fullDuplex()) {
            Serial.print(", FULL_DUPLEX");
        }
        Serial.print(", ");
        Serial.print(ETH.linkSpeed());
        Serial.println("Mbps");
        eth_connected = true;
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        Serial.println("ETH Disconnected");
        eth_connected = false;
        break;
    case SYSTEM_EVENT_ETH_STOP:
        Serial.println("ETH Stopped");
        eth_connected = false;
        break;
    default:
        break;
    }
}

void configModeCallback(WiFiManager* wifiManager)
{
    Serial.println("Entered config mode, setup an access point");
    Serial.println(WiFi.softAPIP());
    Serial.println(wifiManager->getConfigPortalSSID());
    flashLed(2, CRGB::Orange);
}

bool networkIsConnected() {
    return eth_connected;
}

void setupNetwork()
{
    // Setup an Access Point if wifi is not yet configured.
    // We start by connecting to a WiFi network
    // wifiManager.resetSettings();


    WiFi.onEvent(WiFiEvent);
    delay(1000);
    ETH.begin();
    // Ethernet.begin(mac,);
    delay(1000);

    // Don't use Wifi yet
    // Serial.println("Current wifi credentials");
    // Serial.println(wifiManager.getSSID());
    // Serial.println(wifiManager.getPassword());
    // wifiManager.setAPCallback(configModeCallback);
    // TODO: Do not set a AP password for now, but later on we should?
    // wifiManager.autoConnect("CIIIXO");
}