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

#define func_in()	printk(KERN_INFO "++ %s:%s (%d) ++\n", __FILE__, __func__, __LINE__)
#define func_out()	printk(KERN_INFO "-- %s:%s (%d) --\n", __FILE__, __func__, __LINE__)

static struct platform_device spi_qtft_device = 
{
	.name = "spi_qtft",
	.id   = 0,
};

static struct fb_ops spi_qtft_ops = 
{
	.owner = THIS_MODULE,
};

static int __init spi_qtft_probe(struct platform_device * pdev)
{
	struct fb_info *info;
	func_in();

	info = framebuffer_alloc(0, &pdev->dev);
	// initialize_fb_info(info, pdev);
	info->fbops = &spi_qtft_ops;
	register_framebuffer(info);

	platform_set_drvdata(pdev,info);

	func_out();
	return 0;
}

static int spi_qtft_remove(struct platform_device *pdev)
{
	struct fb_info *info;
	func_in();

	info = platform_get_drvdata(pdev);
	unregister_framebuffer(info);
	framebuffer_release(info);
	platform_set_drvdata(pdev,NULL);

	func_out();
	return 0;
}

static struct platform_driver spi_qtft_driver = 
{
	.probe = spi_qtft_probe,
	.remove = spi_qtft_remove,
	.driver = 
	{
		.name = "spi_qtft_driver",
	},
};

static int  __init spi_qtft_init(void)
{
	int err;
	func_in();

	platform_device_add(&spi_qtft_device);
	err = platform_driver_register(&spi_qtft_driver);

	func_out();
	return err;
}

static void __init spi_qtft_exit(void)
{
	func_in();

	platform_driver_unregister(&spi_qtft_driver);
	platform_device_unregister(&spi_qtft_device);

	func_out();
}

module_init(spi_qtft_init);
module_exit(spi_qtft_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A spi_qtft");