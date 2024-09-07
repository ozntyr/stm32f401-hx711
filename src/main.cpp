#include <Arduino.h>
#include <HX711.h>
#include "eeprom_utils.h"
#include "display_utils.h"
#include "pins.h"

// Buzzer durations
const int SHORT_BUZZ_DURATION = 100; // Duration for a short buzz (100ms)
const int LONG_BUZZ_DURATION = 500;  // Duration for a long buzz (500ms)

// Default values for settings
#define DEFAULT_CALIBRATION_FACTOR 2280.0f
#define SENSOR_READ_COUNT 10
#define DISPLAY_DECIMAL 3
#define LOOP_DELAY 10
#define READ_DELAY 500
#define CALIBRATION_MODE_DURATION 2000 // Duration for calibration mode activation
#define SAVE_PROMPT_DURATION 2000      // Duration for save prompt display
#define DEBOUNCE_DELAY 50              // Debounce delay in milliseconds
#define DEBOUNCE_COUNTER_MAX 3

HX711 scale;

float calibration_factor = DEFAULT_CALIBRATION_FACTOR;

bool calibrationMode = false;
int currentDigitIdx = 0;
float newCalibrationFactor = DEFAULT_CALIBRATION_FACTOR;
unsigned long buttonPressStartTime = 0;

static bool lastSelectButtonState = HIGH;
static bool lastUpButtonState = HIGH;
static bool lastDownButtonState = HIGH;

unsigned long lastReadTime = 0; // The last time the weight was read

unsigned long lastDebounceTimeSelect = 0; // The last time the SELECT button state was checked
unsigned long lastDebounceTimeUp = 0;     // The last time the UP button state was checked
unsigned long lastDebounceTimeDown = 0;   // The last time the DOWN button state was checked

int debounceCounterSelect = 0;
int debounceCounterUp = 0;
int debounceCounterDown = 0;

bool debounceButtonStateSelect = HIGH;
bool debounceButtonStateUp = HIGH;
bool debounceButtonStateDown = HIGH;

void buzz(int duration)
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void handleCalibrationMode()
{
  bool upButtonState = digitalRead(BUTTON_UP_PIN);
  bool downButtonState = digitalRead(BUTTON_DOWN_PIN);
  bool selectButtonState = digitalRead(BUTTON_SELECT_PIN);

  unsigned long currentTime = millis();

  // Debounce logic for UP button
  if (upButtonState != debounceButtonStateUp)
  {
    lastDebounceTimeUp = currentTime;
    debounceCounterUp = 0;
  }

  debounceCounterUp++;

  if ((currentTime - lastDebounceTimeUp) > DEBOUNCE_DELAY && debounceCounterUp > DEBOUNCE_COUNTER_MAX)
  {
    if (upButtonState != lastUpButtonState)
    {
      if (upButtonState == LOW)
      {
        // Increment the current digit
        int tempVal = int(newCalibrationFactor * 1000);             // Multiply by 1000 for 3 decimal places
        int digit = (int)(tempVal / pow(10, currentDigitIdx)) % 10; // Isolate the digit
        int newDigit = (digit + 1) % 10;                            // Increase the digit
        tempVal -= (digit - newDigit) * pow(10, currentDigitIdx);   // Replace the digit
        newCalibrationFactor = float(tempVal) / 1000.0;             // Divide by 1000 to restore precision
        Serial.print("New calibration factor: ");
        Serial.println(newCalibrationFactor, 3);
        displayCalibrationFactor(newCalibrationFactor, currentDigitIdx); // Update display
        buzz(SHORT_BUZZ_DURATION);                                       // Short buzz for button press
      }
      lastUpButtonState = upButtonState;
    }
  }

  // Debounce logic for DOWN button
  if (downButtonState != debounceButtonStateDown)
  {
    lastDebounceTimeDown = currentTime;
    debounceCounterDown = 0;
  }

  debounceCounterDown++;

  if ((currentTime - lastDebounceTimeDown) > DEBOUNCE_DELAY && debounceCounterDown > DEBOUNCE_COUNTER_MAX)
  {
    if (downButtonState != lastDownButtonState)
    {
      if (downButtonState == LOW)
      {
        // Decrement the current digit
        int tempVal = int(newCalibrationFactor * 1000);             // Multiply by 1000 for 3 decimal places
        int digit = (int)(tempVal / pow(10, currentDigitIdx)) % 10; // Isolate the digit
        int newDigit = (digit + 9) % 10;                            // Decrease the digit
        tempVal -= (digit - newDigit) * pow(10, currentDigitIdx);   // Replace the digit
        newCalibrationFactor = float(tempVal) / 1000.0;             // Divide by 1000 to restore precision
        Serial.print("New calibration factor: ");
        Serial.println(newCalibrationFactor, 3);
        displayCalibrationFactor(newCalibrationFactor, currentDigitIdx); // Update display
        buzz(SHORT_BUZZ_DURATION);                                       // Short buzz for button press
      }
      lastDownButtonState = downButtonState;
    }
  }

  // Debounce logic for SELECT button
  if (selectButtonState != debounceButtonStateSelect)
  {
    lastDebounceTimeSelect = currentTime;
    debounceCounterSelect = 0;
  }

  debounceCounterSelect++;

  if ((currentTime - lastDebounceTimeSelect) > DEBOUNCE_DELAY && debounceCounterSelect > DEBOUNCE_COUNTER_MAX)
  {
    if (selectButtonState != lastSelectButtonState)
    {
      if (selectButtonState == LOW)
      {
        buttonPressStartTime = currentTime; // Start timer for button hold detection
      }
      else
      {
        currentDigitIdx = (currentDigitIdx + 1) % 8; // Move to next digit
        buzz(SHORT_BUZZ_DURATION);                   // Short buzz for button release
      }

      lastSelectButtonState = selectButtonState;
    }
    else // Button is held down
    {
      if (lastSelectButtonState == LOW) // Ensure the button was previously pressed
      {
        if (currentTime - buttonPressStartTime > SAVE_PROMPT_DURATION)
        {
          save_CalibrationFactor(newCalibrationFactor);
          calibration_factor = newCalibrationFactor;
          Serial.println("Calibration factor saved.");
          calibrationMode = false;
          displayCalibrationFactor(calibration_factor); // Update display
          buzz(LONG_BUZZ_DURATION);                     // Long buzz for button press
          return;
        }
      }
    }
  }
}

void handleDefaultMode()
{
  bool selectButtonState = digitalRead(BUTTON_SELECT_PIN);
  unsigned long currentTime = millis();

  // Debounce logic for SELECT button
  if (selectButtonState != debounceButtonStateSelect)
  {
    lastDebounceTimeSelect = currentTime;
    debounceCounterSelect = 0;
  }

  debounceCounterSelect++;

  if ((currentTime - lastDebounceTimeSelect) > DEBOUNCE_DELAY && debounceCounterSelect > DEBOUNCE_COUNTER_MAX)
  {
    if (selectButtonState != lastSelectButtonState)
    {
      if (selectButtonState == LOW)
      {
        buttonPressStartTime = currentTime; // Start timer for button hold detection
      }
      else
      {
        buzz(SHORT_BUZZ_DURATION); // Short buzz for button release
      }

      lastSelectButtonState = selectButtonState;
    }
    else // Button is held down
    {
      if (lastSelectButtonState == LOW) // Ensure the button was previously pressed
      {
        if (currentTime - buttonPressStartTime > CALIBRATION_MODE_DURATION)
        {
          calibrationMode = true;   // Enter calibration mode
          buzz(LONG_BUZZ_DURATION); // Long buzz for button hold detection

          Serial.println("Entered Calibration Mode");
          currentDigitIdx = 0;
          newCalibrationFactor = calibration_factor;
          return;
        }
      }
    }
  }

  // Handle weight reading
  if ((currentTime - lastReadTime) > READ_DELAY)
  {
    if (scale.is_ready())
    {
      float weight = scale.get_units(SENSOR_READ_COUNT);
      Serial.print("Weight: ");
      Serial.print(weight, DISPLAY_DECIMAL);
      Serial.println(" g");

      // Update display with current weight
      displayWeight(weight);

      lastReadTime = currentTime; // Update the last read time
    }
    else
    {
      Serial.println("HX711 not found.");
    }
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT); // Initialize buzzer pin

  EEPROM.begin();

  calibration_factor = load_CalibrationFactor();

  Serial.print("Loaded calibration factor: ");
  Serial.println(calibration_factor);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();

  Serial.println("HX711 Initialized");

  initDisplay();                                // Initialize display
  displayCalibrationFactor(calibration_factor); // Show initial calibration factor
}

void loop()
{
  if (calibrationMode)
  {
    handleCalibrationMode();
  }
  else
  {
    handleDefaultMode();
    delay(LOOP_DELAY); // Delay between measurements
  }
}
