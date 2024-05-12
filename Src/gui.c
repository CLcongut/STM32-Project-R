#include "main.h"
#include "gui.h"

#include "OLED.h"

#define temp_value_X_pos 23 // 温度值X坐标
#define temp_value_Y_pos 8  // 温度值Y坐标

#define humi_value_X_pos 23 // 湿度值X坐标
#define humi_value_Y_pos 40 // 湿度值Y坐标

#define fumes_value_X_pos 75 // 烟雾值X坐标
#define fumes_value_Y_pos 30 // 烟雾值Y坐标

#define all_value_length 2 // 所有数字显示位数

void gui_Load(void)
{
    OLED_DrawRectangle(0, 0, 128, 64, OLED_UNFILLED); // 外框
    OLED_DrawLine(63, 1, 63, 63);                     // 中间竖线
    OLED_DrawLine(1, 32, 63, 32);                     // 中间横线

    OLED_ShowString(5, 8, "T:", OLED_8X16);      // 温度提示
    OLED_ShowString(5, 40, "H:", OLED_8X16);     // 湿度提示
    OLED_ShowString(70, 8, "Fumes:", OLED_8X16); // 烟雾提示

    OLED_ShowChar(temp_value_X_pos + 16, temp_value_Y_pos, '.', OLED_8X16);   // 温度小数点
    OLED_ShowChar(humi_value_X_pos + 16, humi_value_Y_pos, '.', OLED_8X16);   // 湿度小数点
    OLED_ShowChar(fumes_value_X_pos + 16, fumes_value_Y_pos, '.', OLED_8X16); // 烟雾小数点

    OLED_Update();
}

void gui_Display(uint8_t *dht11, uint8_t *fumes)
{
    OLED_ShowNum(temp_value_X_pos, temp_value_Y_pos, dht11[2], all_value_length, OLED_8X16);      // 温度值显示，整数两位
    OLED_ShowNum(temp_value_X_pos + 20, temp_value_Y_pos, dht11[3], all_value_length, OLED_8X16); // 温度值显示，小数两位

    OLED_ShowNum(humi_value_X_pos, humi_value_Y_pos, dht11[0], all_value_length, OLED_8X16);      // 湿度值显示，整数两位
    OLED_ShowNum(humi_value_X_pos + 20, humi_value_Y_pos, dht11[1], all_value_length, OLED_8X16); // 湿度值显示，小数两位

    OLED_ShowNum(fumes_value_X_pos, fumes_value_Y_pos, fumes[0], all_value_length, OLED_8X16);      // 烟雾浓度显示，整数两位
    OLED_ShowNum(fumes_value_X_pos + 20, fumes_value_Y_pos, fumes[1], all_value_length, OLED_8X16); // 烟雾浓度显示，小数两位

    OLED_Update();
}