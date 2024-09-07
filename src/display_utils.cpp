#include "display_utils.h"
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include "pins.h"

// Initialize the ILI9341 display
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void initDisplay()
{
    tft.begin();
    tft.setRotation(3); // Adjust the rotation as needed
    clearDisplay();
}

void clearDisplay()
{
    tft.fillScreen(ILI9341_BLACK);
}

void printText(const char *text, int x, int y, int textSize, uint16_t color)
{
    tft.setTextSize(textSize);
    tft.setTextColor(color);
    tft.setCursor(x, y);
    tft.print(text);
}

void displayCalibrationFactor(float factor)
{
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Cal Factor: %.3f", factor);
    clearDisplay();
    printText(buffer, 10, 10, 2, ILI9341_WHITE);
}

void displayCalibrationFactor(float factor, int highlightIdx)
{
    clearDisplay(); // Clear screen
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);

    // Format factor with leading zeros to 8 digits before decimal and 3 decimal places
    char buffer[14];
    snprintf(buffer, sizeof(buffer), "%08.3f", factor);

    // Display the formatted factor with highlighting
    int x = 10; // Starting x position
    int y = 10; // y position
    int bufferLength = strlen(buffer);
    int digitIndex = 0; // To track the index of digits for highlighting

    for (int i = 0; i < bufferLength; i++)
    {
        if (isdigit(buffer[i])) // Check if the current character is a digit
        {
            // Determine the color based on the highlight index
            if (digitIndex == highlightIdx)
            {
                tft.setTextColor(ILI9341_RED); // Highlight color
            }
            else
            {
                tft.setTextColor(ILI9341_WHITE);
            }
            tft.setCursor(x, y);
            tft.print(buffer[i]);
            digitIndex++; // Move to the next digit index
        }
        else
        {
            // Set color for non-digit characters
            tft.setTextColor(ILI9341_WHITE);
            tft.setCursor(x, y);
            tft.print(buffer[i]);
        }
        x += 12; // Move cursor to the right for the next character (adjust as necessary)
    }
}

void displayWeight(float weight)
{
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Weight: %.3f kg", weight); // Format to 3 decimal places
    clearDisplay();
    printText(buffer, 10, 10, 2, ILI9341_WHITE);
}