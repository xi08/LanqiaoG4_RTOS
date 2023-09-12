#ifndef __EEPROM_H
#define __EEPROM_H

#include "main.h"

uint8_t EEPROM_Read(uint8_t addr);
void EEPROM_Write(uint8_t addr,uint8_t dat);

#endif // !__EEPROM_H
