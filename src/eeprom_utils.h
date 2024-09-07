// eeprom_utils.h
#ifndef EEPROM_UTILS_H
#define EEPROM_UTILS_H

#include <EEPROM.h>

#define EEPROM_FLAG_VALUE 0x12345678
#define DEFAULT_CALIBRATION_FACTOR 2280.0f
#define SENSOR_READ_COUNT 10
#define DISPLAY_DECIMAL 3
#define LOOP_DELAY 10

void save_CalibrationFactor(float value);
float load_CalibrationFactor();
void save_SensorReadCount(int value);
int load_SensorReadCount();
void save_DisplayDecimal(int value);
int load_DisplayDecimal();
void save_LoopDelay(int value);
int load_LoopDelay();

#endif // EEPROM_UTILS_H
