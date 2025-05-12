#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

// #define EXTERNAL_NUM_INTERRUPTS 46
// #define NUM_DIGITAL_PINS        48
// #define NUM_ANALOG_INPUTS       20

// #define analogInputToDigitalPin(p)  (((p)<20)?(analogChannelToDigitalPin(p)):-1)
// #define digitalPinToInterrupt(p)    (((p)<48)?(p):-1)
// #define digitalPinHasPWM(p)         (p < 46)

static const uint8_t TEST_PIN=32;

// UART

// I2C

// SPI Bus
static const uint8_t SS    = 21; // In reality, this is 26, but the ESP32-S2 with PSRAM crashes when Pin 26 is used
static const uint8_t SD_CS = 21; // In reality, this is 26, but the ESP32-S2 with PSRAM crashes when Pin 26 is used
static const uint8_t MOSI  = 35;
static const uint8_t MISO  = 37;
static const uint8_t SCK   = 36;

// Analog Inputs

// RTC?


// DAC


// Usage-Specific Pins


#endif /* Pins_Arduino_h */
