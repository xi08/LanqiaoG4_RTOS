#include "i2c_sw.h"
#include "swDelay.h"
#include <rtthread.h>

void swI2C_Init(void)
{
    uint8_t i = 9;
    while (i--)
    {
        LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
        LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
    }
}

void swI2C_Start(void)
{
    LL_GPIO_SetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2);   // SDA=1
    LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
    LL_GPIO_ResetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2); // SDA=0
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
}

void swI2C_Stop(void)
{
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
    LL_GPIO_ResetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2); // SDA=0
    LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
    LL_GPIO_SetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2);   // SDA=1
}

uint8_t swI2C_WACK(void)
{
    uint8_t errTime = 5;
    LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=1
    while (LL_GPIO_IsInputPinSet(SDA_GPIO_Port, SDA_Pin))
    {
        errTime--;
        if (!errTime)
        {
            swI2C_Stop();
            return 1;
        }
    }
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
    return 0;
}

void swI2C_ACK(void)
{
    LL_GPIO_ResetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2); // SDA=0
    LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
}

void swI2C_NACK(void)
{
    LL_GPIO_SetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2);   // SDA=1
    LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
}

void swI2C_Write(uint8_t dat)
{
    uint8_t i = 8;
    while (i--)
    {
        LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
        if (dat & 0x80)
            LL_GPIO_SetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2); // SDA=1
        else
            LL_GPIO_ResetOutputPin(SDA_GPIO_Port, SDA_Pin), delay_us(2); // SDA=0
        LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);       // SCL=1
        dat <<= 1;
    }
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
}

uint8_t swI2C_Read(void)
{
    uint8_t i = 8, dat = 0;

    while (i--)
    {
        dat <<= 1;
        LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
        LL_GPIO_SetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2);   // SCL=1
        dat |= LL_GPIO_IsInputPinSet(SDA_GPIO_Port, SDA_Pin);
    }
    LL_GPIO_ResetOutputPin(SCL_GPIO_Port, SCL_Pin), delay_us(2); // SCL=0
    return dat;
}
