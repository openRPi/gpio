#include "pcf8574.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int loop_run = 1;

void loop_stop(int sig)
{
	loop_run = 0;
}

int main()
{
	char data = 0x01;

	if(!pcf8574_start())
		printf("init error!");

	printf("*************PCF8574-flow leds**************\n\n");

	signal(SIGINT, loop_stop);

	while(loop_run)
	{
		data *= 2;
		pcf8574_write_byte(data);
		if(data == 0x80)
			data = 0x01;
		sleep(1);
	}

	pcf8574_stop();

	return 0;

}
