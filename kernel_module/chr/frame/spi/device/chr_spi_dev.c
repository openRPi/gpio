/*
 *	注册SPI设备到内核
 *	设备名chr_spi_dev，设备地址0x10.
 *	只完成加载和卸载的工作。
 *	
 *	Copyright (C) 2014 concefly <h.wenjian@openrpi.org>
 *	Copyright (C) 2014 openRPi
 *	
 *		代码遵循GNU协议
 *	
 *	文档：http://www.openrpi.org/blogs/?p=281
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include <linux/errno.h>
#include <linux/spi/spi.h>

/**
 * 
 */
#define BUS_NUM 0

struct spi_master * master = NULL;
struct spi_device * device = NULL;

static struct spi_board_info chr_spi_dev_board_info[] __initdata = {
	{
		.modalias    = "chr_spi_dev",
		.bus_num     = BUS_NUM,
		.chip_select = 2,
	}
};

static int __init chr_spi_dev_init(void)
{
	printk(KERN_INFO "++ chr_spi_dev_init ++\n");

	/**
	 * 
	 */
	master = spi_busnum_to_master(BUS_NUM);
	if (!master)
		return -ENODEV;
	printk(KERN_INFO "get master (bus %d)\n",BUS_NUM);

	/**
	 * 注册设备
	 * 若存在设备名匹配，SPI驱动程序的probe函数将被自动调用
	 */
	device = spi_new_device(master,chr_spi_dev_board_info);
	if(device)
		printk(KERN_INFO "add new device. name=%s \n",device->modalias);
	else
		return -ENODEV;

	printk(KERN_INFO "-- chr_spi_dev_init --\n");
	return 0;
}

static void __exit chr_spi_dev_exit(void)
{
	printk(KERN_INFO "++ chr_spi_dev_exit ++\n");

	if(device)
	{
		spi_unregister_device(device);
		printk(KERN_INFO "release device.\n");
	}

	printk(KERN_INFO "-- chr_spi_dev_exit --\n");
}

module_init(chr_spi_dev_init);
module_exit(chr_spi_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A chr_spi_dev");
