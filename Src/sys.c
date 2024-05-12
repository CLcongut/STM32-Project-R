#include "main.h"
#include "sys.h"
#include "gui.h"

#include "OLED.h"
#include "Beep.h"
#include "Relays.h"
#include "DHT11.h"
#include "MQ2Sensor.h"
#include "FireSensor.h"

#define Fumes_Threshold_H 60
#define Fumes_Threshold_L 50

uint8_t dht11_buf[5];
uint8_t mq2_buf[2];
uint8_t fire_alarm = 0;

void _sys_Init(void)
{
    OLED_Init();
}

void _sys_Loop(void)
{
    /*读取火焰传感器状态*/
    fire_alarm = Fire_Sensor_Read();

    /*判断：如果读取到温湿度传感器数据和烟雾传感器数据*/
    if (DHT_Read(dht11_buf) && MQ2_Read(mq2_buf))
    {
        /*将两个数据地址传入gui显示函数*/
        gui_Display(dht11_buf, mq2_buf);
    }
    /*判断：如果烟雾传感器数据超过设定的上阈值或者火焰传感器状态为检测到火焰*/
    if (mq2_buf[0] >= Fumes_Threshold_H || fire_alarm == FIRE_ALARM)
    {
        /*打开蜂鸣器拉响警报*/
        Beep_Alarm_ON();
        /*打开继电器*/
        Relays_ON();
    }
    /*判断：如果烟雾传感器数据低于设定的下阈值，并且火焰传感器状态为未检测到火焰*/
    else if (mq2_buf[0] < Fumes_Threshold_L && fire_alarm == FIRE_DIS)
    {
        /*关闭继电器*/
        Relays_OFF();
        /*关闭蜂鸣器*/
        Beep_Alarm_OFF();
    }
    /*增加延时减少传感器传入次数以此减少gui数据跳变*/
    HAL_Delay(80);
}