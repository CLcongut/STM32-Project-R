#include "main.h"
#include "LightSensor.h"
#include "adc.h"

uint8_t Light_Read(uint8_t *light) // ADC采集程序
{
    uint32_t light_value;
    uint8_t *data = light;
    HAL_ADC_Start(&hadc1);                                               // 开始ADC采集
    HAL_ADC_PollForConversion(&hadc1, 500);                              // 等待采集结束
    if (HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) // 读取ADC完成标志位
    {
        light_value = HAL_ADC_GetValue(&hadc1) * 100 / 4030; // 读出ADC数值
        data[0] = light_value;
        data[1] = 100 - data[0];
        return 1;
    }
    return 0;
}
