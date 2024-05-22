#include "main.h"
#include "RainSensor.h"

/**
 * @brief 获取雨滴传感器数据
 * 
 * @return uint8_t 有雨为 1，无雨为 0
 */
uint8_t Rain_Sensor_Read(void)
{
    return HAL_GPIO_ReadPin(Rain_GPIO_Port, Rain_Pin);
}