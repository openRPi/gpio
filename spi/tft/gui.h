#ifndef __GUI_H__
#define __GUI_H_

extern int GUI_init();

extern void GUI_draw_point(uint16_t x,uint16_t y, uint16_t point_color);
extern void GUI_fill(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end,uint16_t color);
extern void GUI_draw_line(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);
extern void GUI_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
extern void GUI_draw_circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color);
extern void GUI_show_pic(uint8_t n, uint16_t x, uint16_t y, uint16_t wide, uint16_t high);

extern void GUI_close();

#endif
