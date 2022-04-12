#include <EthernetUdp.h>
#include <AsyncUDP.h>
#include "Source.h"

class UdpSource : public Source {
    public:
        void setup();
        void loop();
        void setPort(int);
    protected:
        unsigned int port = 1235;
        AsyncUDP udpClient;
};