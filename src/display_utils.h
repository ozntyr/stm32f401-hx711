#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

// Define screen dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Create an instance of the ILI9341 display
extern Adafruit_ILI9341 tft;

// Function prototypes
void initDisplay();
void clearDisplay();
void printText(const char *text, int x, int y, int textSize, uint16_t color);
void displayCalibrationFactor(float factor);
void displayCalibrationFactor(float factor, int highlightIdx);
void displayWeight(float weight);

#endif // DISPLAY_UTILS_H
