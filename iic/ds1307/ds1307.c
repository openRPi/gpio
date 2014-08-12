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
	char wbuf[2] = {(char)addr, dat};
	bcm2835_i2c_write(wbuf,2);
	#if debug
	printf("write %xH -> 0x%x\n",(char)addr,dat);
	#endif
}

/**
 * 生成掩码
 * @param  a 最高位
 * @param  b 最低位
 * @return   生成的掩码
 */
unsigned char to_mask(int a,int b)
{
	unsigned char mask = 1;
	int dt=a-b;
	while(dt--)
	{
		mask <<= 1;
		mask += 1;
	}
	mask <<= b;
	return mask;
}

/**
 * 设置一个字节指定bit段的值。
 * @param  in  要设置的字节
 * @param  a   最高位
 * @param  b   最低位
 * @param  dat 写入的数据
 * @return     设置好的字节
 */
unsigned char set_bits(unsigned char in,int a,int b,unsigned char dat)
{
	unsigned char mask = to_mask(a,b);
	in &= ~mask;
	in |= dat<<b;
	return in;
}

/**
 * 截取一个字节指定bit段的值。如 get_bits(0xa0,7,4) 截取0xa0第7到第4位的值0xa，返回10.
 * @param  in 要截取的字节
 * @param  a  最高位
 * @param  b  最低位
 * @return    截取的值
 */
unsigned char get_bits(unsigned char in,int a,int b)
{
	unsigned char mask = to_mask(a,b);
	return (in >> b) & (mask>>b);
}

/**
 * 分解一个BCD码
 * @param bcd BCD码
 * @param a   十位的指针
 * @param b   个位的指针
 */
void bcd_split(unsigned char bcd, int *a, int *b)
{
	*a = get_bits(bcd,7,4);
	*b = get_bits(bcd,3,0);
}

/**
 * 给定十位和个位生成BCD码
 * @param  a 十位
 * @param  b 个位
 * @return   生成的BCD码
 */
unsigned char bcd_join(int a,int b)
{
	return set_bits(set_bits(0,7,4,a),3,0,b);
}

/**
 * 给定数值生成BCD
 * @param  x 给定的数值
 * @return   BCD
 */
unsigned char to_bcd(unsigned char x)
{
	return bcd_join(x/10,x%10);
}

/**
 * 读取控制位
 * @return  运行 DS1307_CH_ON，停止 DS1307_CH_OFF
 */
int ds1307_get_ch(void)
{
	char byte = ds1307_read_byte(0);
	return get_bits(byte,7,7);
}

/**
 * 读取秒
 * @return  秒数[0...59]
 */
int ds1307_get_sec(void)
{
	char byte = ds1307_read_byte(0);
	return get_bits(byte,6,4)*10 + get_bits(byte,3,0);
}

/**
 * 读取分钟
 * @return  分钟数[0...59]
 */
int ds1307_get_min(void)
{
	char byte = ds1307_read_byte(1);
	return get_bits(byte,6,4)*10 + get_bits(byte,3,0);
}

/**
 * 读取小时
 * @param hour 小时数指针[1...12]
 * @param ap   AM或PM指示指针。AM为DS1307_HOUR_AM，PM为DS1307_HOUR_PM
 * @return     小时数[1...12]
 */
int ds1307_get_hour(void)
{
	char byte = ds1307_read_byte(2);
	return get_bits(byte,5,4)*10 + get_bits(byte,3,0);
}

/**
 * 读取星期
 * @return  星期数[1...7]
 */
int ds1307_get_day(void)
{
	char byte = ds1307_read_byte(3);
	return get_bits(byte,2,0);
}

/**
 * 读取日期
 * @return  日期数[1...31]
 */
int ds1307_get_date(void)
{
	char byte = ds1307_read_byte(4);
	return get_bits(byte,5,4)*10 + get_bits(byte,3,0);
}

/**
 * 读取月份
 * @return  月份[1...12]
 */
int ds1307_get_mon(void)
{
	char byte = ds1307_read_byte(5);
	return get_bits(byte,4,4)*10 + get_bits(byte,3,0);
}

/**
 * 读取年份
 * @return  年份[0...99]
 */
int ds1307_get_year(void)
{
	char byte = ds1307_read_byte(6);
	return get_bits(byte,7,4)*10 + get_bits(byte,3,0);
}

/**
 * 设置ch控制位
 * @param x DS1307_CH_ON或DS1307_CH_OFF
 */
void ds1307_set_ch(int x)
{
	char last=0;
	last = ds1307_read_byte(0);
	last = set_bits(last,7,7,x?1:0);
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
	ds1307_write_byte(0,to_bcd(get_bits(last,6,0)));
}

/**
 * 设置分钟
 * @param min [0...59]
 */
void ds1307_set_min(int min)
{
	ds1307_write_byte(1,to_bcd(min));
}

/**
 * 以24小时模式设置小时。BIT6=0时为24小时模式。
 * @param hour [0...23]
 */
void ds1307_set_hour(int hour)
{
	ds1307_write_byte(2,to_bcd(hour));
}

void ds1307_set_day(int day)
{
	ds1307_write_byte(3,to_bcd(day));
}

void ds1307_set_date(int date)
{
	ds1307_write_byte(4,to_bcd(date));
}

void ds1307_set_mon(int mon)
{
	ds1307_write_byte(5,to_bcd(mon));
}

void ds1307_set_year(int year)
{
	ds1307_write_byte(6,to_bcd(year));
}
