#ifndef __LCD_H__
#define __LCD_H__

#define LCD_W 240
#define LCD_H 320

#define RED 0xf800
#define GREEN 0x07e0
#define BLUE 0x001f

extern int LCD_init(void);
 
extern void LCD_wr_data8(unsigned char da);
extern void LCD_wr_data(unsigned int da);
extern void LCD_wr_reg(unsigned int da);
extern void LCD_set_addr(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
extern void LCD_clear(unsigned int color);

extern void LCD_close(void);


#endif
