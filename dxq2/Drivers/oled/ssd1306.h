#ifndef _SSD1306_H
#define _SSD1306_H
#include <stdlib.h>
#include <string.h>
#include "ssd1306_i2c.h"
//#include "stm32f1xx_hal.h"

#include "gui.h"
#define OLED_CMD  0X00 //Ð´ÃüÁî
#define OLED_DATA 0X40 //Ð´Êý¾Ý
#define SSD1306_I2C_ADDR         0x78
#define SSD1306_WIDTH            128
#define SSD1306_HEIGHT           64
void SSD1306_Fill(GUI_COLOR color);
void SSD1306_UpdateScreen(void);
void SSD1306_WriteByte(uint8_t dat,uint8_t cmd);
void SSD1306_WriteMuliByte(uint8_t* data,uint16_t count,uint8_t cmd);
void SSD1306_init(void);
void SSD1306_ToggleInvert(void);
void SSD1306_DrawPixel(uint16_t x, uint16_t y, GUI_COLOR color);
void SSD1306_GotoXY(uint16_t x, uint16_t y) ;
void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR c);
void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, GUI_COLOR c);
void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, GUI_COLOR c);
void SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, GUI_COLOR color);
void SSD1306_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, GUI_COLOR color);
void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, GUI_COLOR c);
void SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, GUI_COLOR c);
void SSD1306_ON(void);
void SSD1306_OFF(void);

#endif

