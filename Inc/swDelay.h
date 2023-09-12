#ifndef __SWDELAY_H

#include "main.h"

#define delay_us(x) swDelay5((x)*170 / 5)
#define delay_ms(x) swDelay5((x)*170 * 200)

void swDelay5(uint32_t n);

#endif // !__SWDELAY_H
