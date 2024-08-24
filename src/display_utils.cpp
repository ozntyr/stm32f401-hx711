#include "display_utils.h"
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

// Initialize the ILI9341 display
Adafruit_ILI9341 tft = Adafruit_ILI9341(PB9, PB8); // Adjust the pins as necessary

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
    tft.setCursor(10, 10);

    String factorStr = String(factor, 3); // 3 decimal places
    for (int i = 0; i < factorStr.length(); i++)
    {
        if (i == highlightIdx)
        {
            tft.setTextColor(ILI9341_RED); // Highlight color
        }
        else
        {
            tft.setTextColor(ILI9341_WHITE);
        }
        tft.print(factorStr[i]);
    }
}

void displaySavePrompt()
{
    clearDisplay();
    printText("Do you want to save?", 10, 10, 2, ILI9341_YELLOW);
    printText("Press SELECT to confirm", 10, 40, 2, ILI9341_YELLOW);
}
