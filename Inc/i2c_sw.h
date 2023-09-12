#ifndef __I2CSW_H
#define __I2CSW_H

#include "main.h"

void swI2C_Init(void);
void swI2C_Start(void);
void swI2C_Stop(void);
uint8_t swI2C_WACK(void);
void swI2C_ACK(void);
void swI2C_NACK(void);
void swI2C_Write(uint8_t writeByte);
uint8_t swI2C_Read(void);

#endif // !__I2C_H
