/*
 *	ds1307.c DS1307时钟芯片的操作函数（IIC）
 *	
 *	Copyright (C) 2014 concefly <h.wenjian@openrpi.org>
 *	Copyright (C) 2014 openRPi
 *	
 *		代码遵循GNU协议
 *	
 *	文档： http://www.openrpi.org/blogs/?p=242
 */

#include <string.h>
#include <bcm2835.h>
#include "ds1307.h"

#define debug 1

#if debug
#include <stdio.h>
#endif

/**
 * 幂运算
 * @param  a 底数
 * @param  b 幂
 * @return   幂运算结果
 */
int pow(int a,int b)
{
	int ret=1;
	while(b--)
		ret *= a;
	return ret;
}

/**
 * DS1307初始化
 * @return  成功1，失败0
 */
int ds1307_init(void)
{
	if(!bcm2835_init())
		return 0;
	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(DS1307_ADDR);
	#if debug
	printf("ds1307 init done\n");
	#endif
	return 1;
}

/**
 * 关闭IIC总线，结束DS1307操作
 */
void ds1307_end(void)
{
	bcm2835_i2c_end();
	#if debug
	printf("ds1307 end\n");
	#endif
}

/**
 * IIC读取一个字节
 * @param  addr 寄存器地址[0...]
 * @return      读取的字节
 */
char ds1307_read_byte(int addr)
{
	char addr_str[1] = {(char)addr};
	char buf[1];
	
	bcm2835_i2c_read_register_rs(addr_str,buf,1);
	#if debug
	printf("read %xH -> 0x%x\n",(char)addr,buf[0]);
	#endif
	return buf[0];
}

/**
 * IIC写入一个字节
 * @param addr 寄存器地址[0...]
 * @param dat  要写入的数据（一个字节）
 */
void ds1307_write_byte(int addr, char dat)
{
	;
}

/**
 * 截取一个字节指定bit段的值。如 byte_join(0xa0,7,4) 截取0xa0第7到第4位的值0xa，返回10.
 * @param  byt 要截取的字节
 * @param  a   最高位
 * @param  b   最低位
 * @return     截取的值
 */
int byte_join(char byt, int a,int b)
{
	unsigned char mask = 0;
	byt >>= b;
	mask = pow(2,a-b+1) - 1;
	return (int)(byt&mask);
}

/**
 * 读取控制位
 * @return  运行 DS1307_CH_ON，停止 DS1307_CH_OFF
 */
int ds1307_get_ch(void)
{
	char byte = ds1307_read_byte(0);
	return byte_join(byte,7,7);
}

/**
 * 读取秒
 * @return  秒数[0...59]
 */
int ds1307_get_sec(void)
{
	char byte = ds1307_read_byte(0);
	return byte_join(byte,6,4)*10 + byte_join(byte,3,0);
}

/**
 * 读取分钟
 * @return  分钟数[0...59]
 */
int ds1307_get_min(void)
{
	char byte = ds1307_read_byte(1);
	return byte_join(byte,6,4)*10 + byte_join(byte,3,0);
}

/**
 * 读取小时模式
 * @return  小时模式，DS1307_HOUR_12 或 DS1307_HOUR_24
 */
int ds1307_get_hour_mode(void)
{
	char byte = ds1307_read_byte(2);
	return byte_join(byte,6,6);
}

/**
 * 以12小时模式读取小时
 * @param hour 小时数指针[1...12]
 * @param ap   AM或PM指示指针。AM为DS1307_HOUR_AM，PM为DS1307_HOUR_PM
 * @return     小时数[1...12]
 */
int ds1307_get_hour_12(int * hour, int * ap)
{
	char byte = ds1307_read_byte(2);
	*ap = byte_join(byte,5,5);
	*hour = byte_join(byte,4,4)*10 + byte_join(byte,3,0);
	return *hour;
}

/**
 * 以24小时模式读取小时
 * @param  hour 小时数指针[0...23]
 * @return      小时数[0...23]
 */
int ds1307_get_hour_24(int * hour)
{
	char byte = ds1307_read_byte(2);
	*hour = byte_join(byte,5,4)*10 + byte_join(byte,3,0);
	return *hour;
}

/**
 * 读取星期
 * @return  星期数[1...7]
 */
int ds1307_get_day(void)
{
	char byte = ds1307_read_byte(3);
	return byte_join(byte,2,0);
}

/**
 * 读取日期
 * @return  日期数[1...31]
 */
int ds1307_get_date(void)
{
	char byte = ds1307_read_byte(4);
	return byte_join(byte,5,4)*10 + byte_join(byte,3,0);
}

/**
 * 读取月份
 * @return  月份[1...12]
 */
int ds1307_get_mon(void)
{
	char byte = ds1307_read_byte(5);
	return byte_join(byte,4,4)*10 + byte_join(byte,3,0);
}

/**
 * 读取年份
 * @return  年份[0...99]
 */
int ds1307_get_year(void)
{
	char byte = ds1307_read_byte(6);
	return byte_join(byte,7,4)*10 + byte_join(byte,3,0);
}

/**
 * 设置ch控制位
 * @param x DS1307_CH_ON或DS1307_CH_OFF
 */
void ds1307_set_ch(int x)
{
	char last=0;
	last = ds1307_read_byte(0);
	if(x)
		last |= 0x80;
	else
		last &= ~0x80;
	ds1307_write_byte(0,last);
}

/**
 * 设置秒数
 * @param sec [0...59]
 */
void ds1307_set_sec(int sec)
{
	char last=0;
	last = ds1307_read_byte(0);
	last &= 0x80;
	last += (unsigned char)sec;
	ds1307_write_byte(0,last);
}

/**
 * 设置分钟
 * @param min [0...59]
 */
void ds1307_set_min(int min)
{
	ds1307_write_byte(1,(unsigned char)min);
}

/**
 * 设置小时模式
 * @param x DS1307_HOUR_12或DS1307_HOUR_24
 */
void ds1307_set_hour_mode(int x)
{
	char last=0;
	last = ds1307_read_byte(2);
	if(x)
		last |= 0x40;
	else
		last &= ~0x40;
	ds1307_write_byte(2,last);
}

/**
 * 以24小时模式设置小时
 * @param hour [0...23]
 */
void ds1307_set_hour_24(int hour)
{
	char last=0;
	last = ds1307_read_byte(2);
	last = 0;
	last += (unsigned char)hour;
	ds1307_write_byte(3,last);
}

/**
 * 以12小时模式设置小时
 * @param hour [1...12]
 * @param ap   DS1307_HOUR_AM 或 DS1307_HOUR_PM
 */
void ds1307_set_hour_12(int hour,int ap)
{
	char last=0;
	last = ds1307_read_byte(2);
	last &= 0x40;
	if(ap==DS1307_HOUR_PM)
		hour += 12;
	last += (unsigned char)hour;
	ds1307_write_byte(3,last);
}

void ds1307_set_day(int day)
{
	ds1307_write_byte(4,(unsigned char)day);
}

void ds1307_set_date(int date)
{
	ds1307_write_byte(5,(unsigned char)date);
}

void ds1307_set_mon(int mon)
{
	ds1307_write_byte(6,(unsigned char)mon);
}

void ds1307_set_year(int year)
{
	ds1307_write_byte(7,(unsigned char)year);
}
