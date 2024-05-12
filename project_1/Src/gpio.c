/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "tim.h"
#include "usart.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED3_Pin|LED4_Pin|LED5_Pin|LED6_Pin
                          |LED7_Pin|LED8_Pin|LED1_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                           PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = LED3_Pin|LED4_Pin|LED5_Pin|LED6_Pin
                          |LED7_Pin|LED8_Pin|LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin */
  GPIO_InitStruct.Pin = Key1_Pin|Key2_Pin|Key3_Pin|Key4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 2);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 2, 2);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 2);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 2);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

/* USER CODE BEGIN 2 */
void Display_gray(){

    HAL_GPIO_WritePin(GPIOE, LED1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, LED2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, LED3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, LED4_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOE, LED5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, LED6_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, LED7_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, LED8_Pin, GPIO_PIN_RESET);
}

void Flow_LED(){
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_All, GPIO_PIN_RESET);
    TIM_Delay_ms(500);
    HAL_GPIO_WritePin(GPIOE, LED_array[0], GPIO_PIN_SET);
    TIM_Delay_ms(500);
    HAL_GPIO_WritePin(GPIOE, LED_array[1], GPIO_PIN_SET);
    TIM_Delay_ms(500);
    HAL_GPIO_WritePin(GPIOE, LED_array[2], GPIO_PIN_SET);
    TIM_Delay_ms(500);
    HAL_GPIO_WritePin(GPIOE, LED_array[3], GPIO_PIN_SET);
    TIM_Delay_ms(2000);
    HAL_GPIO_WritePin(GPIOE, LED_array[3], GPIO_PIN_RESET);
    TIM_Delay_ms(1000);
    HAL_GPIO_WritePin(GPIOE, LED_array[2], GPIO_PIN_RESET);
    TIM_Delay_ms(1000);
    HAL_GPIO_WritePin(GPIOE, LED_array[1], GPIO_PIN_RESET);
    TIM_Delay_ms(1000);
    HAL_GPIO_WritePin(GPIOE, LED_array[0], GPIO_PIN_RESET);

}

void Flow_LED_UART(){
    HAL_GPIO_WritePin(GPIOE, LED_array[0], GPIO_PIN_SET);
    println("D4,D5 ON");
    TIM_Delay_ms(500);
    HAL_GPIO_WritePin(GPIOE, LED_array[1], GPIO_PIN_SET);
    println("D3,D6 ON");
    TIM_Delay_ms(500);
    HAL_GPIO_WritePin(GPIOE, LED_array[2], GPIO_PIN_SET);
    println("D2,D7 ON");
    TIM_Delay_ms(500);
    HAL_GPIO_WritePin(GPIOE, LED_array[3], GPIO_PIN_SET);
    println("D1,D8 ON");
    TIM_Delay_ms(2000);
    HAL_GPIO_WritePin(GPIOE, LED_array[3], GPIO_PIN_RESET);
    println("D1,D8 OFF");
    TIM_Delay_ms(1000);
    HAL_GPIO_WritePin(GPIOE, LED_array[2], GPIO_PIN_RESET);
    println("D2,D7 OFF");
    TIM_Delay_ms(1000);
    HAL_GPIO_WritePin(GPIOE, LED_array[1], GPIO_PIN_RESET);
    println("D3,D6 OFF");
    TIM_Delay_ms(1000);
    HAL_GPIO_WritePin(GPIOE, LED_array[0], GPIO_PIN_RESET);
    println("D4,D5 OFF");
}

/* USER CODE END 2 */
