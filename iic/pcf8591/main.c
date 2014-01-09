/*
 *	main.c PCF8591 检测输入的模拟量
 *	
 *	Copyright (C) 2014 w.guihong <w.guihong@openrpi.org>
 *	Copyright (C) 2014 openRPi
 *	
 *		代码遵循GNU协议
 *	
 *	文档：
 */

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
		value = pcf8591_value(PCF8591_CHANNEL_0);
		printf("\r%.3f V", value);
	}
	pcf8591_stop();

	printf("\nEXIT SUCCESS!\n");
   	
	return 0;
}

