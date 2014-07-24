#include "gui.h"
#include <signal.h>

int loop_run = 1;

void loop_stop(int sig)
{
	loop_run = 0;
}

int main()
{	
	LCD_Init();
	
	signal(SIGINT, loop_stop);
	
/*	LCD_Clear(GREEN);
	GUI_DrawPoint(120, 160, RED);
	GUI_Fill(60, 170, 180, 190, BLUE );
	GUI_DrawLine(20, 40, 100, 120, RED);
	GUI_DrawRectangle(30, 50, 70, 100, BLUE);
	GUI_DrawCircle(100, 150, 20, RED);*/
	
	while(loop_run)
	{
		GUI_ShowPicture1(0, 0, 240, 320);
		bcm2835_delay(5000);
		GUI_ShowPicture2(0, 0, 240, 320);
		bcm2835_delay(5000);
	}
	printf("Init sucess!!!");
	LCD_end();
	
	
	return 0;
}
