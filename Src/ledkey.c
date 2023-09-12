#include "ledkey.h"

void ledUpdate(uint8_t led)
{
    LL_GPIO_WriteOutputPort(LD1_GPIO_Port, (~led) << 8);
    LL_GPIO_SetOutputPin(LE_GPIO_Port, LE_Pin);
    (__NOP, __NOP, __NOP);
    LL_GPIO_ResetOutputPin(LE_GPIO_Port, LE_Pin);
    (__NOP, __NOP, __NOP);
}
