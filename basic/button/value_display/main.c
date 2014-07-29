#include <stdio.h>
#include <signal.h>
#include "button.h"

int loop_run = 1;

void loop_stop(int sig)
{
	loop_run = 0;
}

int main()
{
	if(!button_init())
		return 1;
		
	// Set the signal
	signal(SIGINT,loop_stop); // Ctrl-C
	
	printf("Checking button,press Ctrl-C to exit...\n");
	while(loop_run)
	{
		printf("\rbtn = %d",button_value());
	}
	printf("Exit\n");

	button_end();
	return 0;
}
