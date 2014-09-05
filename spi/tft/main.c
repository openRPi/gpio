#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "lib/lcd_lib.h"

int loop_run = 1;

void loop_stop(int sig)
{
	loop_run = 0;
}

int main()
{	
	printf("----------QVGA TFT----------\n\n");

	if(lcd_init()!=0)
	{
		printf("LCD init ERROR!\n");
		return 1;
	}
	
	

	lcd_exit();
	
	return 0;
}
