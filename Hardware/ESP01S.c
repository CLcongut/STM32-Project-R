#include "main.h"
#include "ESP01S.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "DHT11.h"
#include "LightSensor.h"
#include "RainSensor.h"

/*调试，只重复MQTT部分*/
// #define MQTT_ONLY

/********************************************************************************
 * 移植需求更改
 * 开始 >>
 */
/*开启串口DMA接收*/
#define ESP_Open_Receive HAL_UARTEx_ReceiveToIdle_DMA(&huart1, USART1_RX_BUF, USART_REC_LEN)
/*关闭串口DMA接收*/
#define ESP_Close_Receive HAL_UART_AbortReceive(&huart1)
/*串口发送*/
#define ESP_Transmit(_cmd) printf(_cmd)
/********************************************************************************
 * << 结束
 */

/********************************************************************************
 * 根据不同用户需要修改的部分
 * 开始 >>
 */
/*要让ESP01S连接的WIFI信息*/
#define wifi_SSID "RedmiK50"   // WIFI名字，不可中文
#define wifi_PSWD "1597638189" // WIFI密码，不可中文，最好 8 位

/*设备名称*/
#define DeviceName "SmartClothesRail"

/*MQTT三元组*/
#define mqtt_USERNAME "SmartClothesRail&a16KgIm5BAN"
#define mqtt_PASSWORD "0532b3e9e827549a7b11e03bb0b4f1f66a71b580acb7f9e2131794880765653e"
#define mqtt_CLIENTID "a16KgIm5BAN.SmartClothesRail|securemode=2\\,signmethod=hmacsha256\\,timestamp=1716476009339|" // 逗号前需要加两个反斜杠转义

/*MQTT域名*/
#define mqtt_HOST_URL "a16KgIm5BAN.iot-as-mqtt.cn-shanghai.aliyuncs.com"

/*发布主题*/
#define mqtt_pub_TOPIC "/sys/a16KgIm5BAN/" DeviceName "/thing/event/property/post"
/*订阅主题*/
#define mqtt_sub_TOPIC "/sys/a16KgIm5BAN/" DeviceName "/thing/service/property/set"

/*受控设备对象关键字*/
#define setClass_NAME "motorControl" // 电机

/*状态发送对象关键字*/
#define postClass_NAME1 "temperature"  // 温度
#define postClass_NAME2 "humidity"     // 湿度
#define postClass_NAME3 "brightness"   // 光照
#define postClass_NAME4 "raining"      // 雨滴
#define postClass_NAME5 "motorControl" // 电机
/********************************************************************************
 * << 结束
 */

/********************************************************************************
 * 根据应用需求修改的部分，皆为AT命令
 * 开始 >>
 */
#define cmd_RST "AT+RST\r\n"                                                                                   // 重启
#define cmd_REPLY "ATE0\r\n"                                                                                   // 关闭回显
#define cmd_CWQAP "AT+CWQAP\r\n"                                                                               // 关闭AP连接
#define cmd_CWJAPASK "AT+CWJAP?\r\n"                                                                           // 询问是否连接AP
#define cmd_MQTTCLEAN "AT+MQTTCLEAN=0\r\n"                                                                     // 关闭MQTT连接
#define cmd_MQTTCONNASK "AT+MQTTCONN?\r\n"                                                                     // 询问是否连接MQTT
#define cmd_CWMODE "AT+CWMODE=1\r\n"                                                                           // 切换模式1：STATION模式
#define cmd_AUTOCONN "AT+CWAUTOCONN=0\r\n"                                                                     // 关闭上电自动连接AP
#define cmd_CIPSNTPCFG "AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n"                           // 设置时区服务器
#define cmd_CWJAP "AT+CWJAP=\"" wifi_SSID "\",\"" wifi_PSWD "\"\r\n"                                           // 连接AP
#define cmd_MQTTUSERCFG "AT+MQTTUSERCFG=0,1,\"NULL\",\"" mqtt_USERNAME "\",\"" mqtt_PASSWORD "\",0,0,\"\"\r\n" // 设置MQTT用户名，密码
#define cmd_MQTTCLIENTID "AT+MQTTCLIENTID=0,\"" mqtt_CLIENTID "\"\r\n"                                         // 设置MQTT CLIENT ID
#define cmd_MQTTCONN "AT+MQTTCONN=0,\"" mqtt_HOST_URL "\",1883,1\r\n"                                          // 连接MQTT域名
/********************************************************************************
 * << 结束
 */

uint8_t tah_data[5]; // 温湿度缓存
uint8_t lig_data[2]; // 光照缓存

/***************************
 * 连接状态：
 * 0为WIFI和MQTT均未连接；
 * 1为连接WIFI但未连接MQTT；
 * 2为WIFI和MQTT均连接
 */
uint8_t Connect_state;

/**
 * @brief 内部函数，使用屏幕显示提示内容
 *
 * @param _info 需要显示的内容
 */
void gui_dp_info(const uint8_t *_info)
{
    LCD_Fill(0, 20, 128, 12, WHITE);
    LCD_ShowString(0, 20, _info, BLACK, WHITE, 12, 0);
}

/**
 * @brief 清除接收缓存
 *
 */
void ESP_RXBUF_Clear(void)
{
    memset(ESP_RXS.rx_data, 0x00, USART_REC_LEN);
}

/**
 * @brief 判断WIFI是否连接成功，若超时无法找到或连接WIFI，则重新尝试连接
 *
 */
void ESP_JudgeWIFI(void)
{
    /*全局结构体成员，使用也要创个变量来赋值，不可直接使用，更不能强制转换*/
    uint8_t *data_rx;
    uint16_t timeout;
    while (1)
    {
        data_rx = ESP_RXS.rx_data;
        /*判断接收到的是否是OK*/
        if (strstr((const char *)data_rx, "OK"))
        {
            Connect_state = 1;
            gui_dp_info("WIFI CONNECTED");
            HAL_Delay(1500);
            break;
        }
        else if (strstr((const char *)data_rx, "ERROR"))
        {
            Connect_state = 0;
            gui_dp_info("WIFI NOT FOUND");
            HAL_Delay(1500);
            ESP_RXBUF_Clear();
            /*嵌套初始化函数达到重新尝试连接的目的*/
            ESP_Init();
        }
        else if (timeout > 10000)
        {
            Connect_state = 0;
            gui_dp_info("WIFI FOUND TIMEOUT");
            HAL_Delay(1500);
            ESP_Init();
        }
        timeout++;
    }
    ESP_RXBUF_Clear();
}

/**
 * @brief 判断MQTT域名是否连接成功，若超时无法连接MQTT域名，则重新尝试连接
 *
 */
void ESP_JudgeMQTT(void)
{
    uint8_t *data_rx;
    uint16_t timeout;
    while (1)
    {
        data_rx = ESP_RXS.rx_data;
        if (strstr((const char *)data_rx, "OK"))
        {
            gui_dp_info("MQTT CONNECTED");
            HAL_Delay(1500);
            Connect_state = 2;
            break;
        }
        else if (strstr((const char *)data_rx, "ERROR"))
        {
            gui_dp_info("MQTT CONN FAIL");
            HAL_Delay(3000);
            Connect_state = 1;
            ESP_RXBUF_Clear();
            HAL_Delay(3000);
            ESP_Init();
        }
        else if (timeout > 10000)
        {
            Connect_state = 0;
            gui_dp_info("MQTT CONN TIMEOUT");
            HAL_Delay(1500);
            ESP_Init();
        }
        timeout++;
    }
    ESP_RXBUF_Clear();
}

/**
 * @brief 判断询问WIFI和MQTT是否连接
 *
 */
void ESP_Confirm_WIFIMQTT(void)
{
    gui_dp_info("START ASK");
    uint8_t *data_rx;
    HAL_Delay(1500);
    ESP_Transmit(cmd_CWJAPASK);
    HAL_Delay(100);
    while (1)
    {
        data_rx = ESP_RXS.rx_data;
        if (strstr((const char *)data_rx, "+CWJAP:"))
        {
            Connect_state = 1;
            gui_dp_info("WIFI WAS CONN");
            break;
        }
        else if (strstr((const char *)data_rx, "OK"))
        {
            Connect_state = 0;
            gui_dp_info("WIFI IS DISCONN");
            return;
        }
    }
    HAL_Delay(1500);

    ESP_Transmit(cmd_MQTTCONNASK);
    HAL_Delay(100);
    while (1)
    {
        data_rx = ESP_RXS.rx_data;
        if (strstr((const char *)data_rx, "1883"))
        {
            Connect_state = 2;
            ESP_RXS.rx_mqtt_en_flag = 1;
            gui_dp_info("MQTT WAS CONN");
            return;
        }
        else if (strstr((const char *)data_rx, "OK"))
        {
            Connect_state = 1;
            gui_dp_info("MQTT IS DISCONN");
            return;
        }
    }
}

/**
 * @brief 解析云平台发送的JSON数据
 *
 * @param jsond 要解析的数据
 * @param flagd 要找的对象关键字字符串
 * @return char
 */
char ESP_Parse_Json(uint8_t *jsond, const char *flagd)
{
    /*用来指向对象关键字的指针*/
    char *pflag;
    /*指向strstr()函数找到的字符串首字母位置*/
    pflag = strstr((const char *)jsond, flagd);
    /*根据分析JSON数据格式得出要找的对象的值的位置*/
    pflag += strlen(flagd) + 2;
    return *pflag;
}

/**
 * @brief ESP01S初始化函数，详细内容看函数内部注释
 *
 */
void ESP_Init(void)
{
    if (Connect_state == 0)
    {

#ifdef MQTT_ONLY

        HAL_Delay(1500);
        ESP_Transmit(cmd_MQTTCLEAN);
        HAL_Delay(1500);
        gui_dp_info("MQTT DISCONN");
        HAL_Delay(3000);
#endif
#ifndef MQTT_ONLY

        /**
         * @brief 延时稳定，关闭串口接收，准备
         *
         */
        HAL_Delay(1500);
        ESP_Close_Receive;
        gui_dp_info("READY FOR ESP01");

        /**
         * @brief 断开MQTT连接
         *
         */
        ESP_Transmit(cmd_MQTTCLEAN);
        HAL_Delay(1500);
        gui_dp_info("MQTT DISCONN");

        /**
         * @brief 断开WIFI连接
         *
         */
        ESP_Transmit(cmd_CWQAP);
        HAL_Delay(1500);
        gui_dp_info("WIFI DISCONN");

        /**
         * @brief 重启ESP01S
         *
         */
        ESP_Transmit(cmd_RST);
        gui_dp_info("RESET ESP01");
        HAL_Delay(3000);

        /**
         * @brief 切换模式 1 station模式
         *
         */
        ESP_Transmit(cmd_CWMODE);
        HAL_Delay(1500);
        gui_dp_info("SET ESP01 MODE");

        /**
         * @brief 关闭上电自动连接WIFI
         *
         */
        ESP_Transmit(cmd_AUTOCONN);
        HAL_Delay(1500);
        gui_dp_info("CLOSE AUTO CONN");

        /**
         * @brief 设置时区服务器
         *
         */
        ESP_Transmit(cmd_CIPSNTPCFG);
        HAL_Delay(1500);
        gui_dp_info("SET TIME ZONE");

        /**
         * @brief 设置WIFI名字和密码，并且连接
         *
         */
        ESP_Open_Receive;
        ESP_Transmit(cmd_CWJAP);
        HAL_Delay(1500);
        gui_dp_info("WIFI SET DONE");
        HAL_Delay(1500);
        ESP_JudgeWIFI();
    }
    if (Connect_state == 1)
    {
#endif
        /**
         * @brief 设定MQTT用户名和密钥
         *
         */
        ESP_Close_Receive;
        HAL_Delay(1500);
        ESP_Transmit(cmd_MQTTUSERCFG);
        HAL_Delay(2000);
        gui_dp_info("SET MQTT USERCFG");

        /**
         * @brief 设定MQTT CLINT ID
         *
         */
        ESP_Transmit(cmd_MQTTCLIENTID);
        HAL_Delay(2000);
        gui_dp_info("SET MQTT CLINTID");

        /**
         * @brief 设定并连接MQTT域名
         *
         */
        ESP_Open_Receive;
        ESP_Transmit(cmd_MQTTCONN);
        HAL_Delay(4000);
        gui_dp_info("SET MQTT URL");
        HAL_Delay(4000);
        ESP_JudgeMQTT();
    }
    if (Connect_state == 2)
    {
        /**
         * @brief 订阅MQTT主题
         *
         */
        ESP_Transmit("AT+MQTTSUB=0,\"" mqtt_sub_TOPIC "\",1\r\n");
        HAL_Delay(3000);
        gui_dp_info("TOPIC SUB");
        ESP_RXS.rx_mqtt_en_flag = 1;
    }
}

void ESP_MQTT_Trans_Data(uint8_t numS)
{
    float temp, humi;
    uint8_t railpos;
    uint8_t rainsta;
    if (DHT_Read(tah_data) && Light_Read(lig_data))
    {
        temp = tah_data[2] + tah_data[3] / 100.0;
        humi = tah_data[0] + tah_data[1] / 100.0;
        railpos = cur_rail_state ? 1 : 2;
        rainsta = Rain_Sensor_Read();

        switch (numS)
        {
        case 0:
            printf("AT+MQTTPUB=0,\"" mqtt_pub_TOPIC "\",\"{\\\"params\\\":{\\\"" postClass_NAME1 "\\\":%f\\,             \
                                                                        \\\"" postClass_NAME2 "\\\":%f}}\",0,0\r\n",
                   temp, humi);
            break;

        case 1:
            printf("AT+MQTTPUB=0,\"" mqtt_pub_TOPIC "\",\"{\\\"params\\\":{\\\"" postClass_NAME3 "\\\":%d\\,             \
                                                                        \\\"" postClass_NAME4 "\\\":%d}}\",0,0\r\n",
                   lig_data[1], rainsta);
            break;

        case 2:
            printf("AT+MQTTPUB=0,\"" mqtt_pub_TOPIC "\",\"{\\\"params\\\":{\\\"" postClass_NAME5 "\\\":%d}}\",0,0\r\n", railpos);
            break;
        }

        ESP_RXBUF_Clear();
    }
}

void ESP_MQTT_Recei_Data(void)
{
    if (ESP_RXS.rx_mqtt_en_flag)
    {
        uint8_t *data_rx;
        data_rx = ESP_RXS.rx_data;
        uint8_t json_body[128];
        int strlen;
        if (strstr((const char *)data_rx, "+MQTTSUBRECV:"))
        {
            sscanf((const char *)data_rx, "+MQTTSUBRECV:0,\"" mqtt_sub_TOPIC "\",%d,%s", &strlen, json_body);
            if (ESP_Parse_Json(json_body, setClass_NAME) == '1')
            {
                set_rail_state = 1;
            }
            else if (ESP_Parse_Json(json_body, setClass_NAME) == '2')
            {
                set_rail_state = 0;
            }
            ESP_RXBUF_Clear();
        }
    }
}