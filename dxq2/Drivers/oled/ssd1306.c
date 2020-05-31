#include "ssd1306.h"
/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))
/* SSD1306 data buffer */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
/* Private SSD1306 structure */
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

/* Private variable */
static SSD1306_t SSD1306;

void SSD1306_WriteByte(uint8_t dat, uint8_t cmd)
{
	OLED_IIC_Start();
	OLED_IIC_SendByte(SSD1306_I2C_ADDR);
	OLED_IIC_SendByte(cmd);
	OLED_IIC_SendByte(dat);
	OLED_IIC_Stop();
//  HAL_I2C_Mem_Write(&hi2c2, SSD1306_I2C_ADDR, cmd, I2C_MEMADD_SIZE_8BIT, &dat, 1, 100);
}
void SSD1306_WriteMuliByte(uint8_t* data, uint16_t count, uint8_t cmd)
{
	uint16_t i;
	OLED_IIC_Start();
	OLED_IIC_SendByte(SSD1306_I2C_ADDR);
	OLED_IIC_SendByte(cmd);
	for(i=0;i<count;i++)
	{
		OLED_IIC_SendByte(data[i]);
	}
	OLED_IIC_Stop();
//  HAL_I2C_Mem_Write(&hi2c2, SSD1306_I2C_ADDR, cmd, I2C_MEMADD_SIZE_8BIT, data, count, 100);
}
//³õÊ¼»¯SSD1306
void SSD1306_init()
{
  SSD1306_WriteByte(0xAE,OLED_CMD); //display off
  SSD1306_WriteByte(0x20,OLED_CMD); //Set Memory Addressing Mode   
  SSD1306_WriteByte(0x10,OLED_CMD); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  SSD1306_WriteByte(0xB0,OLED_CMD); //Set Page Start Address for Page Addressing Mode,0-7
  SSD1306_WriteByte(0xC8,OLED_CMD); //Set COM Output Scan Direction
  SSD1306_WriteByte(0x00,OLED_CMD); //---set low column address
  SSD1306_WriteByte(0x10,OLED_CMD); //---set high column address
  SSD1306_WriteByte(0x40,OLED_CMD); //--set start line address
  SSD1306_WriteByte(0x81,OLED_CMD); //--set contrast control register
  SSD1306_WriteByte(0xFF,OLED_CMD);
  SSD1306_WriteByte(0xA1,OLED_CMD); //--set segment re-map 0 to 127
  SSD1306_WriteByte(0xA6,OLED_CMD); //--set normal display
  SSD1306_WriteByte(0xA8,OLED_CMD); //--set multiplex ratio(1 to 64)
  SSD1306_WriteByte(0x3F,OLED_CMD); //
  SSD1306_WriteByte(0xA4,OLED_CMD); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  SSD1306_WriteByte(0xD3,OLED_CMD); //-set display offset
  SSD1306_WriteByte(0x00,OLED_CMD); //-not offset
  SSD1306_WriteByte(0xD5,OLED_CMD); //--set display clock divide ratio/oscillator frequency
  SSD1306_WriteByte(0xF0,OLED_CMD); //--set divide ratio
  SSD1306_WriteByte(0xD9,OLED_CMD); //--set pre-charge period
  SSD1306_WriteByte(0x22,OLED_CMD); //
  SSD1306_WriteByte(0xDA,OLED_CMD); //--set com pins hardware configuration
  SSD1306_WriteByte(0x12,OLED_CMD);
  SSD1306_WriteByte(0xDB,OLED_CMD); //--set vcomh
  SSD1306_WriteByte(0x20,OLED_CMD); //0x20,0.77xVcc
  SSD1306_WriteByte(0x8D,OLED_CMD); //--set DC-DC enable
  SSD1306_WriteByte(0x14,OLED_CMD); //
  SSD1306_WriteByte(0xAF,OLED_CMD); //--turn on SSD1306 panel
  
  SSD1306_Fill(GUI_COLOR_BLACK);
  
  SSD1306_UpdateScreen();
  SSD1306.CurrentX = 0;
  SSD1306.CurrentY = 0;
  
  /* Initialized OK */
  SSD1306.Initialized = 1;
}

void SSD1306_UpdateScreen(void)
{
	uint8_t m;
	for(m=0;m<8;m++)
	{
		SSD1306_WriteByte(0xb0+m,OLED_CMD);
		SSD1306_WriteByte(0X00,OLED_CMD);
		SSD1306_WriteByte(0X10,OLED_CMD);
		SSD1306_WriteMuliByte(&SSD1306_Buffer[SSD1306_WIDTH*m],SSD1306_WIDTH,OLED_DATA);
	}
}

void SSD1306_Fill(GUI_COLOR color) {
	/* Set memory */
	memset(SSD1306_Buffer, (color == GUI_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

void SSD1306_ToggleInvert(void) {
	uint16_t i;
	
	/* Toggle invert */
	SSD1306.Inverted = !SSD1306.Inverted;
	
	/* Do memory toggle */
	for (i = 0; i < sizeof(SSD1306_Buffer); i++) {
		SSD1306_Buffer[i] = ~SSD1306_Buffer[i];
	}
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, GUI_COLOR color) {
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Error */
		return;
	}
	
	/* Check if pixels are inverted */
	if (SSD1306.Inverted) {
		color = (GUI_COLOR)!color;
	}
	
	/* Set color */
	if (color == GUI_COLOR_WHITE) {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} else {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

void SSD1306_GotoXY(uint16_t x, uint16_t y) {
	/* Set write pointers */
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR c) {
	int16_t dx, dy, sx, sy, err, e2, i, tmp; 
	
	/* Check for overflow */
	if (x0 >= SSD1306_WIDTH) {
		x0 = SSD1306_WIDTH - 1;
	}
	if (x1 >= SSD1306_WIDTH) {
		x1 = SSD1306_WIDTH - 1;
	}
	if (y0 >= SSD1306_HEIGHT) {
		y0 = SSD1306_HEIGHT - 1;
	}
	if (y1 >= SSD1306_HEIGHT) {
		y1 = SSD1306_HEIGHT - 1;
	}
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			SSD1306_DrawPixel(x0, i, c);
		}
		
		/* Return from function */
		return;
	}
	
	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			SSD1306_DrawPixel(i, y0, c);
		}
		
		/* Return from function */
		return;
	}
	
	while (1) {
		SSD1306_DrawPixel(x0, y0, c);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err; 
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		} 
	}
}

void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, GUI_COLOR c) {
	/* Check input parameters */
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH) {
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) {
		h = SSD1306_HEIGHT - y;
	}
	
	/* Draw 4 lines */
	SSD1306_DrawLine(x, y, x + w, y, c);         /* Top line */
	SSD1306_DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
	SSD1306_DrawLine(x, y, x, y + h, c);         /* Left line */
	SSD1306_DrawLine(x + w, y, x + w, y + h, c); /* Right line */
}

void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, GUI_COLOR c) {
	uint8_t i;
	
	/* Check input parameters */
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH) {
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) {
		h = SSD1306_HEIGHT - y;
	}
	
	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		SSD1306_DrawLine(x, y + i, x + w, y + i, c);
	}
}

void SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, GUI_COLOR color) {
	/* Draw lines */
	SSD1306_DrawLine(x1, y1, x2, y2, color);
	SSD1306_DrawLine(x2, y2, x3, y3, color);
	SSD1306_DrawLine(x3, y3, x1, y1, color);
}


void SSD1306_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, GUI_COLOR color) {
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;
	
	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay){
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		SSD1306_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, GUI_COLOR c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    SSD1306_DrawPixel(x0, y0 + r, c);
    SSD1306_DrawPixel(x0, y0 - r, c);
    SSD1306_DrawPixel(x0 + r, y0, c);
    SSD1306_DrawPixel(x0 - r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawPixel(x0 + x, y0 + y, c);
        SSD1306_DrawPixel(x0 - x, y0 + y, c);
        SSD1306_DrawPixel(x0 + x, y0 - y, c);
        SSD1306_DrawPixel(x0 - x, y0 - y, c);

        SSD1306_DrawPixel(x0 + y, y0 + x, c);
        SSD1306_DrawPixel(x0 - y, y0 + x, c);
        SSD1306_DrawPixel(x0 + y, y0 - x, c);
        SSD1306_DrawPixel(x0 - y, y0 - x, c);
    }
}

void SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, GUI_COLOR c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    SSD1306_DrawPixel(x0, y0 + r, c);
    SSD1306_DrawPixel(x0, y0 - r, c);
    SSD1306_DrawPixel(x0 + r, y0, c);
    SSD1306_DrawPixel(x0 - r, y0, c);
    SSD1306_DrawLine(x0 - r, y0, x0 + r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, c);
        SSD1306_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, c);

        SSD1306_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, c);
        SSD1306_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}

void SSD1306_ON(void) {
	SSD1306_WriteByte(0x8D,OLED_CMD);  
	SSD1306_WriteByte(0x14,OLED_CMD);  
	SSD1306_WriteByte(0xAF,OLED_CMD);  
}
void SSD1306_OFF(void) {
	SSD1306_WriteByte(0x8D,OLED_CMD);  
	SSD1306_WriteByte(0x10,OLED_CMD);
	SSD1306_WriteByte(0xAE,OLED_CMD);  
}
