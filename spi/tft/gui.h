#ifndef __GUI_H__
#define __GUI_H_

extern int GUI_init();

extern void GUI_draw_point(unsigned int x,unsigned int y, unsigned int point_color);
extern void GUI_fill(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end,unsigned int color);
extern void GUI_draw_line(unsigned int x_start, unsigned int y_start, unsigned int x_end, unsigned int y_end, unsigned int color);
extern void GUI_draw_rectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);
extern void GUI_draw_circle(unsigned int x0,unsigned int y0,unsigned char r, unsigned int color);
extern void GUI_show_pic(unsigned char n, unsigned int x, unsigned int y, unsigned int wide, unsigned int high);

extern void GUI_close();

#endif
