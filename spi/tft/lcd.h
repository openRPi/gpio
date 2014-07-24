#ifndef __LCD_H__
#define __LCD_H__

#define LCD_W 240
#define LCD_H 320

#define RED 0xf800
#define GREEN 0x07e0
#define BLUE 0x001f

extern int LCD_init(void);
 
extern void LCD_wr_data8(uint8_t da);
extern void LCD_wr_data(uint16_t da);
extern void LCD_wr_reg(uint16_t da);
extern void LCD_set_addr(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
extern void LCD_clear(uint16_t color);

extern void LCD_close(void);


#endif
