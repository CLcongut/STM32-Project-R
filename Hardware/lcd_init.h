#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

#define USE_HORIZONTAL 1 // 设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif

#define GPIO_SetBits(gpio, bits) gpio->ODR |= bits
#define GPIO_ResetBits(gpio, bits) gpio->ODR &= ~bits

//-----------------LCD端口定义----------------

#define LCD_RES_Clr() GPIO_ResetBits(RES_GPIO_Port, RES_Pin) // RES
#define LCD_RES_Set() GPIO_SetBits(RES_GPIO_Port, RES_Pin)

#define LCD_DC_Clr() GPIO_ResetBits(DC_GPIO_Port, DC_Pin) // DC
#define LCD_DC_Set() GPIO_SetBits(DC_GPIO_Port, DC_Pin)

#define LCD_CS_Clr() GPIO_ResetBits(CS_GPIO_Port, CS_Pin) // CS
#define LCD_CS_Set() GPIO_SetBits(CS_GPIO_Port, CS_Pin)

#define LCD_BLK_Clr() GPIO_ResetBits(BLK_GPIO_Port, BLK_Pin) // BLK
#define LCD_BLK_Set() GPIO_SetBits(BLK_GPIO_Port, BLK_Pin)

void LCD_GPIO_Init(void);                                                 // 初始化GPIO
void LCD_Writ_Bus(uint8_t dat);                                           // 模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);                                           // 写入一个字节
void LCD_WR_DATA(uint16_t dat);                                           // 写入两个字节
void LCD_WR_REG(uint8_t dat);                                             // 写入一个指令
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); // 设置坐标函数
void LCD_Init(void);                                                      // LCD初始化
#endif
