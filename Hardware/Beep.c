#include "main.h"
#include "Beep.h"

/**
 * @brief 打开蜂鸣器
 * 
 */
void Beep_Alarm_ON(void)
{
    HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_SET);
}

/**
 * @brief 关闭蜂鸣器
 * 
 */
void Beep_Alarm_OFF(void)
{
    HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_RESET);
}

/**
 * @brief 切换蜂鸣器开关
 * 
 */
void Beep_Alarm_TOGGLE(void)
{
    HAL_GPIO_TogglePin(Beep_GPIO_Port, Beep_Pin);
}