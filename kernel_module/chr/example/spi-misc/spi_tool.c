/*
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

#define SPI_BUS_NUM 0

#define func_in()	printk(KERN_INFO "++ %s (%d) ++\n", __func__, __LINE__)
#define func_out()	printk(KERN_INFO "-- %s (%d) --\n", __func__, __LINE__)

static struct spi_device *current_device=NULL;
static struct spi_device *new_device=NULL;

static struct spi_board_info spi_tool_dev_board_info[] __initdata = {
	{
		.modalias    = "spi_tool_dev",
		.bus_num     = SPI_BUS_NUM,
		.chip_select = 0,
	}
};

static struct spi_device_id spi_tool_idtable[] = {
	{ "spi_tool_dev", 0 },
	{ }
};

MODULE_DEVICE_TABLE(spi,spi_tool_idtable);

/**
 * 设备名匹配时的回调函数
 * @param  spi      设备spi结构体指针
 * @return          0或错误号
 */
int spi_tool_probe(struct spi_device *dev)
{
	func_in();

	current_device = dev;

	func_out();
	return 0;
}

int spi_tool_remove(struct spi_device *dev)
{
	func_in();

	current_device = NULL;

	func_out();
	return 0;
}

static struct spi_driver spi_tool_driver = {
	.driver = {
		.name	= "spi_tool_dri",
		.owner = THIS_MODULE,
	},

	.id_table	= spi_tool_idtable,
	.probe		= spi_tool_probe,
	.remove		= spi_tool_remove,
};

static int __init spi_tool_init(void)
{
	int err=0;
	struct spi_master *master = NULL;
	func_in();

	err = spi_register_driver(&spi_tool_driver);
	if(err)
	{
		printk(KERN_ERR "Can't get SPI driver \n");
		goto out;
	}

	master = spi_busnum_to_master(SPI_BUS_NUM);
	if(!master)
	{
		printk(KERN_ERR "Can't get SPI bus %d\n",SPI_BUS_NUM);
		err = -ENODEV;
		goto err0;
	}

	new_device = spi_new_device(master,spi_tool_dev_board_info);
	if(!new_device)
	{
		printk(KERN_ERR "Can't add SPI device \n");
		goto err0;
	}

	goto out;

err0:
	spi_unregister_driver(&spi_tool_driver);
out:
	func_out();
	return err;
}

static void __exit spi_tool_exit(void)
{
	func_in();
	
	if(new_device)
	{
		spi_unregister_device(new_device);
	}
	spi_unregister_driver(&spi_tool_driver);

	func_out();
}

module_init(spi_tool_init);
module_exit(spi_tool_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A spi_tool");
