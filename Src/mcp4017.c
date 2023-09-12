#include "mcp4017.h"
#include "i2c_sw.h"
#include "swDelay.h"
#include <rtthread.h>

uint8_t MCP4017_Read(void)
{
    uint8_t dat;

    swI2C_Start();
    swI2C_Write(0x5f);
    swI2C_WACK();
    dat = swI2C_Read();
    swI2C_NACK();
    swI2C_Stop();

    return dat;
}

void MCP4017_Write(uint8_t dat)
{
    swI2C_Start();
    swI2C_Write(0x5e);
    swI2C_WACK();
    swI2C_Write(dat);
    swI2C_WACK();
    swI2C_Stop();
}
