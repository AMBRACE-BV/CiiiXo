/**
 @brief This is a library for the PCA9698 IO Expander with interrupts enabled.
 @author AMBRACE<info@ambrace.eu>
*/

#include "I2CConnection.h"
#include <Arduino.h>
#include <Wire.h>

//#define DEBUG

#define PCA9698_SLAVE_ADDRESS 0x20 ///< Default I2C Slave Address

/// @name Register Addresses
/// @{
#define PCA9698_OUTPUT_REG 0x01
#define PCA9698_CONFIGURATION_REG 0x03
#define PCA9698_INPUT_PORT0 0x00
#define PCA9698_INPUT_PORT1 0x01
#define PCA9698_INPUT_PORT2 0x02
#define PCA9698_INPUT_PORT3 0x03
#define PCA9698_INPUT_PORT4 0x04
#define PCA9698_INPUT_PORT0_AUTO_INCREMENT 0x80
#define PCA9698_OUTPUT_PORT0 0x08
#define PCA9698_OUTPUT_PORT1 0x09
#define PCA9698_OUTPUT_PORT2 0x0A
#define PCA9698_OUTPUT_PORT3 0x0B
#define PCA9698_OUTPUT_PORT4 0x0C
#define PCA9698_CONFIG_PORT0 0x18
#define PCA9698_CONFIG_PORT1 0x19
#define PCA9698_CONFIG_PORT2 0x1A
#define PCA9698_CONFIG_PORT3 0x1B
#define PCA9698_CONFIG_PORT4 0x1C
#define PCA9698_CONFIG_PORT0_AUTO_INCREMENT 0x98
#define PCA9698_MASK_INTERUPT_PORT0 0x20
#define PCA9698_MASK_INTERUPT_PORT1 0x21
#define PCA9698_MASK_INTERUPT_PORT2 0x22
#define PCA9698_MASK_INTERUPT_PORT3 0x23
#define PCA9698_MASK_INTERUPT_PORT4 0x24
#define PCA9698_MASK_INTERUPT_PORT0_AUTO_INCREMENT 0xa0
#define PCA9698_MODE_REGISTER 0x2A

/// @}

/// @name OUTPUT Parameter
/// @{
#define PORT1
/// @}

/**
 @class PCA9698
 @brief GPIO I2C Controll class
*/
class CiiiXo {
public:
    // PCA9698(uint8_t i2c_address);
    CiiiXo(uint8_t i2c_address, uint8_t interrupt_pin, void (*interrupt_function)());
    void digitalWrite(uint8_t pin, uint8_t output);
    int digitalRead(uint8_t pin);
    void digitalReadAll(uint8_t data[40]);
    // void highPins(uint8_t pin[]);
    void serialPrintRegister(uint8_t register_address, uint8_t number_of_bytes);
    void setAllClear();
    void begin(bool);
    void portMode(uint8_t port, uint8_t mode);

private:
    uint8_t _i2c_address;
    I2CConnection _i2c_connection;
    bool _using_interrupt = true;
    uint8_t _interrupt_pin = 35;
    void (*_interrupt_function)() {};
    uint8_t _output_port0;
    uint8_t _output_port1;
    uint8_t _output_port2;
    uint8_t _output_port3;
    uint8_t _output_port4;
    void configureInput(void);
    void configureOutput(void);
};
