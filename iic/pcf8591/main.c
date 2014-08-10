#include "pcf8591.h"
#include <signal.h>
#include <stdio.h>

int loop_run = 1;

void loop_stop(int sig)
{
	loop_run = 0;
}

int main()
{
	float value;

	printf("--------------------PCF8591-------------------\n\n");

	signal(SIGINT, loop_stop);
	
	if(!pcf8591_start())
	{
		printf("INIT ERROR!");
		return 1;
	}

	printf("the value of channel_0 is:\n");

	pcf8591_start();
	while(loop_run)
	{
		value = pcf8591_value(CHANNEL_0);
		printf("\r%.3f V", value);
	}
	pcf8591_stop();

	printf("\nEXIT SUCCESS!\n");
   	
	return 0;
}

