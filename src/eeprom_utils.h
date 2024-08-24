// eeprom_utils.h
#ifndef EEPROM_UTILS_H
#define EEPROM_UTILS_H

#include <EEPROM.h>

void save_CalibrationFactor(float value);
float load_CalibrationFactor();
void save_SensorReadCount(int value);
int load_SensorReadCount();
void save_DisplayDecimal(int value);
int load_DisplayDecimal();
void save_LoopDelay(int value);
int load_LoopDelay();

#endif // EEPROM_UTILS_H
