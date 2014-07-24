#include "gui.h"

void GUI_DrawPoint(uint16_t x,uint16_t y, uint16_t color)
{
	uint8_t i;
	
	LCD_AddressSet(x-1,y-1,x+1,y+1);
	
	for(i=0;i<9;i++)
	{
		LCD_WR_DATA(color); 
	}	    
}

void GUI_Fill(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end,uint16_t color)
{          
	uint16_t i,j; 
	LCD_AddressSet(x_start,y_start,x_end,y_end);      
	for(i=y_start;i<=y_end;i++)
	{													   	 	
		for(j=x_start;j<=x_end;j++)
			LCD_WR_DATA(color);	    
	} 					  	    
} 

void GUI_DrawLine(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color)
{
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy;
	uint16_t row, col;
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
	    GUI_DrawPoint(col, row, color);
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

void GUI_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	GUI_DrawLine(x1,y1,x2,y1,color);
	GUI_DrawLine(x1,y1,x1,y2,color);
	GUI_DrawLine(x1,y2,x2,y2,color);
	GUI_DrawLine(x2,y1,x2,y2,color);
}

void GUI_DrawCircle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);            
	while(a<=b)
	{
		GUI_DrawPoint(x0-b,y0-a,color);                       
		GUI_DrawPoint(x0+b,y0-a,color);                        
		GUI_DrawPoint(x0-a,y0+b,color);                   
		GUI_DrawPoint(x0-b,y0-a,color);                      
		GUI_DrawPoint(x0-a,y0-b,color);                        
		GUI_DrawPoint(x0+b,y0+a,color);                           
		GUI_DrawPoint(x0+a,y0-b,color);             
		GUI_DrawPoint(x0+a,y0+b,color);             
		GUI_DrawPoint(x0-b,y0+a,color);            
		a++;
		 
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		GUI_DrawPoint(x0+a,y0+b,color);
	}
}

void GUI_ShowPicture1(uint16_t x, uint16_t y, uint16_t wide, uint16_t high)
{
	uint32_t temp = 0, tmp = 0, num = 0;
	LCD_AddressSet(x, y, x+wide-1, y+high-1);
	num = wide * high * 2;
	do
	{
		temp = pic_1[tmp + 1];
		temp = temp << 8;
		temp = temp | pic_1[tmp];

		LCD_WR_DATA(temp);

		tmp += 2;
	}
	while(tmp < num);
}

void GUI_ShowPicture2(uint16_t x, uint16_t y, uint16_t wide, uint16_t high)
{
	uint32_t temp = 0, tmp = 0, num = 0;
	LCD_AddressSet(x, y, x+wide-1, y+high-1);
	num = wide * high * 2;
	do
	{
		temp = pic_2[tmp + 1];
		temp = temp << 8;
		temp = temp | pic_2[tmp];

		LCD_WR_DATA(temp);

		tmp += 2;
	}
	while(tmp < num);
}
