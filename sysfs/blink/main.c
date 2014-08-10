/*
 *	main.c 用GPIO的sysfs接口点亮一盏LED
 *	
 *	Copyright (C) 2014 concefly <h.wenjian@openrpi.org>
 *	Copyright (C) 2014 openRPi
 *	
 *		代码遵循GNU协议
 *	
 *	文档：
 */

#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#define PIN_STR "17"
#define PIN_INT 17

int loop_run = 1;

void loop_stop(int sig)
{
	loop_run = 0;
}

int main()
{
	FILE * fd;

	signal(SIGINT,loop_stop);
	
	// 导出 GPIO 控制文件到用户空间
	fd = fopen("/sys/class/gpio/export","w");
	fputs(PIN_STR,fd);
	fclose(fd);
	
	printf("export gpio%d\n",PIN_INT);
	
	// 设置 GPIO 为输出
	fd = fopen("/sys/class/gpio/gpio" PIN_STR "/direction","w");
	fputs("out",fd);
	fclose(fd);

	/**
	 * 写 GPIO 的输出电平。
	 * 调用 sleep() 函数前应及时flush相应的文件指针。
	 */
	fd = fopen("/sys/class/gpio/gpio" PIN_STR "/value","w");
	while(loop_run)
	{
		fputs("1",fd);
		printf("\rGPIO%d = 1",PIN_INT);
		fflush(stdout);
		fflush(fd);
		sleep(1);
		fputs("0",fd);
		printf("\rGPIO%d = 0",PIN_INT);
		fflush(stdout);
		fflush(fd);
		sleep(1);
	}
	fclose(fd);
	
	// 回收 GPIO 
	fd = fopen("/sys/class/gpio/unexport","w");
	fputs(PIN_STR,fd);
	fclose(fd);
	printf("unexport gpio%d\n",PIN_INT);
	
	return 0;
}
