#include "gui.h"
#include "lcd.h"
#include "pic.h"
#include "bcm2835.h"

void GUI_draw_point(unsigned int x,unsigned int y, unsigned int color)
{
	unsigned char i;
	
	LCD_set_addr(x-1,y-1,x+1,y+1);
	
	for(i=0;i<9;i++)
	{
		LCD_wr_data(color); 
	}	    
}

void GUI_fill(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end,unsigned int color)
{          
	unsigned int i,j; 
	LCD_set_addr(x_start,y_start,x_end,y_end);      
	for(i=y_start;i<=y_end;i++)
	{													   	 	
		for(j=x_start;j<=x_end;j++)
			LCD_wr_data(color);	    
	} 					  	    
} 

void GUI_draw_line(unsigned int x_start, unsigned int y_start, unsigned int x_end, unsigned int y_end, unsigned int color)
{
	unsigned int t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy;
	unsigned int row, col;
	delta_x = x_end - x_start;
	delta_y = y_end - y_start;
	col = x_start;
	row = y_start;
	if (delta_x > 0)
	{
		incx=1;
	}
	else
	{
	    if (delta_x == 0)
		{
			incx = 0;
		}
	    else
			{
				incx = -1;
				delta_x = -delta_x;
			}
	}
	if (delta_y > 0)
	{
		incy = 1;
	}
	else
	{
	    if (delta_y == 0)
		{
			incy = 0;
		}
	    else
		{
			incy = -1;
			delta_y = -delta_y;
		}
	}
	if (delta_x > delta_y)
	{
		distance = delta_x;
	}
	else
	{
		distance = delta_y;
	}
	for (t=0; t<=distance+1; t++)
	{                                   
	    GUI_draw_point(col, row, color);
	    xerr += delta_x;
	    yerr += delta_y;
	  	if(xerr > distance)
	    {
	        xerr -= distance;
	        col += incx;
	    }
	    if(yerr > distance)
	    {
	        yerr -= distance;
	        row += incy;
	    }
	}
}

void GUI_draw_rectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color)
{
	GUI_draw_line(x1,y1,x2,y1,color);
	GUI_draw_line(x1,y1,x1,y2,color);
	GUI_draw_line(x1,y2,x2,y2,color);
	GUI_draw_line(x2,y1,x2,y2,color);
}

void GUI_draw_circle(unsigned int x0,unsigned int y0,unsigned char r, unsigned int color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);            
	while(a<=b)
	{
		GUI_draw_point(x0-b,y0-a,color);                       
		GUI_draw_point(x0+b,y0-a,color);                        
		GUI_draw_point(x0-a,y0+b,color);                   
		GUI_draw_point(x0-b,y0-a,color);                      
		GUI_draw_point(x0-a,y0-b,color);                        
		GUI_draw_point(x0+b,y0+a,color);                           
		GUI_draw_point(x0+a,y0-b,color);             
		GUI_draw_point(x0+a,y0+b,color);             
		GUI_draw_point(x0-b,y0+a,color);            
		a++;
		 
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		GUI_draw_point(x0+a,y0+b,color);
	}
}

void GUI_show_pic(unsigned char n, unsigned int x, unsigned int y, unsigned int wide, unsigned int high)
{
	uint32_t temp = 0, tmp = 0, num = 0;

	if(n>=PIC_MAX)
		return ;

	LCD_set_addr(x, y, x+wide-1, y+high-1);
	num = wide * high * 2;
	do
	{
		temp = pic_table[n][tmp + 1];
		temp = temp << 8;
		temp = temp | pic_table[n][tmp];

		LCD_wr_data(temp);

		tmp += 2;
	}
	while(tmp < num);
}

int GUI_init(void)
{
	return LCD_init();
}

void GUI_close(void)
{
	LCD_close();
}
