#include "eeprom.h"
#include "i2c_sw.h"
#include <rtthread.h>
#include "swDelay.h"

uint8_t EEPROM_Read(uint8_t addr)
{
    uint8_t dat;

    swI2C_Start();
    swI2C_Write(0xa0);
    swI2C_WACK();
    swI2C_Write(addr);
    swI2C_WACK();

    swI2C_Start();
    swI2C_Write(0xa1);
    swI2C_WACK();
    dat = swI2C_Read();
    swI2C_NACK();
    swI2C_Stop();

    return dat;
}

void EEPROM_Write(uint8_t addr, uint8_t dat)
{
    swI2C_Start();
    swI2C_Write(0xa0);
    swI2C_WACK();
    swI2C_Write(addr);
    swI2C_WACK();
    swI2C_Write(dat);
    swI2C_WACK();
    swI2C_Stop();

    rt_thread_mdelay(6);
}
