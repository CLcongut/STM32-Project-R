#include "main.h"
#include "Beep.h"

void Beep_Alarm_ON(void)
{
    HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_SET);
}

void Beep_Alarm_OFF(void)
{
    HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_RESET);
}
