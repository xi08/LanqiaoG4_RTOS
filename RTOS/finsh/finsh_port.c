/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <rtconfig.h>
#include <rthw.h>

#include "main.h"

#ifndef RT_USING_FINSH
#error Please uncomment the line <#include "finsh_config.h"> in the rtconfig.h
#endif

#ifdef RT_USING_FINSH

RT_WEAK char rt_hw_console_getchar(void)
{
    /* Note: the initial value of ch must < 0 */
    int ch = -1;

    if (LL_USART_IsActiveFlag_RXNE(USART1))
        ch = LL_USART_ReceiveData8(USART1);

    else
    {
        if (LL_USART_IsActiveFlag_ORE(USART1))
            LL_USART_ClearFlag_ORE(USART1);
        //rt_thread_mdelay(1);
    }

    return ch;
}

#endif /* RT_USING_FINSH */
