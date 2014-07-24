#ifndef __GUI_H__
#define __GUI_H_

#include "lcd.h"
#include "pic1.h"
#include "pic2.h"

void GUI_DrawPoint(uint16_t x,uint16_t y, uint16_t point_color);
void GUI_Fill(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end,uint16_t color);
void GUI_DrawLine(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);
void GUI_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void GUI_DrawCircle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color);
void GUI_ShowPicture1(uint16_t x, uint16_t y, uint16_t wide, uint16_t high);
void GUI_ShowPicture2(uint16_t x, uint16_t y, uint16_t wide, uint16_t high);

#endif
