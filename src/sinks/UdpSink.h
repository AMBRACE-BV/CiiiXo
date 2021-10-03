#include <EthernetUdp.h>
#include <AsyncUDP.h>
#include "Sink.h"

class UdpSink: public Sink {
    public:
        void setup();
        void loop();
        void sendMessage(String);
        void sendInputsData(uint8_t, uint8_t);
    // CHANGE TO PROTECTED
    protected:
        IPAddress remoteIp = IPAddress(192, 168, 1, 1);
        unsigned int port = 1234;
        AsyncUDP udpClient;
        void setRemote(IPAddress, int);
        bool broadcast = true;
};