#include <Arduino.h>
#include <Wire.h>

#include "defines.h"
#include "IOManager.h"


using namespace std;

uint8_t nr_inputs, nr_outputs = 0;

CiiiXo* input_modules[MAX_INPUT_MODULES];
CiiiXo* output_modules[MAX_OUTPUT_MODULES];

QueueHandle_t interrupt_queue = xQueueCreate(80, sizeof(boolean));
QueueHandle_t output_queue = xQueueCreate(80, sizeof(OutputChange));

// Keep track of pin history - gives us the ability to detect changes
uint8_t pin_history[MODULE_SIZE * MAX_INPUT_MODULES];

// Note: avoid Serial prints during interrupt - will cause Gure meditation error
void handleInterrupt() {
    bool interrupted = true;
    xQueueSend(interrupt_queue, &interrupted, 0);
}

void configureIO() {
    bool active;

    // Inputs
    for (uint8_t address = 0x20; address < 0x22 && nr_inputs < MAX_INPUT_MODULES; address++) {
        active = detectDevice(address);
        if (active) {
            input_modules[nr_inputs] = new CiiiXo(address, PIN_INT_PCA9698, handleInterrupt);
            nr_inputs++;
        }
    }
    // Outputs - note, ordened based on address
    for (uint8_t address = 0x22; address < 0x30 && nr_outputs < MAX_OUTPUT_MODULES; address++) {
        active = detectDevice(address);
        if (active) {
            output_modules[nr_outputs] = new CiiiXo(address, PIN_INT_PCA9698, handleInterrupt);
            nr_outputs++;
        }
    }
}

void beginIO() {
    for (int i = 0; i < nr_inputs; i++) {
        input_modules[i]->begin(true);
    }
    for (int i = 0; i < nr_outputs; i++) {
        output_modules[i]->begin(false);
    }

    // Fill initial history
    readAll(pin_history);
}

/* Set pin value*/
void setPin(uint8_t pin, uint8_t value) {
    #ifdef LOCAL_DEBUG
    Serial.print("setting pin ");
    Serial.print(pin);
    Serial.print(" to value ");
    Serial.println(value);
    #endif

    uint8_t module = pin / MODULE_SIZE;
    uint8_t module_pin = pin % MODULE_SIZE;

    if (module >= nr_outputs) {
        Serial.print("trying to set pin on non-existing module: ");
        Serial.println(module);
        return;
    }

    output_modules[module]->digitalWrite(module_pin, value);

    // Notify change occurred
    OutputChange change;
    change.pin = pin;
    change.value = value;
    xQueueSend(output_queue, &change, 0);
}

/* Set pin value for a defined duration in ms*/
void setPin(uint8_t pin, uint8_t value, uint32_t duration) {
    #ifdef LOCAL_DEBUG
    Serial.print("setting pin ");
    Serial.print(pin);
    Serial.print(" to value ");
    Serial.print(value);
    Serial.print(" for ");
    Serial.print(duration);
    Serial.println(" ms");
    #endif DEBUG
    // TODO ! 
    Serial.println("DURATION NOT IMPLEMENTED YET - ignoring message!");
}

uint8_t readPin(uint8_t pin) {
    return readPin(pin, INPUT_MODULE);
}

uint8_t readPin(uint8_t pin, IOType io_type) {
    uint8_t module = pin / MODULE_SIZE;
    uint8_t module_pin = pin % MODULE_SIZE;

    // Predefine value
    uint8_t value;

    if (io_type == INPUT_MODULE) {
        if (module >= nr_inputs) {
            Serial.print("trying to read pin on a non-existing input module: ");
            Serial.println(module);
            return 0;
        }
        value = input_modules[module]->digitalRead(module_pin);
        return value;
    } else {
        if (module >= nr_outputs) {
            Serial.print("trying to read pin on a non-existing output module: ");
            Serial.println(module);
            return 0;
        }
        value = output_modules[module]->digitalRead(module_pin);
        return value;
    }
}

void readAll(uint8_t data[MODULE_SIZE * MAX_INPUT_MODULES]) {
    readAll(data, INPUT_MODULE);
}

void readAll(uint8_t data[MODULE_SIZE * MAX_INPUT_MODULES], IOType io_type) {
    uint8_t temp[MODULE_SIZE * MAX_INPUT_MODULES];
    for (int mod = 0; mod < MAX_INPUT_MODULES; mod++) {
        input_modules[mod]->digitalReadAll(temp);
        for (int i = 0; i < MODULE_SIZE; i++) {
            data[mod * MODULE_SIZE + i] = temp[i];
        }
    }
}

bool inputChangeDetected() {
    bool interrupted = false;
    xQueueReceive(interrupt_queue, &interrupted, 0);
    return interrupted;
}

OutputChange outputChangeDetected() {
    OutputChange change;
    bool received = xQueueReceive(output_queue, &change, 0);
    change.received = received;
    return change;
}

/* Fills data with -1, 0 or 1 ; indicating the change compared to the previous state*/
void inputChanges(int8_t data[MAX_INPUT_MODULES * MODULE_SIZE]) {
    uint8_t temp[MODULE_SIZE * MAX_INPUT_MODULES];
    readAll(temp, INPUT_MODULE);
    for (uint8_t i = 0; i < MAX_INPUT_MODULES * MODULE_SIZE; i++) {
        data[i] = temp[i] - pin_history[i];
        pin_history[i] = pin_history[i] + data[i];  // Update history
    }
}

bool detectDevice(uint8_t address) {
    byte error;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address < 16)
            Serial.print("0");
        Serial.print(address, HEX);
        Serial.println("  !");
        return true;
    } else if (error == 4) {
        Serial.print("Unknown error at address 0x");
        if (address < 16)
            Serial.print("0");
        Serial.println(address, HEX);
    }
    return false;
}

uint8_t getNumberOfInputs() {
    return nr_inputs;
}
uint8_t getNumberOfOutputs() {
    return nr_outputs;
}

void printHighPins() {
    uint8_t data[getNumberOfInputs() * MODULE_SIZE];
    readAll(data);

    Serial.println("Active on:");
    Serial.println(" | 0 1 2 3 4 5 6 7 8 9");
    Serial.println("----------------------");
    for (int indexy = 0; indexy < getNumberOfInputs() * MODULE_SIZE / 10; indexy++) {
        Serial.print(indexy);
        Serial.print("|");
        for (int index = 0; index < 10; index++) {
            if (data[(10 * indexy) + index] == LOW) {
                Serial.print("0 ");
            } else {
                Serial.print("1 ");
            }
        }
        Serial.println();
    }
}