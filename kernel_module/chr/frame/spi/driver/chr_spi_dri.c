/*
 *	注册SPI设备驱动到内核
 *	驱动名chr_spi_dri，匹配I2C设备chr_spi_dev
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

static struct spi_device_id chr_spi_dri_idtable[] = {
	{ "chr_spi_dev", 0 },
	{ }
};

MODULE_DEVICE_TABLE(spi,chr_spi_dri_idtable);

/**
 * 设备名匹配时的回调函数
 * @param  spi      设备spi结构体指针
 * @param  id_table 设备id_table指针
 * @return          0或错误号
 */
int chr_spi_dri_probe(struct spi_device *spi)
{
	printk(KERN_INFO "++ chr_spi_dri_probe ++\n");

	printk(KERN_INFO "probe device. name=%s\n",spi->modalias);

	printk(KERN_INFO "-- chr_spi_dri_probe --\n");
	return 0;
}

int chr_spi_dri_remove(struct spi_device *spi)
{
	printk(KERN_INFO "++ chr_spi_dri_remove ++\n");

	printk(KERN_INFO "remove device. name=%s\n",spi->modalias);

	printk(KERN_INFO "-- chr_spi_dri_remove --\n");
	return 0;
}

static struct spi_driver chr_spi_dri_driver = {
	.driver = {
		.name	= "chr_spi_dri",
	},

	.id_table	= chr_spi_dri_idtable,
	.probe		= chr_spi_dri_probe,
	.remove		= chr_spi_dri_remove,
};

static int __init chr_spi_dri_init(void)
{
	int re;
	printk(KERN_INFO "++ chr_spi_dri_init ++\n");
	re = spi_register_driver(&chr_spi_dri_driver);
	printk(KERN_INFO "-- chr_spi_dri_init --\n");
	return re;
}

static void __exit chr_spi_dri_exit(void)
{
	printk(KERN_INFO "++ chr_spi_dri_exit ++\n");
	spi_unregister_driver(&chr_spi_dri_driver);
	printk(KERN_INFO "-- chr_spi_dri_exit --\n");
}

module_init(chr_spi_dri_init);
module_exit(chr_spi_dri_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A chr_spi_dri");
