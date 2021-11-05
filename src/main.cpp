#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>

#define FASTLED_ESP32_I2S
#include <FastLED.h>
#include <FreeRTOS.h>

#define MQTT_MAX_PACKET_SIZE 256

#include "HttpServer.h"
#include "IOManager.h"
#include "Leds.h"
#include "Network.h"
#include "sinks/Sink.h"
#include "sources/UdpSource.h"

// #define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12
#define CONFIG_ESP_INT_WDT_TIMEOUT_MS 500

#define DEBUG

/*---Prototypes----*/
// void callback_MQTT(char* topic, byte* payload, unsigned int length);
/// void IRAM_ATTR handleInterrupt();
void handleInterrupt();
void printHighPins();

// void IRAM_ATTR handleInterrupt();
QueueHandle_t job_queue;

UdpSource udpSource;

struct PinChange{
    uint8_t pin;
    uint8_t value;
};


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

TaskHandle_t main_task;
TaskHandle_t job_task;

// Running on core 1 (default core)
void mainTask(void* parameters) {
    for (;;) {
        if (ioChangeDetected()) {
            flashLed(2, CRGB::Green, 50);
            Serial.println("io change!");

            int8_t data[getNumberOfInputs() * MODULE_SIZE];
            ioChanges(data);
            for (uint8_t i = 0; i < getNumberOfInputs() * MODULE_SIZE; i++) {
                // data == -1 --> current state = 0; change == 1 --> current state = 1
                if (data[i] != 0) {
                    PinChange change;
                    change.pin = i;
                    change.value = (data[i] + 1) / 2;

                    xQueueSend(job_queue, &change, portMAX_DELAY);
#ifdef DEBUG
                    Serial.print("Change detected on pin ");
                    Serial.print(i);
                    Serial.print(" new value ");
                    Serial.println(change.value);
#endif
                }
            }
        }
    }
}

// Running on core 0
void jobTask(void* parameters) {
    for (;;) {
        loopSinks();
        udpSource.loop();

        // Poll from queue to fetch interrupt pins
        PinChange change;
        if (xQueueReceive(job_queue, &change, 20)) {            
            sendInputsData(change.pin, change.value);
        }

        EVERY_N_MILLISECONDS(500) {
            if (networkIsConnected()) {
                setLed(2, CRGB::Blue);
            } else {
                setLed(2, CRGB::Red);
            }
        }
    }
}

// Split tasks on the two cores - note: esp32 by default uses core 1
void setupTasks() {
    job_queue = xQueueCreate(40, sizeof(struct PinChange*));
    xTaskCreatePinnedToCore(
        jobTask,   /* Task function. */
        "Job",     /* name of task. */
        10000,     /* Stack size of task */
        NULL,      /* parameter of the task */
        1,         /* priority of the task */
        &job_task, /* Task handle to keep track of created task */
        0);        /* pin task to core 0 */

    // Main tasks runs on core 1, could thus also be implemented in the loop() function
    xTaskCreatePinnedToCore(
        mainTask,   /* Task function. */
        "Main",     /* name of task. */
        10000,      /* Stack size of task */
        NULL,       /* parameter of the task */
        1,          /* priority of the task */
        &main_task, /* Task handle to keep track of created task */
        1);         /* pin task to core 0 */
}

void setupDebug() {
    esp_log_level_set("*", ESP_LOG_ERROR);     // set all components to ERROR level
    esp_log_level_set("wifi", ESP_LOG_WARN);   // enable WARN logs from WiFi stack
    esp_log_level_set("dhcpc", ESP_LOG_INFO);  // enable INFO logs from DHCP client
}

void setupESP() {
    pinMode(PIN_INT_PCA9698, INPUT);
    pinMode(PIN_RESET, INPUT);
    Wire.begin(33, 32, 400000);
    Serial.begin(115200);
    delay(1000);
}

void setupIOExpanders() {
    // Start IO connectors
    configureIO();
    beginIO();

#ifdef DEBUG
    Serial.println("PCA should be interrupting");
    Serial.println("initial IO: ");
    printHighPins();
#endif
}

void setup() {
    // Set log levels
    // setupDebug();
    // ESP configuration
    setupESP();
    // Leds
    setupLeds();
    // Wifi configuration
    setupNetwork();
    // Setup IO expanders
    setupIOExpanders();
    // Launching webservice
    setupWebservice();
    // Launching MQTT publisher
    // setupMqtt();
    // Launch UDP publisher
    setupUdp();
    // Split tasks on dual core
    setupTasks();
}

// Empty loop due to tasks ran on multi-core
void loop() {}