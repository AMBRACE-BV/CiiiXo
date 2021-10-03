#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>

class I2CConnection {
public:
    I2CConnection(uint8_t i2c_address);
    void writeI2C(uint8_t register_address, uint8_t data);
    void writeI2C(uint8_t register_address, const uint8_t* data, int size);
    void readI2C(uint8_t register_address, uint8_t num, uint8_t* data);

private:
    uint8_t _i2c_address;
};
