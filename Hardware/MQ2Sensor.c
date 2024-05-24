#include "main.h"
#include "adc.h"
#include "MQ2Sensor.h"

uint8_t MQ2_Read(uint8_t *fumes) // ADC采集程序
{
    uint32_t fumes_value;
    uint8_t *data = fumes;
    HAL_ADC_Start(&hadc1);                                               // 开始ADC采集
    HAL_ADC_PollForConversion(&hadc1, 500);                              // 等待采集结束
    if (HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) // 读取ADC完成标志位
    {
        fumes_value = HAL_ADC_GetValue(&hadc1) * 10000 / 4095; // 读出ADC数值
        data[0] = fumes_value / 100;
        data[1] = fumes_value % 100;
        return 1;
    }
    return 0;
}