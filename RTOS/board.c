/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */
 
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>

#include "main.h"

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (15 * 1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
    extern void SystemClock_Config(void);
    extern void MX_GPIO_Init(void);
    extern void MX_DMA_Init(void);
    extern void MX_ADC1_Init(void);
    extern void MX_ADC2_Init(void);
    extern void MX_TIM3_Init(void);
    extern void MX_TIM8_Init(void);

    HAL_Init();
    SystemClock_Config();
    HAL_SetTickFreq(HAL_RCC_GetSysClockFreq() / RT_TICK_PER_SECOND);
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_ADC2_Init();
    MX_TIM3_Init();
    MX_TIM8_Init();

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

#ifdef RT_USING_CONSOLE

static int uart_init(void)
{
    extern void MX_USART1_UART_Init(void);
    MX_USART1_UART_Init();
    return 0;
}
INIT_BOARD_EXPORT(uart_init);
extern void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = rt_strlen(str);

    for (i = 0; i < size; i++)
    {
        if (str[i] == '\n')
        {
            while (!LL_USART_IsActiveFlag_TXE(USART1))
                ;
            LL_USART_TransmitData8(USART1, '\r');
        }

        while (!LL_USART_IsActiveFlag_TXE(USART1))
            ;
        LL_USART_TransmitData8(USART1, (uint8_t)str[i]);
    }
}

#endif
