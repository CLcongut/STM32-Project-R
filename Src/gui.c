#include "main.h"
#include "gui.h"

#include "lcd.h"
#include "lcd_init.h"
#include "Beep.h"

#define FONT_SIZE 16
#define LINE_Y_EXC 20

#define fire_sensor_X_POS 20
#define fire_sensor_Y_POS 8

#define fumes_value_X_POS 42
#define fumes_value_Y_POS 54

#define temp_value_X_POS 22
#define temp_value_Y_POS 96

#define humi_value_X_POS 76
#define humi_value_Y_POS 96

#define fumes_alarm_X_T_POS 38
#define fumes_alarm_X_L_POS 24
#define fumes_alarm_X_H_POS 68
#define fumes_alarm_Y_POS 134

void _gui_Load(void)
{
    _gui_Fire_State(0);
    LCD_DrawLine(0, fire_sensor_Y_POS + LINE_Y_EXC, LCD_W, fire_sensor_Y_POS + LINE_Y_EXC, BLUE);

    LCD_ShowChinese(fumes_value_X_POS - 12, fumes_value_Y_POS - 20, "烟雾浓度", BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChar(fumes_value_X_POS - 12 + 64, fumes_value_Y_POS - 20, ':', BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChar(fumes_value_X_POS + 44, fumes_value_Y_POS, '%', BLACK, WHITE, FONT_SIZE, 0);
    LCD_DrawLine(0, fumes_value_Y_POS + LINE_Y_EXC, LCD_W, fumes_value_Y_POS + LINE_Y_EXC, BLUE);

    LCD_ShowChinese(temp_value_X_POS - 2, temp_value_Y_POS - 18, "温度", BLACK, WHITE, FONT_SIZE, 0);
    Draw_Circle(temp_value_X_POS + 22, temp_value_Y_POS + 4, 2, BLACK);
    LCD_ShowChar(temp_value_X_POS + 26, temp_value_Y_POS, 'C', BLACK, WHITE, FONT_SIZE, 0);

    LCD_ShowChinese(humi_value_X_POS - 2, humi_value_Y_POS - 18, "湿度", BLACK, WHITE, FONT_SIZE, 0);
    LCD_ShowChar(humi_value_X_POS + 22, humi_value_Y_POS, '%', BLACK, WHITE, FONT_SIZE, 0);
    LCD_DrawLine(0, humi_value_Y_POS + LINE_Y_EXC, LCD_W, humi_value_Y_POS + LINE_Y_EXC, BLUE);

    LCD_ShowChinese(fumes_alarm_X_T_POS, fumes_alarm_Y_POS - 14, "报警阈值", BLACK, WHITE, 12, 0);
    LCD_Fill(10, 140, 20, 150, YELLOW);
    LCD_Fill(108, 140, 118, 150, YELLOW);
}

void _gui_Fire_State(uint8_t FState)
{
    if (FState)
    {
        LCD_Fill(fire_sensor_X_POS - 6, fire_sensor_Y_POS, fire_sensor_X_POS + 94, fire_sensor_Y_POS + 16, WHITE);
        LCD_ShowChinese(fire_sensor_X_POS, fire_sensor_Y_POS, "检测到明火", RED, WHITE, FONT_SIZE, 0);
        LCD_ShowChar(fire_sensor_X_POS + 82, fire_sensor_Y_POS, '!', RED, WHITE, FONT_SIZE, 0);
    }
    else
    {
        LCD_ShowChinese(fire_sensor_X_POS - 4, fire_sensor_Y_POS, "未检测到明火", BLACK, WHITE, FONT_SIZE, 0);
    }
}

void _gui_Fumes_Value(uint8_t *fume_buf)
{
    float f_fume = fume_buf[0] + fume_buf[1] / 100.0;
    LCD_ShowFloatNum1(fumes_value_X_POS, fumes_value_Y_POS, f_fume, 4, WHITE, GRAY, FONT_SIZE);
}

void _gui_TAH_Value(uint8_t *tah_buf)
{
    LCD_ShowIntNum(temp_value_X_POS, temp_value_Y_POS, tah_buf[2], 2, BLUE, LIGHTBLUE, FONT_SIZE);
    LCD_ShowIntNum(humi_value_X_POS, humi_value_Y_POS, tah_buf[0], 2, BLUE, LIGHTBLUE, FONT_SIZE);
}

void _gui_Fumes_Alarm(uint8_t d_fume_L_alarm, uint8_t d_fume_H_alarm, uint8_t b_fume_state, uint8_t b_beep_state)
{
    uint16_t fume_L_state_color;
    uint16_t fume_H_state_color;
    if (b_fume_state)
    {
        fume_L_state_color = YELLOW;
        fume_H_state_color = RED;
    }
    else
    {
        fume_L_state_color = RED;
        fume_H_state_color = YELLOW;
    }

    if (!b_beep_state)
    {
        LCD_Fill(fumes_alarm_X_T_POS + 70, fumes_alarm_Y_POS - 14, fumes_alarm_X_T_POS + 82, fumes_alarm_Y_POS - 2, WHITE);
        LCD_ShowChinese(fumes_alarm_X_T_POS - 32, fumes_alarm_Y_POS - 14, "开", RED, WHITE, 12, 0);
    }
    else
    {
        LCD_Fill(fumes_alarm_X_T_POS - 32, fumes_alarm_Y_POS - 14, fumes_alarm_X_T_POS, fumes_alarm_Y_POS + 10, WHITE);
        LCD_ShowChinese(fumes_alarm_X_T_POS + 70, fumes_alarm_Y_POS - 14, "关", GREEN, WHITE, 12, 0);
    }

    if (d_fume_L_alarm < 10)
    {
        LCD_Fill(fumes_alarm_X_L_POS + 6, fumes_alarm_Y_POS, fumes_alarm_X_L_POS + 12, fumes_alarm_Y_POS + 24, WHITE);
        LCD_Fill(fumes_alarm_X_L_POS + 24, fumes_alarm_Y_POS, fumes_alarm_X_L_POS + 30, fumes_alarm_Y_POS + 24, WHITE);
        LCD_ShowIntNum(fumes_alarm_X_L_POS + 12, fumes_alarm_Y_POS, d_fume_L_alarm, 1, fume_L_state_color, WHITE, 24);
    }
    else if (d_fume_L_alarm < 100)
    {
        LCD_Fill(fumes_alarm_X_L_POS, fumes_alarm_Y_POS, fumes_alarm_X_L_POS + 6, fumes_alarm_Y_POS + 24, WHITE);
        LCD_Fill(fumes_alarm_X_L_POS + 30, fumes_alarm_Y_POS, fumes_alarm_X_L_POS + 36, fumes_alarm_Y_POS + 24, WHITE);
        LCD_ShowIntNum(fumes_alarm_X_L_POS + 6, fumes_alarm_Y_POS, d_fume_L_alarm, 2, fume_L_state_color, WHITE, 24);
    }
    else
    {
        LCD_ShowIntNum(fumes_alarm_X_L_POS, fumes_alarm_Y_POS, d_fume_L_alarm, 3, fume_L_state_color, WHITE, 24);
    }

    if (d_fume_H_alarm < 10)
    {
        LCD_Fill(fumes_alarm_X_H_POS + 6, fumes_alarm_Y_POS, fumes_alarm_X_H_POS + 12, fumes_alarm_Y_POS + 24, WHITE);
        LCD_Fill(fumes_alarm_X_H_POS + 24, fumes_alarm_Y_POS, fumes_alarm_X_H_POS + 30, fumes_alarm_Y_POS + 24, WHITE);
        LCD_ShowIntNum(fumes_alarm_X_H_POS + 12, fumes_alarm_Y_POS, d_fume_H_alarm, 1, fume_H_state_color, WHITE, 24);
    }
    else if (d_fume_H_alarm < 100)
    {
        LCD_Fill(fumes_alarm_X_H_POS, fumes_alarm_Y_POS, fumes_alarm_X_H_POS + 6, fumes_alarm_Y_POS + 24, WHITE);
        LCD_Fill(fumes_alarm_X_H_POS + 30, fumes_alarm_Y_POS, fumes_alarm_X_H_POS + 36, fumes_alarm_Y_POS + 24, WHITE);
        LCD_ShowIntNum(fumes_alarm_X_H_POS + 6, fumes_alarm_Y_POS, d_fume_H_alarm, 2, fume_H_state_color, WHITE, 24);
    }
    else
    {
        LCD_ShowIntNum(fumes_alarm_X_H_POS, fumes_alarm_Y_POS, d_fume_H_alarm, 3, fume_H_state_color, WHITE, 24);
    }
}

#if 0
void _gui_Display(uint8_t *dht11, uint8_t *fumes)
{
    // OLED_ShowNum(temp_value_X_pos, temp_value_Y_pos, dht11[2], all_value_length, OLED_8X16);      // 温度值显示，整数两位
    // OLED_ShowNum(temp_value_X_pos + 20, temp_value_Y_pos, dht11[3], all_value_length, OLED_8X16); // 温度值显示，小数两位

    // OLED_ShowNum(humi_value_X_pos, humi_value_Y_pos, dht11[0], all_value_length, OLED_8X16);      // 湿度值显示，整数两位
    // OLED_ShowNum(humi_value_X_pos + 20, humi_value_Y_pos, dht11[1], all_value_length, OLED_8X16); // 湿度值显示，小数两位

    // OLED_ShowNum(fumes_value_X_pos, fumes_value_Y_pos, fumes[0], all_value_length, OLED_8X16);      // 烟雾浓度显示，整数两??
    // OLED_ShowNum(fumes_value_X_pos + 20, fumes_value_Y_pos, fumes[1], all_value_length, OLED_8X16); // 烟雾浓度显示，小数两??

    // OLED_Update();
    float f_fumes;
    f_fumes = fumes[0] + fumes[1] / 100.0;
    LCD_ShowFloatNum1(0, 30, f_fumes, 4, BLACK, WHITE, 16);
}
#endif