#include "I2CConnection.h"

/**
 @brief Write I2C
 @param [in] address register address
 @param [in] data write data
*/
I2CConnection::I2CConnection(uint8_t i2c_address)
    : _i2c_address { i2c_address } {
        // Wire.begin();
        // Serial.begin(115200);
    };

void I2CConnection::writeI2C(uint8_t register_address, uint8_t data)
{
    // Serial.print("Writing from device: ");
    // Serial.println(_i2c_address,HEX);
    // Serial.print("On the address: ");
    // Serial.println(register_address,HEX);
    // Serial.println(data,HEX);
    Wire.beginTransmission(_i2c_address);
    Wire.write(register_address);
    Wire.write(data);
    Wire.endTransmission();
}

void I2CConnection::writeI2C(uint8_t register_address, const uint8_t* data, int size)
{
    // Serial.print("Writing from device: ");
    // Serial.println(_i2c_address,HEX);
    // Serial.print("On the address: ");
    // Serial.println(register_address,HEX);
    Wire.beginTransmission(_i2c_address);
    Wire.write(register_address);
    Wire.write(data, (size_t)size);
    Wire.endTransmission();
}

/**
 @brief Read I2C
 @param [in] address register address
 @param [in] num read length
 @param [out] data read data
*/
void I2CConnection::readI2C(uint8_t register_address, uint8_t number_of_bytes, uint8_t* data)
{
    // Serial.print("Reading from device: ");
    // Serial.println(_i2c_address,HEX);
    // Serial.print("On the address: ");
    // Serial.println(register_address,HEX);
    Wire.beginTransmission(_i2c_address);
    Wire.write(register_address);
    Wire.endTransmission();
    uint8_t index = 0;
    Wire.requestFrom(_i2c_address, number_of_bytes);
    while (Wire.available()) {
        data[index++] = Wire.read();
    }
}
