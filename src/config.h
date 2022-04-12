struct MqttSourceConfig {
    char* mqtt_broker;
    char* mqtt_username;
    char* mqtt_password;
};

struct UdpSourceConfig {
    int udp_port;
};

struct MqttSinkConfig {
    char* mqtt_broker;
    char* mqtt_username;
    char* mqtt_password;
};

struct UdpSinkConfig {
    int udp_port;
};