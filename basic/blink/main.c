/*
 *	main.c 点亮一盏LED
 *	
 *	Copyright (C) 2014 concefly <h.wenjian@openrpi.org>
 *	Copyright (C) 2014 openRPi
 *	
 *		代码遵循GNU协议
 *	
 *	文档：http://www.openrpi.org/blogs/?p=11
 */

#include <stdio.h>
#include <signal.h>
#include "bcm2835.h"

#define BLINK_PIN	RPI_V2_GPIO_P1_11
#define DELAY_MS	500

int loop_run = 1;

void loop_stop(int sig)
{
	loop_run = 0;
}

int main()
{
	if(!bcm2835_init())
	{
		printf("GPIO init ERROR!\n");
		return 1;
	}
	
	/**
	 * 设置SIGINT信号。CTRL-C 发出SIGINT信号。
	 */
	signal(SIGINT,loop_stop);
	
	bcm2835_gpio_fsel(BLINK_PIN, BCM2835_GPIO_FSEL_OUTP);
	
	printf("blinking at %fHz, press Ctrl-C to exit...\n",1000.0/DELAY_MS);
	
	while(loop_run)
	{
		bcm2835_gpio_write(BLINK_PIN,1);
		bcm2835_delay(DELAY_MS);
		bcm2835_gpio_write(BLINK_PIN,0);
		bcm2835_delay(DELAY_MS);
	}
	
	bcm2835_close();
	printf("Exit\n");
	return 0;
}
