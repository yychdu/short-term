#include "ssd1306_i2c.h"
#define OLED_SCL		GPIO_PIN_10
#define OLED_SDA		GPIO_PIN_11
#define OLED_GPIO	GPIOB

#define OLED_SCL_H         HAL_GPIO_WritePin(OLED_GPIO,OLED_SCL,GPIO_PIN_SET)
#define OLED_SCL_L         HAL_GPIO_WritePin(OLED_GPIO, OLED_SCL,GPIO_PIN_RESET)

#define OLED_SDA_H         HAL_GPIO_WritePin(OLED_GPIO, OLED_SDA,GPIO_PIN_SET)
#define OLED_SDA_L         HAL_GPIO_WritePin(OLED_GPIO, OLED_SDA,GPIO_PIN_RESET)

#define OLED_SCL_read      HAL_GPIO_ReadPin(OLED_GPIO, OLED_SCL)
#define OLED_SDA_read      HAL_GPIO_ReadPin(OLED_GPIO, OLED_SDA)

void OLED_IIC_Start(void)
{
	OLED_SCL_H;
	OLED_SDA_H;
	OLED_SDA_L;
	OLED_SCL_L;
}
void OLED_IIC_Stop(void)
{
	OLED_SCL_L;
	OLED_SDA_L;
	OLED_SDA_H;
	OLED_SCL_H;	
}
void OLED_IIC_SendByte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		OLED_SCL_L;
		if(dat&0x80)
			OLED_SDA_H;
		else
			OLED_SDA_L;
		dat<<=1;
		OLED_SCL_H;
			
	}
	OLED_SCL_L;
	OLED_SCL_H;
}
void IIC_delay_ms(u16 time)
{
	for(;time;time--)
	{
//		for(i=0;i<5000;i++);
    HAL_Delay(1);
	}
}
