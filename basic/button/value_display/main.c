/*
 *	main.c 循环检测GPIO（按键）输入电平
 *	
 *	Copyright (C) 2014 concefly <h.wenjian@openrpi.org>
 *	Copyright (C) 2014 openRPi
 *	
 *		代码遵循GNU协议
 *	
 *	文档：http://www.openrpi.org/blogs/?p=232
 */

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
		
	/**
	 * 设置SIGINT信号。CTRL-C 发出SIGINT信号。
	 */
	signal(SIGINT,loop_stop);
	
	printf("Checking button,press Ctrl-C to exit...\n");
	while(loop_run)
	{
		printf("\rbtn = %d",button_value());
	}
	printf("Exit\n");

	button_end();
	return 0;
}
