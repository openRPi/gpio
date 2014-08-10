/*
 *	main.c 用GPIO的 /dev/mem 接口点亮一盏LED
 *	
 *	Copyright (C) 2014 concefly <h.wenjian@openrpi.org>
 *	Copyright (C) 2014 openRPi
 *	
 *		代码遵循GNU协议
 *	
 *	文档：
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

#define GPIO_BASE	0x20200000
#define GPFSEL1		0x0004
#define GPSET0		0x001c
#define GPCLR0		0x0028

int main()
{
	int mem_fd=0;
	volatile int * gpio=MAP_FAILED;

	/**
	 * 以 读写、同步 的方式打开 /dev/mem 文件
	 */
	if( (mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0 )
	{
		printf("/dev/mem open ERROR: %s\n",strerror(errno));
		return 1;
	}
	printf("open /dev/mem\n");

	/**
	 * 映射 /dev/mem 到 GPIO 的物理地址
	 */
	gpio = mmap(NULL,4096,PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, GPIO_BASE);
	printf("map GPIO_BASE(%x) to %x\n",GPIO_BASE,(unsigned int)gpio);
	
	if(gpio==MAP_FAILED)
	{
		printf("map failed: %s\n",strerror(errno));
		close(mem_fd);
		return 1;
	}

	// GPFSEL1 ^ [23...21] = 001
	*(gpio+GPFSEL1/4) &= ~0xE00000;
	*(gpio+GPFSEL1/4) |= 1<<21;

	*(gpio+GPSET0/4) = 1<<17;
	sleep(1);
	*(gpio+GPCLR0/4) = 1<<17;

	munmap(&gpio,4096);
	close(mem_fd);
	printf("munmap and close\n");

	return 0;
}
