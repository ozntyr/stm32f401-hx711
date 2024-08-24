#include <Arduino.h>
#include <HX711.h>
#include "eeprom_utils.h"
#include "display_utils.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = PA0;
const int LOADCELL_SCK_PIN = PA1;

// Button pins
const int BUTTON_SELECT_PIN = PA2;
const int BUTTON_UP_PIN = PA3;
const int BUTTON_DOWN_PIN = PA4;

HX711 scale;

// Default values for settings
#define DEFAULT_CALIBRATION_FACTOR 2280.0f
#define DEFAULT_SENSOR_READ_COUNT 10
#define DEFAULT_DISPLAY_DECIMAL 2
#define DEFAULT_LOOP_DELAY 500

float calibration_factor = DEFAULT_CALIBRATION_FACTOR;
int sensorReadCount = DEFAULT_SENSOR_READ_COUNT;
int displayDecimal = DEFAULT_DISPLAY_DECIMAL;
int loopDelay = DEFAULT_LOOP_DELAY;

bool calibrationMode = false;
int currentDigitIdx = 0;
float newCalibrationFactor = DEFAULT_CALIBRATION_FACTOR;
unsigned long buttonPressStartTime = 0;
const unsigned long CALIBRATION_MODE_DURATION = 2000;
const unsigned long SAVE_PROMPT_DURATION = 2000;

static bool lastSelectButtonState = HIGH;
static bool lastUpButtonState = HIGH;
static bool lastDownButtonState = HIGH;
void handleCalibrationMode()
{
  bool upButtonState = digitalRead(BUTTON_UP_PIN);
  bool downButtonState = digitalRead(BUTTON_DOWN_PIN);

  if (digitalRead(BUTTON_SELECT_PIN) == LOW)
  {
    unsigned long pressDuration = millis() - buttonPressStartTime;

    if (pressDuration > SAVE_PROMPT_DURATION)
    {
      Serial.println("Do you want to save? Press SELECT again to confirm.");
      displaySavePrompt(); // Show save prompt on display
      while (digitalRead(BUTTON_SELECT_PIN) == LOW)
      {
        // Wait for confirmation
      }
      // Confirm and save
      save_CalibrationFactor(newCalibrationFactor);
      calibration_factor = newCalibrationFactor;
      Serial.println("Calibration factor saved.");
      calibrationMode = false;
      displayCalibrationFactor(calibration_factor); // Update display
      return;
    }
  }

  if (upButtonState == LOW && lastUpButtonState == HIGH)
  {
    // Increment the current digit
    int factorAsInt = int(newCalibrationFactor * pow(10, currentDigitIdx)) % 10;
    factorAsInt = (factorAsInt + 1) % 10;
    newCalibrationFactor += (factorAsInt - (int(newCalibrationFactor * pow(10, currentDigitIdx)) % 10)) * pow(10, currentDigitIdx);
    Serial.print("New calibration factor: ");
    Serial.println(newCalibrationFactor, 3);
    displayCalibrationFactor(newCalibrationFactor, currentDigitIdx); // Update display
  }
  if (downButtonState == LOW && lastDownButtonState == HIGH)
  {
    // Decrement the current digit
    int factorAsInt = int(newCalibrationFactor * pow(10, currentDigitIdx)) % 10;
    factorAsInt = (factorAsInt + 9) % 10;
    newCalibrationFactor += (factorAsInt - (int(newCalibrationFactor * pow(10, currentDigitIdx)) % 10)) * pow(10, currentDigitIdx);
    Serial.print("New calibration factor: ");
    Serial.println(newCalibrationFactor, 3);
    displayCalibrationFactor(newCalibrationFactor, currentDigitIdx); // Update display
  }
  if (digitalRead(BUTTON_SELECT_PIN) == HIGH && lastSelectButtonState == LOW)
  {
    // Move to the next digit
    currentDigitIdx = (currentDigitIdx + 1) % 8;
    Serial.print("Current digit index: ");
    Serial.println(currentDigitIdx);
    displayCalibrationFactor(newCalibrationFactor, currentDigitIdx); // Update display
  }

  lastUpButtonState = upButtonState;
  lastDownButtonState = downButtonState;
  lastSelectButtonState = digitalRead(BUTTON_SELECT_PIN);
}

void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);

  EEPROM.begin();

  calibration_factor = load_CalibrationFactor();
  sensorReadCount = load_SensorReadCount();
  displayDecimal = load_DisplayDecimal();
  loopDelay = load_LoopDelay();

  Serial.print("Loaded calibration factor: ");
  Serial.println(calibration_factor);
  Serial.print("Loaded sensor read count: ");
  Serial.println(sensorReadCount);
  Serial.print("Loaded display decimal: ");
  Serial.println(displayDecimal);
  Serial.print("Loaded loop delay: ");
  Serial.println(loopDelay);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();

  Serial.println("HX711 Initialized");

  initDisplay();                                // Initialize display
  displayCalibrationFactor(calibration_factor); // Show initial calibration factor
}

void loop()
{
  bool selectButtonState = digitalRead(BUTTON_SELECT_PIN);

  if (selectButtonState == LOW && lastSelectButtonState == HIGH)
  {
    buttonPressStartTime = millis();
  }
  else if (selectButtonState == HIGH && lastSelectButtonState == LOW)
  {
    unsigned long pressDuration = millis() - buttonPressStartTime;

    if (pressDuration > CALIBRATION_MODE_DURATION)
    {
      calibrationMode = !calibrationMode;
      if (calibrationMode)
      {
        Serial.println("Entered Calibration Mode");
        currentDigitIdx = 0;
        newCalibrationFactor = calibration_factor;
      }
      else
      {
        Serial.println("Exited Calibration Mode");
        displayCalibrationFactor(calibration_factor); // Update display
      }
    }
  }

  lastSelectButtonState = selectButtonState;

  if (calibrationMode)
  {
    handleCalibrationMode();
  }
  else
  {
    if (scale.is_ready())
    {
      float weight = scale.get_units(sensorReadCount);
      Serial.print("Weight: ");
      Serial.print(weight, displayDecimal);
      Serial.println(" g");
    }
    else
    {
      Serial.println("HX711 not found.");
    }

    delay(loopDelay);
  }
}
