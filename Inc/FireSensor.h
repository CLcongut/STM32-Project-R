#ifndef FIRESENSOR_H
#define FIRESENSOR_H

#define FIRE_ALARM 0x66//Fire首字母F的ascii码值
#define FIRE_DIS 0x73  // Safe首字母S的ascii码值

uint8_t Fire_Sensor_Read(void);

#endif