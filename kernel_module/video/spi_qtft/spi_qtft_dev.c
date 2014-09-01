/*
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
#include <linux/fb.h>
#include <linux/platform_device.h>

#include "ops.h"

#define func_in()	printk(KERN_INFO "++ %s:%s (%d) ++\n", __FILE__, __func__, __LINE__)
#define func_out()	printk(KERN_INFO "-- %s:%s (%d) --\n", __FILE__, __func__, __LINE__)

static struct platform_device spi_qtft_device = 
{
	.name = "spi_qtft",
	.id   = 0,
};

static int spi_qtft_probe(struct platform_device * pdev)
{
	struct fb_info *info;
	int err=0;
	func_in();

	info = framebuffer_alloc(0, &pdev->dev);
	if(!info)
	{
		dev_err(&pdev->dev,"Failed to alloc framebuffer\n");
		err = -EBUSY;
		goto out;
	}
	// spi_qtft_ops 定义在 ops.c
	info->fbops = &spi_qtft_ops;
	
	err = register_framebuffer(info);
	if(err<0)
	{
		dev_err(&pdev->dev,"Failed to register framebuffer\n");
		goto free_framebuffer_alloc;
	}

	platform_set_drvdata(pdev,info);

free_framebuffer_alloc:
	framebuffer_release(info);
out:
	func_out();
	return err;
}

static int spi_qtft_remove(struct platform_device *pdev)
{
	struct fb_info *info;
	int err=0;
	func_in();

	info = platform_get_drvdata(pdev);
	if(!info)
	{
		dev_err(&pdev->dev,"platform_get_drvdata retutn NULL\n");
		err = -ENODEV;
		goto out;
	}
	unregister_framebuffer(info);
	framebuffer_release(info);
	platform_set_drvdata(pdev,NULL);

out:
	func_out();
	return err;
}

struct platform_device_id spi_qtft_idtable[] = 
{
	{"spi_qtft", 0},
	{ },
};

static struct platform_driver spi_qtft_driver = 
{
	.driver = 
	{
		.name = "spi_qtft_driver",
	},
	.probe    = spi_qtft_probe,
	.remove   = spi_qtft_remove,
	.id_table = spi_qtft_idtable,
};

static int  __init spi_qtft_init(void)
{
	int err;
	func_in();

	err = platform_device_register(&spi_qtft_device);
	if(err<0)
	{
		printk(KERN_ERR "Failed to add platform device\n");
		goto out;
	}

	err = platform_driver_register(&spi_qtft_driver);
	if(err<0)
	{
		printk(KERN_ERR "Failed to register platform driver\n");
		goto out;
	}

out:
	func_out();
	return err;
}

static void __exit spi_qtft_exit(void)
{
	func_in();

	platform_device_unregister(&spi_qtft_device);
	platform_driver_unregister(&spi_qtft_driver);

	func_out();
}

module_init(spi_qtft_init);
module_exit(spi_qtft_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("spi QVGA TFT driver");