#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "lcd_lib.h"
#include "pic.h"

int loop_run = 1;

void loop_stop(int sig)
{
	loop_run = 0;
}

int main()
{	
	printf("----------HVGA TFT----------\n\n");

	if(lcd_init()!=0)
	{
		printf("LCD init ERROR!\n");
		return 1;
	}

	lcd_hard_reset();
	lcd_normal_config();
	
	lcd_memory_write(pic_table[0],LCD_WIDTH_MAX * LCD_HEIGHT_MAX * 2,0);

	lcd_exit();
	
	return 0;
}
