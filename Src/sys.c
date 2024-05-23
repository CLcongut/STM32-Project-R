#include "main.h"
#include "sys.h"
#include "gui.h"

#include "lcd.h"
#include "lcd_init.h"
#include "Beep.h"
#include "DHT11.h"
#include "RainSensor.h"
#include "StepMotor.h"
#include "LightSensor.h"
#include "ESP01S.h"

/************************************************
 * 定义对应按键寄存器对应一位的高低
 */
#define Key1IN (Key1_GPIO_Port->IDR & Key1_Pin)
#define Key2IN (Key2_GPIO_Port->IDR & Key2_Pin)
#define Key3IN (Key3_GPIO_Port->IDR & Key3_Pin)

/*定义光照阈值，达到阈值并且无雨时升起晾衣杆*/
#define LIGHT_VALUE 70

/*定义键码*/
uint8_t keycode;

uint32_t PastTime;
uint32_t MotorTime;

uint8_t set_rail_state;
uint8_t cur_rail_state;
uint8_t past_rain_state;
uint8_t trans_task_switch;

uint8_t light_buf[2];
uint8_t tah_buf[4];

/**
 * @brief 按键扫描
 *
 */
void KeyScan(void)
{
    if (!Key1IN)
    {
        HAL_Delay(10);
        while (!Key1IN)
        {
        }
        keycode = 1;
        HAL_Delay(10);
    }

    if (!Key2IN)
    {
        HAL_Delay(10);
        while (!Key2IN)
        {
        }
        keycode = 2;
        HAL_Delay(10);
    }

    if (!Key3IN)
    {
        HAL_Delay(10);
        while (!Key3IN)
        {
        }
        keycode = 3;
        HAL_Delay(10);
    }
}

/**
 * @brief 系统滴答非阻塞延时
 *
 * @param Delay_Time 延时时间：毫秒
 * @return uint8_t 1为到达延时时间，0为未到达
 */
uint8_t SYS_Time_Interval(uint32_t Delay_Time)
{
    /*需要配合创建Pasttime全局变量, uint32_t*/
    uint32_t SYST = HAL_GetTick();
    if (SYST < PastTime) // 实时时间小于开始时间，时间计数发生回滚
    {
        if (0xFFFFFFFF - PastTime + SYST >= Delay_Time)
        {
            PastTime = HAL_GetTick();
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else // 实时时间大于开始时间
    {
        if (SYST - PastTime >= Delay_Time)
        {
            PastTime = HAL_GetTick();
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

/**
 * @brief 系统初始化
 *
 */
void _sys_Init(void)
{
    LCD_Init();
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
    _gui_load();
    _gui_rail_state(set_rail_state);
    _gui_rain_state(0);
}

/**
 * @brief 系统主循环
 *
 */
void _sys_Loop(void)
{
    /**************************************************
     * 按键控制部分
     * 开始 >>
     */
    KeyScan();
    if (keycode)
    {
        switch (keycode)
        {
        case 1:
            LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
            ESP_Confirm_WIFIMQTT();
            ESP_Init();
            _gui_load();
            _gui_rail_state(set_rail_state);
            _gui_rain_state(0);
            break;
        case 2:
            Beep_Alarm_OFF();
            break;
        case 3:
            set_rail_state = !set_rail_state;
            break;
        }
        keycode = 0;
    }
    /**************************************************
     * << 结束
     */

    /**************************************************
     * 舵机控制与显示部分
     * 开始 >>
     */
    if (set_rail_state != cur_rail_state)
    {
        cur_rail_state = set_rail_state;
        if (cur_rail_state)
        {
            while (MotorTime < 300)
            {
                Step_Motor_CW(1000);
                MotorTime++;
            }
            Step_Motor_Stop();
            MotorTime = 0;
        }
        else
        {
            while (MotorTime < 300)
            {
                Step_Motor_CCW(1000);
                MotorTime++;
            }
            Step_Motor_Stop();
            MotorTime = 0;
        }
        _gui_rail_state(set_rail_state);
    }
    /**************************************************
     * << 结束
     */

    /**************************************************
     * 光照显示部分
     * 开始 >>
     */
    if (Light_Read(light_buf))
    {
        _gui_light_state(light_buf);
    }
    /**************************************************
     * << 结束
     */

    /**************************************************
     * 温湿度显示部分
     * 开始 >>
     */
    if (DHT_Read(tah_buf))
    {
        _gui_tah_state(tah_buf);
    }
    /**************************************************
     * << 结束
     */

    /**************************************************
     * 雨滴显示与蜂鸣器逻辑部分
     * 开始 >>
     */
    if (Rain_Sensor_Read() != past_rain_state)
    {
        past_rain_state = Rain_Sensor_Read();
        if (Rain_Sensor_Read())
        {
            Beep_Alarm_ON();
            _gui_rain_state(1);
            set_rail_state = 0;
        }
        else
        {
            Beep_Alarm_OFF();
            _gui_rain_state(0);
        }
    }
    /**************************************************
     * << 结束
     */

    /**************************************************
     * 光照控制电机部分
     * 开始 >>
     */
    if (light_buf[1] >= 70 && !Rain_Sensor_Read())
    {
        set_rail_state = 1;
    }
    /**************************************************
     * << 结束
     */

    /**************************************************
     * 5个数据轮询发送部分，间隔太短可能造成ESP01负担
     * 开始 >>
     */
    if (SYS_Time_Interval(1500))
    {
        ESP_MQTT_Trans_Data(trans_task_switch);
        if (++trans_task_switch > 3)
            trans_task_switch = 0;
    }
    /**************************************************
     * << 结束
     */
}