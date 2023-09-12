/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

#include "stm32g4xx_ll_usart.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_exti.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LD5_Pin LL_GPIO_PIN_13
#define LD5_GPIO_Port GPIOC
#define LD6_Pin LL_GPIO_PIN_14
#define LD6_GPIO_Port GPIOC
#define LD7_Pin LL_GPIO_PIN_15
#define LD7_GPIO_Port GPIOC
#define OSCI_Pin LL_GPIO_PIN_0
#define OSCI_GPIO_Port GPIOF
#define OSCO_Pin LL_GPIO_PIN_1
#define OSCO_GPIO_Port GPIOF
#define LC0_Pin LL_GPIO_PIN_0
#define LC0_GPIO_Port GPIOC
#define LC1_Pin LL_GPIO_PIN_1
#define LC1_GPIO_Port GPIOC
#define LC2_Pin LL_GPIO_PIN_2
#define LC2_GPIO_Port GPIOC
#define LC3_Pin LL_GPIO_PIN_3
#define LC3_GPIO_Port GPIOC
#define B4_Pin LL_GPIO_PIN_0
#define B4_GPIO_Port GPIOA
#define SER_Pin LL_GPIO_PIN_1
#define SER_GPIO_Port GPIOA
#define RCK_Pin LL_GPIO_PIN_2
#define RCK_GPIO_Port GPIOA
#define SCK_Pin LL_GPIO_PIN_3
#define SCK_GPIO_Port GPIOA
#define TRA_Pin LL_GPIO_PIN_4
#define TRA_GPIO_Port GPIOA
#define AKY_Pin LL_GPIO_PIN_5
#define AKY_GPIO_Port GPIOA
#define OWD_Pin LL_GPIO_PIN_6
#define OWD_GPIO_Port GPIOA
#define DHT_Pin LL_GPIO_PIN_7
#define DHT_GPIO_Port GPIOA
#define LC4_Pin LL_GPIO_PIN_4
#define LC4_GPIO_Port GPIOC
#define LC5_Pin LL_GPIO_PIN_5
#define LC5_GPIO_Port GPIOC
#define B1_Pin LL_GPIO_PIN_0
#define B1_GPIO_Port GPIOB
#define B2_Pin LL_GPIO_PIN_1
#define B2_GPIO_Port GPIOB
#define B3_Pin LL_GPIO_PIN_2
#define B3_GPIO_Port GPIOB
#define R38_Pin LL_GPIO_PIN_12
#define R38_GPIO_Port GPIOB
#define MCP_Pin LL_GPIO_PIN_14
#define MCP_GPIO_Port GPIOB
#define R37_Pin LL_GPIO_PIN_15
#define R37_GPIO_Port GPIOB
#define LC6_Pin LL_GPIO_PIN_6
#define LC6_GPIO_Port GPIOC
#define LC7_Pin LL_GPIO_PIN_7
#define LC7_GPIO_Port GPIOC
#define LD0_Pin LL_GPIO_PIN_8
#define LD0_GPIO_Port GPIOC
#define LD1_Pin LL_GPIO_PIN_9
#define LD1_GPIO_Port GPIOC
#define nRD_Pin LL_GPIO_PIN_8
#define nRD_GPIO_Port GPIOA
#define TXD_Pin LL_GPIO_PIN_9
#define TXD_GPIO_Port GPIOA
#define RXD_Pin LL_GPIO_PIN_10
#define RXD_GPIO_Port GPIOA
#define SWDIO_Pin LL_GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin LL_GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define R40_Pin LL_GPIO_PIN_15
#define R40_GPIO_Port GPIOA
#define LD2_Pin LL_GPIO_PIN_10
#define LD2_GPIO_Port GPIOC
#define LD3_Pin LL_GPIO_PIN_11
#define LD3_GPIO_Port GPIOC
#define LD4_Pin LL_GPIO_PIN_12
#define LD4_GPIO_Port GPIOC
#define LE_Pin LL_GPIO_PIN_2
#define LE_GPIO_Port GPIOD
#define R39_Pin LL_GPIO_PIN_4
#define R39_GPIO_Port GPIOB
#define nWR_Pin LL_GPIO_PIN_5
#define nWR_GPIO_Port GPIOB
#define SCL_Pin LL_GPIO_PIN_6
#define SCL_GPIO_Port GPIOB
#define SDA_Pin LL_GPIO_PIN_7
#define SDA_GPIO_Port GPIOB
#define RS_Pin LL_GPIO_PIN_8
#define RS_GPIO_Port GPIOB
#define nCS_Pin LL_GPIO_PIN_9
#define nCS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
