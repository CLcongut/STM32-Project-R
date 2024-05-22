#include "main.h"
#include "gui.h"

#include "lcd.h"
#include "lcd_init.h"

#define FONT_SIZE 16
#define LINE_Y_EXC 20

#define RAIL_Y_POS 20
#define LIGHT_Y_POS 44
#define RAIN_Y_POS 68
#define TEMP_Y_POS 92
#define HUMI_Y_POS 116

void _T_gui(void)
{

}

void _gui_load(void)
{
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
    LCD_DrawLine(0, RAIL_Y_POS + LINE_Y_EXC, LCD_W, RAIL_Y_POS + LINE_Y_EXC, GREEN);
    LCD_DrawLine(0, LIGHT_Y_POS + LINE_Y_EXC, LCD_W, LIGHT_Y_POS + LINE_Y_EXC, GREEN);
    LCD_DrawLine(0, RAIN_Y_POS + LINE_Y_EXC, LCD_W, RAIN_Y_POS + LINE_Y_EXC, GREEN);
    LCD_DrawLine(0, TEMP_Y_POS + LINE_Y_EXC, LCD_W, TEMP_Y_POS + LINE_Y_EXC, GREEN);
    Draw_Circle(102, TEMP_Y_POS + 4, 2, BLACK);

    LCD_ShowChinese(24, 0, "���¸�״̬", BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChinese(24, LIGHT_Y_POS, "����", BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChinese(32, RAIN_Y_POS, "��ǰ", BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChinese(80, RAIN_Y_POS, "��", BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChinese(12, TEMP_Y_POS, "�¶�", BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChinese(12, HUMI_Y_POS, "ʪ��", BLACK, WHITE, FONT_SIZE, 0);

    LCD_ShowChar(58, LIGHT_Y_POS, ':', BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChar(96, LIGHT_Y_POS, '%', BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChar(44, TEMP_Y_POS, ':', BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChar(106, TEMP_Y_POS, 'C', BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChar(44, HUMI_Y_POS, ':', BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChar(102, HUMI_Y_POS, '%', BLACK, WHITE, FONT_SIZE, 0);
}

void _gui_rail_state(uint8_t state)
{
    switch (state)
    {
    case 0 :
        LCD_Fill(46, RAIL_Y_POS, 46 + FONT_SIZE, RAIL_Y_POS + FONT_SIZE, WHITE);
        LCD_ShowChar(74, RAIL_Y_POS, '>', DARKBLUE, WHITE, FONT_SIZE, 0);
        LCD_ShowChinese(30, RAIL_Y_POS, "��", BLACK, WHITE, FONT_SIZE, 0);
        LCD_ShowChinese(82, RAIL_Y_POS, "��", DARKBLUE, LIGHTBLUE, FONT_SIZE, 0);
        break;

    case 1 :
        LCD_Fill(74, RAIL_Y_POS, 66 + FONT_SIZE, RAIL_Y_POS + FONT_SIZE, WHITE);
        LCD_ShowChar(46, RAIL_Y_POS, '<', DARKBLUE, WHITE, FONT_SIZE, 0);
        LCD_ShowChinese(30, RAIL_Y_POS, "��", DARKBLUE, LIGHTBLUE, FONT_SIZE, 0);
        LCD_ShowChinese(82, RAIL_Y_POS, "��", BLACK, WHITE, FONT_SIZE, 0);
        break;
    }
}

void _gui_light_state(uint8_t *light)
{
    LCD_ShowIntNum(74, LIGHT_Y_POS, light[1], 2, RED, YELLOW, FONT_SIZE);
}

void _gui_rain_state(uint8_t rain)
{
    switch (rain)
    {
    case 0 :
        LCD_ShowChinese(64, RAIN_Y_POS, "��", BLACK, WHITE, FONT_SIZE, 0);
        break;
    
    case 1 :
        LCD_ShowChinese(64, RAIN_Y_POS, "��", BLACK, WHITE, FONT_SIZE, 0);
        break;
    }
}

void _gui_tah_state(uint8_t *tah)
{
    float temp, humi;
    temp = tah[2] + tah[3] / 100.0;
    humi = tah[0] + tah[1] / 100.0;

    LCD_ShowFloatNum1(56, TEMP_Y_POS, temp, 4, DARKBLUE, LGRAY, 16);
    LCD_ShowFloatNum1(56, HUMI_Y_POS, humi, 4, DARKBLUE, LGRAY, 16);
}