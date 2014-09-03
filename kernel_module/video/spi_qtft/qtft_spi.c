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

#include "qtft_gpio.h"
#include "qtft_fb.h"

#define func_in()	printk(KERN_INFO "++ %s (%d) ++\n", __func__, __LINE__)
#define func_out()	printk(KERN_INFO "-- %s (%d) --\n", __func__, __LINE__)

#define SPI_BUS_NUM 0

static struct spi_device *device=NULL;

static struct spi_board_info qtft_spi_dev_board_info[] __initdata = {
	{
		.modalias    = "qtft_spi_dev",
		.bus_num     = SPI_BUS_NUM,
		.chip_select = 0,
	}
};

static struct spi_device_id qtft_spi_dri_idtable[] = {
	{ "qtft_spi_dev", 0 },
	{ }
};

MODULE_DEVICE_TABLE(spi,qtft_spi_dri_idtable);

/**
 * probe中注册 framebuffer 的device和driver ?
 * @param  spi      设备spi结构体指针
 * @return          0或错误号
 */
int qtft_spi_dri_probe(struct spi_device *spi)
{
	int err=0;
	func_in();

	// 注册gpio
	err = qtft_gpio_init();
	if(err)
	{
		dev_err(&spi->dev, "Can't init qtft gpio\n");
		goto out;
	}

	// 注册 framebuffer
	err = qtft_fb_init();
	if(err)
	{
		dev_err(&spi->dev, "Can't init qtft framebuffer\n");
		goto err0;
	}

err0:
	qtft_gpio_exit();
out:
	func_out();
	return err;
}

int qtft_spi_dri_remove(struct spi_device *spi)
{
	func_in();

	qtft_gpio_exit();
	qtft_fb_exit();

	func_out();
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

static int __init qtft_spi_dri_init(void)
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

static void __exit qtft_spi_dri_exit(void)
{
	func_in();

	qtft_spi_device_unregister(device);
	spi_unregister_driver(&qtft_spi_dri_driver);

	func_out();
}

module_init(qtft_spi_dri_init);
module_exit(qtft_spi_dri_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("qtft spi subsystem");
