/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define USART_REC_LEN 400
  /* USER CODE END EM */

  /* Exported functions prototypes ---------------------------------------------*/
  void Error_Handler(void);

  /* USER CODE BEGIN EFP */
  extern uint8_t USART1_RX_BUF[USART_REC_LEN];

  typedef struct
  {
    uint8_t rx_data[USART_REC_LEN];
    uint8_t rx_mqtt_en_flag;
  } ESP_RX_STRUCT;

  extern ESP_RX_STRUCT ESP_RXS;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DHT11_Pin GPIO_PIN_4
#define DHT11_GPIO_Port GPIOA
#define Key3_Pin GPIO_PIN_1
#define Key3_GPIO_Port GPIOB
#define Key2_Pin GPIO_PIN_10
#define Key2_GPIO_Port GPIOB
#define Key1_Pin GPIO_PIN_11
#define Key1_GPIO_Port GPIOB
#define BLK_Pin GPIO_PIN_12
#define BLK_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_13
#define CS_GPIO_Port GPIOB
#define DC_Pin GPIO_PIN_14
#define DC_GPIO_Port GPIOB
#define RES_Pin GPIO_PIN_15
#define RES_GPIO_Port GPIOB
#define Rain_Pin GPIO_PIN_4
#define Rain_GPIO_Port GPIOB
#define Beep_Pin GPIO_PIN_5
#define Beep_GPIO_Port GPIOB
#define LA_Pin GPIO_PIN_6
#define LA_GPIO_Port GPIOB
#define LB_Pin GPIO_PIN_7
#define LB_GPIO_Port GPIOB
#define LC_Pin GPIO_PIN_8
#define LC_GPIO_Port GPIOB
#define LD_Pin GPIO_PIN_9
#define LD_GPIO_Port GPIOB

  /* USER CODE BEGIN Private defines */

  /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
