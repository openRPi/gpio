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
#include <linux/string.h>

#include "ops.h"

#define func_in()	printk(KERN_INFO "++ %s:%s (%d) ++\n", __FILE__, __func__, __LINE__)
#define func_out()	printk(KERN_INFO "-- %s:%s (%d) --\n", __FILE__, __func__, __LINE__)

static struct platform_device spi_qtft_device = 
{
	.name = "spi_qtft",
	.id   = 0,
};

static int fill_fix_info(struct fb_fix_screeninfo *fix)
{
	func_in();

	strncpy(fix->id,"qtft_fixinfo",16);
	// Packed Pixels
	fix->type = FB_TYPE_PACKED_PIXELS;
	// True color
	fix->visual      = FB_VISUAL_TRUECOLOR;
	fix->type_aux    = 0;
	fix->xpanstep    = 0;
	fix->ypanstep    = 0;
	fix->ywrapstep   = 0;
	fix->line_length = 16*320;
	// no hardware accelerator
	fix->accel       = FB_ACCEL_NONE;
	// How can we deal with it?
	// fix->smem_start  = 0;
	// fix->smem_len    = 0;
	// fix->mmio_start  = 0;
	// fix->mmio_len    = 0;

	func_out();
	return 0;
}

static int fill_var_info(struct fb_var_screeninfo *var)
{
	func_in();

	var->xres           = 320;
	var->yres           = 240;
	var->xres_virtual   = 320;
	var->yres_virtual   = 240;
	var->xoffset        = 0;
	var->yoffset        = 0;
	var->bits_per_pixel = 16;
	var->grayscale      = 0;
	var->red.length     = 5;
	var->red.offset     = 11;
	var->green.length   = 6;
	var->green.offset   = 5;
	var->blue.length    = 5;
	var->blue.offset    = 0;
	var->transp.length  = 0;
	var->transp.offset  = 0;
	var->nonstd         = 0;
	var->activate       = FB_ACTIVATE_NOW;

	func_out();
	return 0;
}

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
	fb_alloc_cmap(&info->cmap,16,0);

	// 填充screeninfo
	fill_var_info(&info->var);
	fill_fix_info(&info->fix);

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

	fb_dealloc_cmap(&info->cmap);
	kfree(info->pseudo_palette);
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