/**
 @author AMBRACE<info@ambrace.eu>
*/

#include "CiiiXo.h"

/**
 @brief Constructor
*/
CiiiXo::CiiiXo(uint8_t i2c_address, uint8_t interrupt_pin, void (*interrupt_function)())
    : _i2c_address { i2c_address }
    , _i2c_connection { i2c_address }
    , _using_interrupt { true }
    , _interrupt_pin { interrupt_pin }
    , _interrupt_function { interrupt_function }
    , _output_port0 { 0x00 }
    , _output_port1 { 0x00 }
    , _output_port2 { 0x00 }
    , _output_port3 { 0x00 }
    , _output_port4 { 0x00 }
{

    Serial.println("Construction of PCA");
}

/**
 @brief Configure Device
*/
void CiiiXo::configureInput()
{

    // TODO - GURU MEDITATION ERROR 

    Serial.println("Configure input");
    const uint8_t all_high[5] = { 0xff, 0xff, 0xff, 0xff, 0xff };
    const uint8_t all_low[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
    // Clear
    _i2c_connection.writeI2C(PCA9698_CONFIG_PORT0_AUTO_INCREMENT, all_high, 5);
// _i2c_connection.writeI2C(PCA9698_CONFIG_PORT0, 0x00);
// _i2c_connection.writeI2C(PCA9698_CONFIG_PORT1, 0x00);
// _i2c_connection.writeI2C(PCA9698_CONFIG_PORT2, 0x00);
// _i2c_connection.writeI2C(PCA9698_CONFIG_PORT3, 0x00);
// _i2c_connection.writeI2C(PCA9698_CONFIG_PORT4, 0x00);
#ifdef DEBUG
    Serial.println("IO Configuration register values: ");
    serialPrintRegister(PCA9698_CONFIG_PORT0, 5);
#endif
    // Remove all interupt masks
    _i2c_connection.writeI2C(PCA9698_MASK_INTERUPT_PORT0_AUTO_INCREMENT, all_low, 5);
// _i2c_connection.writeI2C(PCA9698_MASK_INTERUPT_PORT0, 0x00);
// _i2c_connection.writeI2C(PCA9698_MASK_INTERUPT_PORT1, 0x00);
// _i2c_connection.writeI2C(PCA9698_MASK_INTERUPT_PORT2, 0x00);
// _i2c_connection.writeI2C(PCA9698_MASK_INTERUPT_PORT3, 0x00);
// _i2c_connection.writeI2C(PCA9698_MASK_INTERUPT_PORT4, 0x00);
#ifdef DEBUG
    Serial.println("Mask register values: ");
    // serialPrintRegister(PCA9698_MASK_INTERUPT_PORT0, 5);
    serialPrintRegister(PCA9698_MASK_INTERUPT_PORT0, 1);
    serialPrintRegister(PCA9698_MASK_INTERUPT_PORT1, 1);
    serialPrintRegister(PCA9698_MASK_INTERUPT_PORT2, 1);
    serialPrintRegister(PCA9698_MASK_INTERUPT_PORT3, 1);
    serialPrintRegister(PCA9698_MASK_INTERUPT_PORT4, 1);
#endif
    // Set the mode register for interupting
    _i2c_connection.writeI2C(PCA9698_MODE_REGISTER, 0x12);
#ifdef DEBUG
    Serial.println("Mode register values: ");
    serialPrintRegister(PCA9698_MODE_REGISTER, 1); //Should be 0x12
#endif

    Serial.println("finished configuring input");

}

/**
 @brief Configure Device
*/
void CiiiXo::configureOutput()
{
    Serial.println("Configure output");
    // const uint8_t all_high[5] = { 0xff, 0xff, 0xff, 0xff, 0xff };
    const uint8_t all_low[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
    // Clear
    _i2c_connection.writeI2C(PCA9698_CONFIG_PORT0_AUTO_INCREMENT, all_low, 5);
}

void CiiiXo::serialPrintRegister(uint8_t register_address, uint8_t number_of_bytes)
{
    byte data[number_of_bytes];
    _i2c_connection.readI2C(register_address, number_of_bytes, data);
    for (int index = 0; index < number_of_bytes; index++) {
        Serial.print(data[index], HEX);
        Serial.print("\n");
    }
}

/**
 @brief write GPIO
 @param [in] pin pinNo
 @param [in] output output
*/
void CiiiXo::digitalWrite(uint8_t pin, uint8_t output)
{
    if (8 > pin && pin >= 0) {
        if (output == HIGH) {
            _output_port0 |= 1 << pin;
        } else if (output == LOW) {
            _output_port0 &= ~(1 << pin);
        }
        _i2c_connection.writeI2C(PCA9698_OUTPUT_PORT0, _output_port0);
    } else if (16 > pin && pin >= 8) {
        if (output == HIGH) {
            _output_port1 |= 1 << (pin - 8);
        } else if (output == LOW) {
            _output_port1 &= ~(1 << (pin - 8));
        }
        _i2c_connection.writeI2C(PCA9698_OUTPUT_PORT1, _output_port1);
    } else if (24 > pin && pin >= 16) {
        if (output == HIGH) {
            _output_port2 |= 1 << (pin - 16);
        } else if (output == LOW) {
            _output_port2 &= ~(1 << (pin - 16));
        }
        _i2c_connection.writeI2C(PCA9698_OUTPUT_PORT2, _output_port2);
    } else if (32 > pin && pin >= 24) {
        if (output == HIGH) {
            _output_port3 |= 1 << (pin - 24);
        } else if (output == LOW) {
            _output_port3 &= ~(1 << (pin - 24));
        }
        _i2c_connection.writeI2C(PCA9698_OUTPUT_PORT3, _output_port3);
    } else if (40 > pin && pin >= 32) {
        if (output == HIGH) {
            _output_port4 |= 1 << (pin - 32);
        } else if (output == LOW) {
            _output_port4 &= ~(1 << (pin - 32));
        }
        _i2c_connection.writeI2C(PCA9698_OUTPUT_PORT4, _output_port4);
    }
}

/**
 @brief read GPIO
 @param [in] pin pinNo
*/
int CiiiXo::digitalRead(uint8_t pin)
{
    if (8 > pin && pin >= 0) {
        byte data[1];
        _i2c_connection.readI2C(PCA9698_INPUT_PORT0, sizeof(data), data);
        if ((data[0] & (1 << pin)) == (1 << pin)) {
            return HIGH;
        } else {
            return LOW;
        }
    } else if (16 > pin && pin >= 8) {
        byte data[1];
        _i2c_connection.readI2C(PCA9698_INPUT_PORT1, sizeof(data), data);
        if ((data[0] & (1 << pin - 8)) == (1 << pin - 8)) {
            return HIGH;
        } else {
            return LOW;
        }

    } else if (24 > pin && pin >= 16) {
        byte data[1];
        _i2c_connection.readI2C(PCA9698_INPUT_PORT2, sizeof(data), data);
        if ((data[0] & (1 << pin - 16)) == (1 << pin - 16)) {
            return HIGH;
        } else {
            return LOW;
        }
    } else if (32 > pin && pin >= 24) {
        byte data[1];
        _i2c_connection.readI2C(PCA9698_INPUT_PORT3, sizeof(data), data);
        if ((data[0] & (1 << pin - 24)) == (1 << pin - 24)) {
            return HIGH;
        } else {
            return LOW;
        }
    } else if (40 > pin && pin >= 32) {
        byte data[1];
        _i2c_connection.readI2C(PCA9698_INPUT_PORT4, sizeof(data), data);
        if ((data[0] & (1 << pin - 32)) == (1 << pin - 32)) {
            return HIGH;
        } else {
            return LOW;
        }
    }

    return LOW;
}

void CiiiXo::digitalReadAll(uint8_t data[40])
{
    uint8_t data_in_bytes[5];

    _i2c_connection.readI2C(PCA9698_INPUT_PORT0_AUTO_INCREMENT, sizeof(data_in_bytes), data_in_bytes);

#ifdef DEBUG
    for (int index = 0; index < 5; index++) {
        Serial.println(data_in_bytes[index], BIN);
    }
#endif

    for (int index = 0; index < 40; index++) {
        // Create a mask for the position of this bit
        unsigned char mask = (1 << (index % 8));

        data[index] = (mask & data_in_bytes[index / 8]) == mask ? LOW : HIGH;
    }
}

/**
 * wake up i2c controller
 */
void CiiiXo::begin(bool input)
{
    if (input) {
        configureInput();
        // TODO - check if interrupt occurs twice?
        ::pinMode(_interrupt_pin, INPUT);
        attachInterrupt(35, (*_interrupt_function), FALLING);
    } else {
        configureOutput();
    }
}
