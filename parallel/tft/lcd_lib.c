// ILI9486 芯片
// 
#include "lcd_lib.h"
#include "bcm2835.h"

#define NULL ((void *)0)

#define TFT_RST 24
#define TFT_DC	23
#define TFT_WR	5
#define TFT_CS	18

#define TFT_D7	26
#define TFT_D6	21
#define TFT_D5	20
#define TFT_D4	19
#define TFT_D3	16
#define TFT_D2	13
#define TFT_D1	12
#define TFT_D0	6

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
	bcm2835_delay(ms);
}

/**
 * RESET 管脚控制，可重写
 * @param x 0或1
 */
void iface_set_reset(int x)
{
	bcm2835_gpio_write(TFT_RST,x);
}

/**
 * 底层接口初始化
 * 依赖于硬件，可重写
 * 
 * @return  0或错误号
 */
int iface_init(void)
{
	if(!bcm2835_init())
		return -1;

	bcm2835_gpio_fsel(TFT_RST, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TFT_DC , BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TFT_WR , BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TFT_CS , BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_fsel(TFT_D7 , BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TFT_D6 , BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TFT_D5 , BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TFT_D4 , BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TFT_D3 , BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TFT_D2 , BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TFT_D1 , BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TFT_D0 , BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_write(TFT_WR , 1);
	bcm2835_gpio_write(TFT_RST, 1);
	bcm2835_gpio_write(TFT_DC , 1);

	bcm2835_gpio_write(TFT_CS , 0);

	return 0;
}

/**
 * 释放底层接口
 */
void iface_exit(void)
{
	bcm2835_gpio_write(TFT_CS , 1);
	bcm2835_close();
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
int iface_write_then_read(const void *tbuf, int tn, void *rbuf, int rn, enum flag_t flag)
{
	int err=0;

	switch(flag)
	{
		case flag_cmd: 
			bcm2835_gpio_write(TFT_DC, 0);
			break;

		case flag_data:
			bcm2835_gpio_write(TFT_DC, 1);
			break;

		default:
			err = -1;
			goto out;
	}

	if(tn)
	{
		unsigned char * current = tbuf;
		while(tn--)
		{
			bcm2835_gpio_write(TFT_WR, 0);

			bcm2835_gpio_write(TFT_D7, (*current) & 0x01<<7);
			bcm2835_gpio_write(TFT_D6, (*current) & 0x01<<6);
			bcm2835_gpio_write(TFT_D5, (*current) & 0x01<<5);
			bcm2835_gpio_write(TFT_D4, (*current) & 0x01<<4);
			bcm2835_gpio_write(TFT_D3, (*current) & 0x01<<3);
			bcm2835_gpio_write(TFT_D2, (*current) & 0x01<<2);
			bcm2835_gpio_write(TFT_D1, (*current) & 0x01<<1);
			bcm2835_gpio_write(TFT_D0, (*current) & 0x01<<0);

			bcm2835_gpio_write(TFT_WR, 1);

			current++ ;
		}
	}

	if(rn)
	{
		;
	}

	goto out;

out:
	return err;
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
	err = w8(cmd, flag_cmd);
	if(err)
		return err;
	err = w8(data, flag_data);
	if(err)
		return err;
	return 0;
}

int wc8_then_wdbuf(unsigned char cmd, const unsigned char *buf, int size)
{
	int err=0;
	err = w8(cmd, flag_cmd);
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
	err = w8(cmd, flag_cmd);
	if(err)
		return err;
	err = iface_write_then_read(NULL, 0, buf, size, flag_data);
	if(err)
		return err;
	return 0;
}

int lcd_sleep_in(int delay)
{
	int err=0;
	err = w8(0x10,flag_cmd);
	if(err || !delay)
		return err;
	delay_ms(delay);
	return 0;
}

int lcd_sleep_out(void)
{
	return w8(0x11, flag_cmd);
}

int lcd_memory_access_control(int mode)
{
	return wc8_then_wd8(0x36, mode);
}

int lcd_pixel_format_set(int mode)
{
	return wc8_then_wd8(0x3a, mode);
}

int lcd_display_off(void)
{
	return w8(0x28,flag_cmd);
}

int lcd_display_on(void)
{
	return w8(0x29, flag_cmd);
}

int lcd_column_address_set(int x1, int x2)
{
	unsigned char tbuf[4];
	tbuf[0] = (x1 >> 8) & 0xff;
	tbuf[1] = x1 & 0xff;
	tbuf[2] = (x2 >> 8) & 0xff;
	tbuf[3] = x2 & 0xff;
	return wc8_then_wdbuf(0x2a, tbuf, 4);
}

int lcd_page_address_set(int y1, int y2)
{
	unsigned char tbuf[4];
	tbuf[0] = (y1 >> 8) & 0xff;
	tbuf[1] = y1 & 0xff;
	tbuf[2] = (y2 >> 8) & 0xff;
	tbuf[3] = y2 & 0xff;
	return wc8_then_wdbuf(0x2b, tbuf, 4);
}

int lcd_cursor_reset(void)
{
	return w8(0x2c,flag_cmd);
}

int lcd_memory_area_write(const unsigned char *buf, int size, int _continue)
{
	if(_continue)
		return wc8_then_wdbuf(0x3c, buf, size);
	else
		return wc8_then_wdbuf(0x2c, buf, size);
}

int lcd_memory_write(const unsigned char *buf, int size, int _continue)
{
	lcd_address_set(0,0,LCD_WIDTH_MAX,LCD_HEIGHT_MAX);
	return lcd_memory_area_write(buf, size, _continue);
}

int lcd_memory_area_read(unsigned char *buf, int size, int _continue)
{
	if(_continue)
		return wc8_then_wdbuf(0x2e, buf,size);
	else
		return wc8_then_wdbuf(0x3e, buf,size);
}

int lcd_memory_read(unsigned char *buf, int size, int _continue)
{
	lcd_address_set(0,0,LCD_WIDTH_MAX,LCD_HEIGHT_MAX);
	return lcd_memory_area_read(buf, size, _continue);
}

int lcd_power_contral_a(int reg_vd, int vbc)
{
	unsigned char buf[5] = {0x39,0x2c,0x00,
		0x34|(reg_vd&0x07),0x02|(vbc&0x07)};

	return wc8_then_wdbuf(0xcb, buf, 5);
}

int lcd_power_contral_b(int pc, int dc_ena)
{
	unsigned char buf[5] = {0x39,0x2c,0x00,
		0x81|((pc<<3)&0x18), dc_ena? 0x40:0x30};

	return wc8_then_wdbuf(0xcf, buf, 5);
}

int lcd_soft_reset(void)
{
	return w8(0x01, flag_cmd);
}

int lcd_hard_reset(void)
{
	iface_set_reset(1);
	delay_ms(5);
	iface_set_reset(0);
	delay_ms(20);
	iface_set_reset(1);
	delay_ms(120);
	return 0;
}

int lcd_init(void)
{
	return iface_init();
}

int lcd_normal_config(void)
{
	#define return_err(func) do{int err=func; if(err){return err;}}while(0)

	// int init_cmd[] = {
	// 	-1,0xC0,0x08,0x06,	// Power Control 1
	// 	-1,0xC1,0x43,		// Power Control 2
	// 	-1,0XC5,0x00,0x40,	// VCOM Control
	// 	-1,0xC2,0x33,		// Power Control 3
	// 	-1,0xB1,0xB0,0x11,	// Frame Rate Control
	// 	-1,0xB4,0x02,		// Display Inversion Control
	// 	-1,0xB7,0x86,		// Entry Mode Set
	// 	-1,0xE0,0x0F,0x29,0x25,0x0B,0x0E,0x07,0x42,0x87,0x2C,0x06,0x0F,0x02,0x0B,0x07,0x00, // Positive Gamma Control
	// 	-1,0XE1,0x0F,0x38,0x34,0x0D,0x10,0x09,0x53,0x87,0x3D,0x08,0x11,0x04,0x1A,0x16,0x00, // Negative Gamma Correction
	// 	-1,0XF2,0x1E,0xA3,0x32,0x02,0xB2,0x52,0xFF,0x10,0x00, 
	// 	-1,0XF8,0x21,0x04,
	// 	-1,0x36,MEMORY_ACCESS_NORMAL,		// Memory Access Control
	// 	-1,0x3A,0x55,		// Interface Pixel Format
	// 	-1,0xF9,0x00,0x08 ,
	// 	-1,0XF1,0x36,0x04,0x00,0x3C,0X0F,0x8F,
	// 	-1,0xB6,0x00,0x22,0x3B,			// Display Function Control
	// 	// -1,0x2A,0x00,0x00,0x01,0x3F,	// Column Address Set
	// 	// -1,0x2B,0x00,0x00,0x01,0xDF,	// Page Address Set
	// 	// -1,0x21,	// Display Inversion ON
	// 	-1,0x11,	// Sleep OUT
	// 	-2,120,
	// 	-1,0x29,	// Display ON
	// 	-3,
	// };
	
	int init_cmd[] = {
	/* Interface Mode Control */
		-1, 0xb0, 0x0,
		/* Sleep OUT */
		-1, 0x11,
		-2, 250,
		/* Interface Pixel Format */
		-1, 0x3A, 0x55,
		/* Power Control 3 */
		-1, 0xC2, 0x44,
		/* VCOM Control 1 */
		-1, 0xC5, 0x00, 0x00, 0x00, 0x00,
		-1,0x36,0x20,		// Memory Access Control
		/* PGAMCTRL(Positive Gamma Control) */
		-1, 0xE0, 0x0F, 0x1F, 0x1C, 0x0C, 0x0F, 0x08, 0x48, 0x98,
		          0x37, 0x0A, 0x13, 0x04, 0x11, 0x0D, 0x00,
		/* NGAMCTRL(Negative Gamma Control) */
		-1, 0xE1, 0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75,
		          0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00,
		/* Digital Gamma Control 1 */
		-1, 0xE2, 0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75,
		          0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00,
		/* Sleep OUT */
		-1, 0x11,
		-2, 120,
		/* Display ON */
		-1, 0x29,
		/* end marker */
		-3
	};


	int i=0, current_type=0, has_sent_cmd=0;

	for(i=0;;i++)
	{
		if(init_cmd[i]<0)
		{
			if(init_cmd[i]==-3)
				break;
			else
			{
				current_type = init_cmd[i];
				has_sent_cmd = 0;
				continue;
			}
		}

		if(current_type==-1 && !has_sent_cmd)
		{
			w8(init_cmd[i],flag_cmd);
			has_sent_cmd = 1;
			continue;
		}

		if(current_type==-1 && has_sent_cmd)
		{
			w8(init_cmd[i],flag_data);
			continue;
		}

		if(current_type==-2)
		{
			delay_ms(init_cmd[i]);
			continue;
		}

	}

	#undef return_err

	return 0;
}

void lcd_exit(void)
{
	iface_exit();
}