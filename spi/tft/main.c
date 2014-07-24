#include <stdio.h>
#include <signal.h>
#include <time.h>
#include "gui.h"

int loop_run = 1;

void loop_stop(int sig)
{
	loop_run = 0;
}

int main()
{	
	printf("----------QVGA TFT----------\n\n");

	if(!GUI_init())
	{
		printf("GUI init ERROR!\n");
		return 1;
	}
	
	signal(SIGINT, loop_stop);
	
	while(loop_run)
	{
		GUI_show_pic(0, 0, 0, 240, 320);
		sleep(5);
		GUI_show_pic(1, 0, 0, 240, 320);
		sleep(5);
	}

	GUI_close();	
	
	return 0;
}
