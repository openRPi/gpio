/*
 *	pcf8591.c PCF8591 操作函数库
 *	
 *	Copyright (C) 2014 w.guihong <w.guihong@openrpi.org>
 *	Copyright (C) 2014 openRPi
 *	
 *		代码遵循GNU协议
 *	
 *	文档：
 */

#include "bcm2835.h"
#include "pcf8591.h"

/**
 * PCF8591 初始化
 * 初始化IIC总线，设置芯片地址
 * @return  成功1，失败0
 */
int pcf8591_start(void)
{
	if(!bcm2835_init())
		return 0;
	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(PCF8591_ADDR);
	return 1;
}

/**
 * 发送指令到芯片。
 * @param channel [description]
 */
void pcf8591_send_cmd(char channel)
{
	char buf[1] = { channel  };
  	bcm2835_i2c_write(buf, 1);
}

unsigned char pcf8591_read_byte(void)
{
	char buf[1] = {0};
	bcm2835_i2c_read(buf,1);
	return buf[0];
}

float pcf8591_value(char channel)
{
	unsigned int adnum;
	float value;

	pcf8591_send_cmd(channel);
	adnum = pcf8591_read_byte()*2;
	value = adnum/2 * (PCF8591_REF_VOL / 256);
	return value;
}

void pcf8591_stop(void)
{
	bcm2835_i2c_end();
	bcm2835_close();
}
 
