#include <EthernetUdp.h>
#include <AsyncUDP.h>

class UdpSource {
    public:
        void setup();
        void loop();
    protected:
        unsigned int port = 1234;
        AsyncUDP udpClient;
};