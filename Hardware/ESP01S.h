#ifndef ESP01S_H
#define ESP01S_H

void ESP_Init(void);
void ESP_MQTT_Trans_Data(uint8_t numS);
void ESP_MQTT_Recei_Data(void);
void ESP_Confirm_WIFIMQTT(void);

#endif

#if 0
{
    "id": "123",
    "version": "1.0",
    "sys":{
        "ack":0
    },
    "params": {
        "properties": {
            "Power": {
                "value": "on",
                "time": 1524448722000
            },
            "WF": {
                "value": { },
                "time": 1524448722000
            }
        },
        "events": {
            "alarmEvent1": {
                "value": {
                    "param1": "on",
                    "param2": "2"
                },
                "time": 1524448722000
            },
            "alertEvent2": {
                "value": {
                    "param1": "on",
                    "param2": "2"
                },
                "time": 1524448722000
            }
        },
        "subDevices": [
            {
                "identity": {
                    "productKey": "",
                    "deviceName": ""
                },
                "properties": {
                    "Power": {
                        "value": "on",
                        "time": 1524448722000
                    },
                    "WF": {
                        "value": { },
                        "time": 1524448722000
                    }
                },
                "events": {
                    "alarmEvent1": {
                        "value": {
                            "param1": "on",
                            "param2": "2"
                        },
                        "time": 1524448722000
                    },
                    "alertEvent2": {
                        "value": {
                            "param1": "on",
                            "param2": "2"
                        },
                        "time": 1524448722000
                    }
                }
            }
        ]
    },
    "method": "thing.event.property.pack.post"
}

printf("AT+MQTTPUB=0,\"" mqtt_pub_TOPIC "\",\"{\\\"params\\\":{\\\"" postClass_NAME1 "\\\":%f\\,             \
                                                                \\\"" postClass_NAME2 "\\\":%f\\,            \
                                                                \\\"" postClass_NAME3 "\\\":%d\\,            \
                                                                \\\"" postClass_NAME4 "\\\":%d\\,            \
                                                                \\\"" postClass_NAME5 "\\\":%d}}\",0,0\r\n",
        temp, humi, lig_data[1], rainsta, railpos);

#endif