// ILI9341 芯片

#include "lcd_lib.h"

enum flag_t {
	flag_data,
	flag_cmd,
};

/**
 * 延时毫秒
 * 可重写
 * 
 * @param ms 毫秒
 */
void delay_ms(int ms)
{
	;
}

/**
 * 底层接口初始化
 * 依赖于硬件，可重写
 * 
 * @return  0或错误号
 */
int iface_init(void)
{
	return 0;
}

/**
 * 底层接口的数据传输函数
 * 依赖于硬件，可重写
 * 
 * @param  tbuf 发送数组
 * @param  tn   发送字节数
 * @param  rbuf 接收数组
 * @param  rn   接收字节数
 * @param  flag 标示符（自定义的其他操作）
 * @return      0或错误号
 */
int iface_write_then_read(void *tbuf, int tn, void *rbuf, int rn, enum flag_t flag)
{
	return 0;
}

int w8(unsigned char value, enum flag_t flag)
{
	return iface_write_then_read(&value, 1, NULL, 0, flag);
}

int r8(unsigned char *value, enum flag_t flag)
{
	return iface_write_then_read(NULL, 0, value, 1, flag);
}

int wc8_then_wd8(unsigned char cmd, unsigned char data)
{
	int err=0;
	err = iface_w8(cmd, flag_cmd);
	if(err)
		return err;
	err = iface_w8(mode, flag_data);
	if(err)
		return err;
	return 0;
}

int wc8_then_wdbuf(unsigned char cmd, const unsigned char *buf, int size)
{
	int err=0;
	err = iface_w8(cmd, flag_cmd);
	if(err)
		return err;
	err = iface_write_then_read(buf, size, NULL, 0, flag_data);
	if(err)
		return err;
	return 0;
}

int wc8_then_rdbuf(unsigned char cmd, unsigned char *buf, int size)
{
	int err=0;
	err = iface_w8(cmd, flag_cmd);
	if(err)
		return err;
	err = iface_write_then_read(NULL, 0, buf, size, flag_data);
	if(err)
		return err;
	return 0;
}

int lcd_sleep_out(void)
{
	return w8(0x10, flag_cmd);
}

int lcd_memory_access_control(int mode)
{
	return wc8_then_wd8(0x36, mode);
}

int lcd_pixel_format_set(int mode)
{
	return wc8_then_wd8(0x3a, mode);
}

int lcd_display_on(void)
{
	return w8(0x29, flag_cmd);
}

int lcd_memory_write(const unsigned char *buf, int size)
{
	return wc8_then_wdbuf(0x2c, buf, size);
}

int lcd_column_address_set(int x1, int x2)
{
	unsigned char tbuf=[4];
	tbuf[0] = (x1 >> 8) & 0xff;
	tbuf[1] = x1 & 0xff;
	tbuf[2] = (x2 >> 8) & 0xff;
	tbuf[3] = x2 & 0xff;
	return wc8_then_wdbuf(0x2a, tbuf, 4);
}

int lcd_page_address_set(int y1, int y2)
{
	unsigned char tbuf=[4];
	tbuf[0] = (y1 >> 8) & 0xff;
	tbuf[1] = y1 & 0xff;
	tbuf[2] = (y2 >> 8) & 0xff;
	tbuf[3] = y2 & 0xff;
	return wc8_then_wdbuf(0x2b, tbuf, 4);
}

int lcd_memory_area_read(int x1, int y1, int x2, int y2, unsigned char *buf, int size)
{
	int err=0;
	int area_size=(x2-x1+1)*(y2-y1+1);
	int min = size<area_size ? size : area_size;

	lcd_address_set(x1,y1,x2,y2);
	err = wc8_then_rdbuf(0x2e, buf, min);
	return err? err : min;
}

int lcd_init(void)
{
	int err=0;

	err = iface_init();
	if(err)
		return err;

	err = lcd_memory_access_control(MEMORY_ACCESS_NORMAL);
	if(err)
		return err;

	err = lcd_pixel_format_set(PIXEL_FORMAT_16);
	if(err)
		return err;

	err = lcd_sleep_out();
	if(err)
		return err;
	
	err = lcd_display_on();
	if(err)
		return err;
}