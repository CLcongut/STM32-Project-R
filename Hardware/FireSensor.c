#include "main.h"
#include "FireSensor.h"

// 数字脚通过比较器输出，比较达到阈值为低电平，否则为高电平
uint8_t Fire_Sensor_Read(void)
{
    return !HAL_GPIO_ReadPin(Fire_Sensor_GPIO_Port, Fire_Sensor_Pin);
}