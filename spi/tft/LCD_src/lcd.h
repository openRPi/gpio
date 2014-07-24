#ifndef __LCD_H__
#define __LCD_H__

#include "bcm2835.h"
#include <stdio.h>

#define TFT_RST RPI_V2_GPIO_P1_11
#define TFT_DC	RPI_V2_GPIO_P1_12 
#define TFT_CS  RPI_V2_GPIO_P1_24

#define LCD_W 240
#define LCD_H 320

#define RED 0xf800
#define GREEN 0x07e0
#define BLUE 0x001f

void LCD_WR_DATA8(uint8_t da);
void LCD_WR_DATA(uint16_t da);
void LCD_WR_REG(uint16_t da);
void LCD_AddressSet(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void LCD_Init(void);
void LCD_end(void);
void LCD_Clear(uint16_t color);

#endif
