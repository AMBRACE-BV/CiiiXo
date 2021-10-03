#ifndef SINK_H
#define SINK_H

#include <Arduino.h>

void setupMqtt();
void setupUdp();
void disableMqtt();
void disableUdp();
void loopSinks();
void sendMessage(String);
void sendInputsData(uint8_t, uint8_t);

class Sink {
    public:
        virtual void setup(){Serial.print("base setup");};
        virtual void loop(){Serial.print("base loop");};
        virtual void sendMessage(String){Serial.print("base msg");};
        virtual void sendInputsData(uint8_t, uint8_t){Serial.print("base inputs");};
};

#endif