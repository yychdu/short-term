#ifndef _SSD1306_I2C_H
#define _SSD1306_I2C_H
#include "stm32f1xx_hal.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
void OLED_IIC_init(void);
void OLED_IIC_Start(void);
void OLED_IIC_Stop(void);
void OLED_IIC_SendByte(u8 dat);
void IIC_delay_ms(u16 time);
#endif
