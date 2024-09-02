// eeprom_utils.cpp
#include "eeprom_utils.h"

// EEPROM indices for each variable
#define EEPROM_IDX_FLAG 0
#define EEPROM_IDX_CALIBRATION_FACTOR (EEPROM_IDX_FLAG + sizeof(uint32_t))
#define EEPROM_IDX_SENSOR_READ_COUNT (EEPROM_IDX_CALIBRATION_FACTOR + sizeof(float))
#define EEPROM_IDX_DISPLAY_DECIMAL (EEPROM_IDX_SENSOR_READ_COUNT + sizeof(int))
#define EEPROM_IDX_LOOP_DELAY (EEPROM_IDX_DISPLAY_DECIMAL + sizeof(int))

// Save the calibration factor and set an initialization flag
void save_CalibrationFactor(float value)
{
    EEPROM.put(EEPROM_IDX_FLAG, EEPROM_FLAG_VALUE);   // Save flag to EEPROM to indicate initialization
    EEPROM.put(EEPROM_IDX_CALIBRATION_FACTOR, value); // Save calibration factor to EEPROM
}

// Load the calibration factor from EEPROM, returning default if not initialized
float load_CalibrationFactor()
{
    float value;
    uint32_t flagValue;

    // Read the flag value first to check if EEPROM has been initialized
    EEPROM.get(EEPROM_IDX_FLAG, flagValue);

    if (flagValue == EEPROM_FLAG_VALUE)
    {
        // If flag value matches, read and return the calibration factor
        EEPROM.get(EEPROM_IDX_CALIBRATION_FACTOR, value);
        return value;
    }
    else
    {
        // If flag value does not match, save the default value and return it
        save_CalibrationFactor(DEFAULT_CALIBRATION_FACTOR);
        return DEFAULT_CALIBRATION_FACTOR;
    }
}

void save_SensorReadCount(int value)
{
    EEPROM.put(EEPROM_IDX_SENSOR_READ_COUNT, value); // Save sensor read count to EEPROM
}

int load_SensorReadCount()
{
    int value;
    EEPROM.get(EEPROM_IDX_SENSOR_READ_COUNT, value); // Load sensor read count from EEPROM
    return value;
}

void save_DisplayDecimal(int value)
{
    EEPROM.put(EEPROM_IDX_DISPLAY_DECIMAL, value); // Save display decimal count to EEPROM
}

int load_DisplayDecimal()
{
    int value;
    EEPROM.get(EEPROM_IDX_DISPLAY_DECIMAL, value); // Load display decimal count from EEPROM
    return value;
}

void save_LoopDelay(int value)
{
    EEPROM.put(EEPROM_IDX_LOOP_DELAY, value); // Save loop delay to EEPROM
}

int load_LoopDelay()
{
    int value;
    EEPROM.get(EEPROM_IDX_LOOP_DELAY, value); // Load loop delay from EEPROM
    return value;
}
