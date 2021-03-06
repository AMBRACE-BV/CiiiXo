#ifndef SOURCE_H
#define SOURCE_H

#include <Arduino.h>

void setupSources();
void loopSources();
void setMqttSourceEnabled(bool);
void setUdpSourceEnabled(bool);

class Source {
    public:
        virtual void setup(){Serial.print("base source setup");};
        virtual void loop(){Serial.print("base source loop");};
        void setEnabled(bool enabled) {
            this->enabled = enabled;
        }
        bool isEnabled() {
            return this->enabled;
        }

        bool enabled = false;
};

#endif