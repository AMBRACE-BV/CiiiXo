#include <WiFiManager.h>

extern byte MAC[6];

void WiFiEvent(WiFiEvent_t event);
void configModeCallback(WiFiManager* wifiManager);
bool networkIsConnected();
void setupNetwork();
