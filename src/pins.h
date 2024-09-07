// pins.h
#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = PA0;
const int LOADCELL_SCK_PIN = PA1;

// Button pins
const int BUTTON_SELECT_PIN = PA2;
const int BUTTON_UP_PIN = PA3;
const int BUTTON_DOWN_PIN = PA4;

// Buzzer pin
const int BUZZER_PIN = PB10;

// Define the pins connected to the display
#define TFT_SCLK PA5 // SPI SCLK pin
#define TFT_MISO PA6 // SPI MISO pin
#define TFT_MOSI PA7 // SPI MOSI pin
#define TFT_DC PB0   // Data/Command pin
#define TFT_RST PB1  // Reset pin (optional, can be connected to a GPIO)
#define TFT_CS PB2   // Chip Select pin

#endif // PINS_H
