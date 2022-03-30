#ifndef SINK_H
#define SINK_H

#include <Arduino.h>

void setupSinks();
void loopSinks();
void setMqttSinkEnabled(bool);
void setUdpSinkEnabled(bool);
void sendMessage(String);
void sendInputsData(uint8_t, uint8_t);

class Sink {
    public:
        virtual void setup(){Serial.print("base sink setup");};
        virtual void loop(){Serial.print("base sink loop");};
        virtual void sendMessage(String){Serial.print("base msg");};
        virtual void sendInputsData(uint8_t, uint8_t){Serial.print("base inputs");};

        void setEnabled(bool enabled) {
            this->enabled = enabled;
        }
        bool isEnabled() {
            return enabled;
        }

        bool enabled = false;
};

#endif