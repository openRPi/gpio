/*
 *	qtft 视频驱动的 SPI 子系统
 *	SPI 注册成功后，再进一步注册 framebuffer
 *
 *  依赖：spi_bcm2708 (SPI controller driver)
 *
 *  注意：不可先加载 spidev (User mode SPI device interface) 模块，
 *  因为它会占用 chip_select 0~1，导致本模块无法加载。
 *	
 *	Copyright (C) 2014 concefly <h.wenjian@openrpi.org>
 *	Copyright (C) 2014 openRPi
 *	
 *		代码遵循GNU协议
 *	
 *	文档：?
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include <linux/errno.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>

#include "qtft_gpio.h"
#include "qtft_fb.h"

#define func_in()	printk(KERN_INFO "++ %s (%d) ++\n", __func__, __LINE__)
#define func_out()	printk(KERN_INFO "-- %s (%d) --\n", __func__, __LINE__)

#define SPI_BUS_NUM 0

static struct spi_device *device=NULL;

static struct spi_board_info qtft_spi_dev_board_info[] = {
	{
		.modalias    = "qtft_spi_dev",
		.bus_num     = SPI_BUS_NUM,
		.chip_select = 0,
		.mode        = SPI_MODE_3,
	}
};

static struct spi_device_id qtft_spi_dri_idtable[] = {
	{ "qtft_spi_dev", 0 },
	{ }
};

MODULE_DEVICE_TABLE(spi,qtft_spi_dri_idtable);

#define lcd_context_reg()	qtft_gpio_set_dc(0)
#define lcd_context_data()	qtft_gpio_set_dc(1)

/**
 * 初始化 LCD 模块。
 * 会引起睡眠
 * @return  0或错误号
 */
int lcd_init(struct spi_device *dev)
{
	qtft_gpio_set_reset(1);
	msleep(5);
	qtft_gpio_set_reset(0);
	msleep(5);
	qtft_gpio_set_reset(1);

	lcd_context_reg();
	spi_write(dev, "\xCB", 1);
	lcd_context_data();
	spi_write(dev, "\x39", 1);
	spi_write(dev, "\x2C", 1);
	spi_write(dev, "\x00", 1);
	spi_write(dev, "\x34", 1);
	spi_write(dev, "\x02", 1);

	lcd_context_reg();
	spi_write(dev, "\xCF", 1);
	lcd_context_data();
	spi_write(dev, "\x00", 1);
	spi_write(dev, "\xC1", 1);
	spi_write(dev, "\x30", 1);

	lcd_context_reg();
	spi_write(dev, "\xE8", 1);
	lcd_context_data();
	spi_write(dev, "\x85", 1);
	spi_write(dev, "\x00", 1);
	spi_write(dev, "\x78", 1);

	lcd_context_reg();
	spi_write(dev, "\xEA", 1);
	lcd_context_data();
	spi_write(dev, "\x00", 1);
	spi_write(dev, "\x00", 1);

	lcd_context_reg();
	spi_write(dev, "\xED", 1);
	lcd_context_data();
	spi_write(dev, "\x64", 1);
	spi_write(dev, "\x03", 1);
	spi_write(dev, "\x12", 1);
	spi_write(dev, "\x81", 1);

	lcd_context_reg();
	spi_write(dev, "\xF7", 1);
	lcd_context_data();
	spi_write(dev, "\x20", 1);

	lcd_context_reg();
	spi_write(dev, "\xC0", 1);
	lcd_context_data();
	spi_write(dev, "\x23", 1);

	lcd_context_reg();
	spi_write(dev, "\xC1", 1);
	lcd_context_data();
	spi_write(dev, "\x10", 1);

	lcd_context_reg();
	spi_write(dev, "\xC5", 1);
	lcd_context_data();
	spi_write(dev, "\x3E", 1);
	spi_write(dev, "\x28", 1);

	lcd_context_reg();
	spi_write(dev, "\xC7", 1);
	lcd_context_data();
	spi_write(dev, "\x86", 1);

	lcd_context_reg();
	spi_write(dev, "\x36", 1);
	lcd_context_data();
	spi_write(dev, "\x28", 1);

	lcd_context_reg();
	spi_write(dev, "\x3A", 1);
	lcd_context_data();
	spi_write(dev, "\x55", 1);

	lcd_context_reg();
	spi_write(dev, "\xB1", 1);
	lcd_context_data();
	spi_write(dev, "\x00", 1);
	spi_write(dev, "\x18", 1);

	lcd_context_reg();
	spi_write(dev, "\xB6", 1);
	lcd_context_data();
	spi_write(dev, "\x08", 1);
	spi_write(dev, "\x82", 1);
	spi_write(dev, "\x27", 1);

	lcd_context_reg();
	spi_write(dev, "\xF2", 1);
	lcd_context_data();
	spi_write(dev, "\x00", 1);

	lcd_context_reg();
	spi_write(dev, "\x26", 1);
	lcd_context_data();
	spi_write(dev, "\x01", 1);

	lcd_context_reg();
	spi_write(dev, "\xE0", 1);     //Set Gamma 
	lcd_context_data();
	spi_write(dev, "\x0F", 1); 
	spi_write(dev, "\x31", 1); 
	spi_write(dev, "\x2B", 1); 
	spi_write(dev, "\x0C", 1); 
	spi_write(dev, "\x0E", 1); 
	spi_write(dev, "\x08", 1); 
	spi_write(dev, "\x4E", 1); 
	spi_write(dev, "\xF1", 1); 
	spi_write(dev, "\x37", 1); 
	spi_write(dev, "\x07", 1); 
	spi_write(dev, "\x10", 1); 
	spi_write(dev, "\x03", 1); 
	spi_write(dev, "\x0E", 1); 
	spi_write(dev, "\x09", 1); 
	spi_write(dev, "\x00", 1); 

	lcd_context_reg();
	spi_write(dev, "\xE1", 1);    //Set Gamma 
	lcd_context_data();
	spi_write(dev, "\x00", 1); 
	spi_write(dev, "\x0E", 1); 
	spi_write(dev, "\x14", 1); 
	spi_write(dev, "\x03", 1); 
	spi_write(dev, "\x11", 1); 
	spi_write(dev, "\x07", 1); 
	spi_write(dev, "\x31", 1); 
	spi_write(dev, "\xC1", 1); 
	spi_write(dev, "\x48", 1); 
	spi_write(dev, "\x08", 1); 
	spi_write(dev, "\x0F", 1); 
	spi_write(dev, "\x0C", 1); 
	spi_write(dev, "\x31", 1); 
	spi_write(dev, "\x36", 1); 
	spi_write(dev, "\x0F", 1); 

	lcd_context_reg();
	spi_write(dev, "\x11", 1);
	msleep(120);

	spi_write(dev, "\x29", 1);
	spi_write(dev, "\x2C", 1);

	return 0;
}

/**
 * 初始化 LCD 模块
 */
int qtft_spi_dri_probe(struct spi_device *dev)
{
	int err = 0;

	err = lcd_init(dev);
	if(err)
	{
		dev_err(&dev->dev, "Can't init LCD\n");
		goto out;
	}

	goto out;

out:
	return err;
}

int qtft_spi_dri_remove(struct spi_device *dev)
{
	return 0;
}

static struct spi_driver qtft_spi_dri_driver = {
	.driver = {
		.name	= "qtft_spi_dri",
		.owner = THIS_MODULE,
	},

	.id_table	= qtft_spi_dri_idtable,
	.probe		= qtft_spi_dri_probe,
	.remove		= qtft_spi_dri_remove,
};

static void qtft_spi_device_unregister(struct spi_device *device)
{
	spi_unregister_device(device);
}

int qtft_spi_init(void)
{
	int err=0;
	struct spi_master *master=NULL;
	func_in();

	err = spi_register_driver(&qtft_spi_dri_driver);
	if(err)
	{
		printk(KERN_ERR "Can't register spi driver \n");
		goto out;
	}

	master = spi_busnum_to_master(SPI_BUS_NUM);
	if(!master)
	{
		printk(KERN_ERR "Can't get SPI bus %d\n",SPI_BUS_NUM);
		err = -ENODEV;
		goto err0;
	}

	device = spi_new_device(master, qtft_spi_dev_board_info);
	if(!device)
	{
		printk(KERN_ERR "Can't register spi device \n");
		err = -ENODEV;
		goto err0;
	}

	goto out;

err0:
	spi_unregister_driver(&qtft_spi_dri_driver);
out:
	func_out();
	return err;
}

void qtft_spi_exit(void)
{
	func_in();

	qtft_spi_device_unregister(device);
	spi_unregister_driver(&qtft_spi_dri_driver);

	func_out();
}

int qtft_spi_write(const void *buf, size_t len)
{
	return spi_write(device, buf, len);
}

int qtft_spi_read(void *buf, size_t len)
{
	return spi_read(device, buf, len);
}