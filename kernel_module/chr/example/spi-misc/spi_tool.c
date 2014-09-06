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

#define func_in()	printk(KERN_INFO "++ %s:%s (%d) ++\n", __func__, __LINE__)
#define func_out()	printk(KERN_INFO "-- %s:%s (%d) --\n", __func__, __LINE__)

static struct spi_device_id spi_tool_dri_idtable[] = {
	{ "spi_tool_dev", 0 },
	{ }
};

MODULE_DEVICE_TABLE(spi,spi_tool_dri_idtable);

/**
 * 设备名匹配时的回调函数
 * @param  spi      设备spi结构体指针
 * @return          0或错误号
 */
int spi_tool_dri_probe(struct spi_device *spi)
{
	func_in();


	func_out();
	return 0;
}

int spi_tool_dri_remove(struct spi_device *spi)
{
	func_in();


	func_out();
	return 0;
}

static struct spi_driver spi_tool_dri_driver = {
	.driver = {
		.name	= "spi_tool_dri",
		.owner = THIS_MODULE,
	},

	.id_table	= spi_tool_dri_idtable,
	.probe		= spi_tool_dri_probe,
	.remove		= spi_tool_dri_remove,
};

static int __init spi_tool_dri_init(void)
{
	int err=0;
	struct spi_master *master = NULL;
	func_in();

	err = spi_register_driver(&spi_tool_dri_driver);
	if(err)
		goto out;

	master = spi_
	err = spi_alloc_device()

out:
	func_out();
	return err;
}

static void __exit spi_tool_dri_exit(void)
{
	func_in();
	spi_unregister_driver(&spi_tool_dri_driver);
	func_out();
}

module_init(spi_tool_dri_init);
module_exit(spi_tool_dri_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A spi_tool_dri");
