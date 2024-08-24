// eeprom_utils.cpp
#include "eeprom_utils.h"

// EEPROM indices for each variable
#define EEPROM_IDX_CALIBRATION_FACTOR 0
#define EEPROM_IDX_SENSOR_READ_COUNT (EEPROM_IDX_CALIBRATION_FACTOR + sizeof(float))
#define EEPROM_IDX_DISPLAY_DECIMAL (EEPROM_IDX_SENSOR_READ_COUNT + sizeof(int))
#define EEPROM_IDX_LOOP_DELAY (EEPROM_IDX_DISPLAY_DECIMAL + sizeof(int))

void save_CalibrationFactor(float value)
{
    EEPROM.put(EEPROM_IDX_CALIBRATION_FACTOR, value); // Save calibration factor to EEPROM
}

float load_CalibrationFactor()
{
    float value;
    EEPROM.get(EEPROM_IDX_CALIBRATION_FACTOR, value); // Load calibration factor from EEPROM
    return value;
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
