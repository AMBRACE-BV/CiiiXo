#include <CiiiXo.h>

/******************DEFINE*******************/
#define PIN_RESET 37

/******************PCA9698*******************/
#define PIN_INT_PCA9698 35

#define ADDR_PCA9698_1 0x20
#define ADDR_PCA9698_2 0x21

const uint8_t MODULE_SIZE = 40;

/********************************************/

const uint8_t MAX_INPUT_MODULES = 2;
const uint8_t MAX_OUTPUT_MODULES = 4;

enum IOType { INPUT_MODULE,
              OUTPUT_MODULE };

void configureIO();
void beginIO();
void setPin(uint8_t pin, uint8_t value);
uint8_t readPin(int pin);
uint8_t readPin(uint8_t pin, IOType io_type);  // Ability to read output pins
void readAll(uint8_t data[MODULE_SIZE * INPUT_MODULE]);
void readAll(uint8_t data[MODULE_SIZE * INPUT_MODULE], IOType io_type);  // Ability to read output pins
bool ioChangeDetected();
void ioChanges(int8_t data[MODULE_SIZE * INPUT_MODULE]);
bool detectDevice(uint8_t address);
uint8_t getNumberOfInputs();
uint8_t getNumberOfOuputs();