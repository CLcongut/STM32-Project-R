#include "main.h"
#include "sys.h"
#include "gui.h"

#include "lcd.h"
#include "lcd_init.h"
#include "Beep.h"
#include "Relays.h"
#include "DHT11.h"
#include "MQ2Sensor.h"
#include "FireSensor.h"
#include "ESP01S.h"

#define Key1IN (Key1_GPIO_Port->IDR & Key1_Pin)
#define Key2IN (Key2_GPIO_Port->IDR & Key2_Pin)
#define Key3IN (Key3_GPIO_Port->IDR & Key3_Pin)
#define Key4IN (Key4_GPIO_Port->IDR & Key4_Pin)

// SYS_Data_Sruct SYS_DS = {50, 60, 0, 0};

uint8_t keycode;
uint8_t KeyTime;
// uint32_t PastTime1;
uint32_t PastTime2;

uint8_t dht11_buf[5];
uint8_t mq2_buf[2];
uint8_t fire_state;
uint8_t fume_state;
uint8_t beep_state;
uint8_t trans_task_switch;

short Fumes_Threshold_L = 50;
short Fumes_Threshold_H = 60;

void Fumes_Threshold_Switch(uint8_t fsw, uint8_t fvd)
{
    if (fsw)
    {
        if (fvd)
        {
            Fumes_Threshold_H++;
            if (Fumes_Threshold_H > 100)
            {
                Fumes_Threshold_H = 100;
            }
        }
        else
        {
            Fumes_Threshold_H--;
            if (Fumes_Threshold_L == Fumes_Threshold_H)
            {
                Fumes_Threshold_L--;
                if (Fumes_Threshold_L < 0)
                {
                    Fumes_Threshold_L = 0;
                }
            }
            if (Fumes_Threshold_H < 1)
            {
                Fumes_Threshold_H = 1;
            }
        }
    }
    else
    {
        if (fvd)
        {
            Fumes_Threshold_L++;
            if (Fumes_Threshold_L == Fumes_Threshold_H)
            {
                Fumes_Threshold_H++;
                if (Fumes_Threshold_H > 100)
                {
                    Fumes_Threshold_H = 100;
                }
            }
            if (Fumes_Threshold_L > 99)
            {
                Fumes_Threshold_L = 99;
            }
        }
        else
        {
            Fumes_Threshold_L--;
            if (Fumes_Threshold_L < 0)
            {
                Fumes_Threshold_L = 0;
            }
        }
    }
}

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
            if (++KeyTime > 10)
            {
                fume_state = !fume_state;
                _gui_Fumes_Alarm(Fumes_Threshold_L, Fumes_Threshold_H, fume_state, beep_state);
                while (!Key2IN)
                    ;
                KeyTime = 0;
                return;
            }
            HAL_Delay(100);
        }
        KeyTime = 0;
        keycode = 2;
        HAL_Delay(10);
    }

    if (!Key3IN)
    {
        HAL_Delay(10);
        while (!Key3IN)
        {
            if (++KeyTime > 3)
            {
                Fumes_Threshold_Switch(fume_state, 0);
            }
            _gui_Fumes_Alarm(Fumes_Threshold_L, Fumes_Threshold_H, fume_state, beep_state);
            LCD_Fill(10, 140, 20, 150, RED);
            HAL_Delay(100);
        }
        KeyTime = 0;
        keycode = 3;
        LCD_Fill(10, 140, 20, 150, YELLOW);
        HAL_Delay(10);
    }

    if (!Key4IN)
    {
        HAL_Delay(10);
        while (!Key4IN)
        {
            if (++KeyTime > 3)
            {
                Fumes_Threshold_Switch(fume_state, 1);
            }
            _gui_Fumes_Alarm(Fumes_Threshold_L, Fumes_Threshold_H, fume_state, beep_state);
            LCD_Fill(108, 140, 118, 150, RED);
            HAL_Delay(100);
        }
        KeyTime = 0;
        keycode = 4;
        LCD_Fill(108, 140, 118, 150, YELLOW);
        HAL_Delay(10);
    }
}

/**
 * @brief 系统滴答非阻塞延时
 *
 * @param Delay_Time 延时时间：毫秒
 * @return uint8_t 1为到达延时时间，0为未到达
 */
uint8_t SYS_Time_Interval(uint32_t CurrentTime, uint32_t PastTime, uint32_t Delay_Time)
{
    if (CurrentTime < PastTime) // 实时时间小于开始时间，时间计数发生回滚
    {
        if (0xFFFFFFFF - PastTime + CurrentTime >= Delay_Time)
        {
            PastTime = CurrentTime;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else // 实时时间大于开始时间
    {
        if (CurrentTime - PastTime >= Delay_Time)
        {
            PastTime = CurrentTime;
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

void _sys_Init(void)
{
    LCD_Init();
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
    _gui_Load();
    _gui_Fumes_Alarm(Fumes_Threshold_L, Fumes_Threshold_H, fume_state, beep_state);
}

void _sys_Loop(void)
{
    static uint32_t PastTime1;
    KeyScan();

    if (keycode)
    {
        switch (keycode)
        {
        case 1:
            LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
            ESP_Confirm_WIFIMQTT();
            ESP_Init();
            LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
            _gui_Load();
            _gui_Fumes_Alarm(Fumes_Threshold_L, Fumes_Threshold_H, fume_state, beep_state);
            break;

        case 2:
            Beep_Alarm_OFF();
            beep_state = !beep_state;
            _gui_Fumes_Alarm(Fumes_Threshold_L, Fumes_Threshold_H, fume_state, beep_state);
            break;

        case 3:
            Fumes_Threshold_Switch(fume_state, 0);
            _gui_Fumes_Alarm(Fumes_Threshold_L, Fumes_Threshold_H, fume_state, beep_state);
            break;

        case 4:
            Fumes_Threshold_Switch(fume_state, 1);
            _gui_Fumes_Alarm(Fumes_Threshold_L, Fumes_Threshold_H, fume_state, beep_state);
            break;
        }
        keycode = 0;
    }

    /*读取火焰传感器状态*/
    if (Fire_Sensor_Read() != fire_state)
    {
        fire_state = Fire_Sensor_Read();
        _gui_Fire_State(fire_state);
    }
    if (SYS_Time_Interval(HAL_GetTick(), PastTime1, 200))
    {
        PastTime1 = HAL_GetTick();
        /*判断：如果读取到温湿度传感器数据和烟雾传感器数据*/
        if (DHT_Read(dht11_buf) && MQ2_Read(mq2_buf))
        {
            /*将两个数据地址传入gui显示函数*/
            _gui_Fumes_Value(mq2_buf);
            _gui_TAH_Value(dht11_buf);
        }
    }
    /*判断：如果烟雾传感器数据超过设定的上阈值或者火焰传感器状态为检测到火焰*/
    if (mq2_buf[0] >= Fumes_Threshold_H || fire_state)
    {
        /*打开蜂鸣器拉响警报*/
        if (!beep_state)
        {
            Beep_Alarm_ON();
        }
        /*打开继电器*/
        Relays_ON();
    }
    /*判断：如果烟雾传感器数据低于设定的下阈值，并且火焰传感器状态为未检测到火焰*/
    else if (mq2_buf[0] < Fumes_Threshold_L && !fire_state)
    {
        /*关闭继电器*/
        Relays_OFF();
        /*关闭蜂鸣器*/
        Beep_Alarm_OFF();
    }

    if (SYS_Time_Interval(HAL_GetTick(), PastTime2, 1500))
    {
        PastTime2 = HAL_GetTick();
        ESP_MQTT_Trans_Data(trans_task_switch, dht11_buf[2], dht11_buf[0], mq2_buf[0], Fumes_Threshold_L, Fumes_Threshold_H, fire_state);
        if (++trans_task_switch > 3)
            trans_task_switch = 0;
    }
}
