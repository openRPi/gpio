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
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>

#include <linux/fb.h>
#include <linux/init.h>

#include "ops.h"


#define VIDEOMEMSIZE	(320*240*16) 

#define func_in()	printk(KERN_INFO "++ %s:%s (%d) ++\n", __FILE__, __func__, __LINE__)
#define func_out()	printk(KERN_INFO "-- %s:%s (%d) --\n", __FILE__, __func__, __LINE__)

static void *videomemory;
static u_long videomemorysize = VIDEOMEMSIZE;

static struct platform_device *spi_qtft_device;

extern struct fb_var_screeninfo spi_qtft_var_default;
extern struct fb_fix_screeninfo spi_qtft_fix_default;

static void *rvmalloc(unsigned long size)
{
	void *mem;
	unsigned long adr;

	size = PAGE_ALIGN(size);
	mem = vmalloc_32(size);
	if (!mem)
		return NULL;

	memset(mem, 0, size); /* Clear the ram out, no junk to the user */
	adr = (unsigned long) mem;
	while (size > 0) {
		SetPageReserved(vmalloc_to_page((void *)adr));
		adr += PAGE_SIZE;
		size -= PAGE_SIZE;
	}

	return mem;
}

static void rvfree(void *mem, unsigned long size)
{
	unsigned long adr;

	if (!mem)
		return;

	adr = (unsigned long) mem;
	while ((long) size > 0) {
		ClearPageReserved(vmalloc_to_page((void *)adr));
		adr += PAGE_SIZE;
		size -= PAGE_SIZE;
	}
	vfree(mem);
}

static int spi_qtft_probe(struct platform_device * dev)
{
	struct fb_info *info;
	int retval = -ENOMEM;

	func_in();

	// 分配显存
	if (!(videomemory = rvmalloc(videomemorysize)))
		return retval;

	/*
	 * VFB must clear memory to prevent kernel info
	 * leakage into userspace
	 * VGA-based drivers MUST NOT clear memory if
	 * they want to be able to take over vgacon
	 */
	memset(videomemory, 0, videomemorysize);

	// info->par 被分配了 sizeof(u32) * 16 字节
	info = framebuffer_alloc(sizeof(u32) * 16, &dev->dev);
	if (!info)
		goto err;

	// 虚拟地址
	info->screen_base = (char __iomem *)videomemory;
	info->screen_size = videomemorysize;
	
	// spi_qtft_ops 在 ops.c 中定义
	info->fbops = &spi_qtft_ops;

	info->var = spi_qtft_var_default;

	spi_qtft_fix_default.smem_start = (unsigned long) videomemory;
	spi_qtft_fix_default.smem_len = videomemorysize;
	info->fix = spi_qtft_fix_default;
	
	// 16色伪调色板作为私有数据存放在 info->par
	info->pseudo_palette = info->par;
	info->par = NULL;
	info->flags = FBINFO_DEFAULT;

	// 为16色伪调色板分配内存
	retval = fb_alloc_cmap(&info->cmap, 16, 0);
	if (retval < 0)
		goto err1;

	retval = register_framebuffer(info);
	if (retval < 0)
		goto err2;

	// 将 info 指针存入平台设备私有数据
	platform_set_drvdata(dev, info);

	printk(KERN_INFO "SPI QVGA TFT LCD driver: fb%d, %ldK video memory\n", info->node, videomemorysize >> 10);
	retval = 0;
	goto out;

err2:
	fb_dealloc_cmap(&info->cmap);
err1:
	framebuffer_release(info);
err:
	rvfree(videomemory, videomemorysize);
out:
	func_out();
	return retval;
}

static int spi_qtft_remove(struct platform_device *dev)
{
	struct fb_info *info = platform_get_drvdata(dev);

	func_in();
	if (info) {
		unregister_framebuffer(info);
		rvfree(videomemory, videomemorysize);
		fb_dealloc_cmap(&info->cmap);
		framebuffer_release(info);
	}
	func_out();
	return 0;
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
	int err = 0;
	func_in();

	err = platform_driver_register(&spi_qtft_driver);
	if(err<0)
		goto out;

	spi_qtft_device = platform_device_alloc("spi_qtft", 0);
	if (!spi_qtft_device)
	{
		err = -ENOMEM;
		goto unregister;
	}

	err = platform_device_add(spi_qtft_device);
	if (err<0)
		goto device_put;

	goto out;

device_put:
	platform_device_put(spi_qtft_device);
unregister:
	platform_driver_unregister(&spi_qtft_driver);
out:
	func_out();
	return err;
}

static void __exit spi_qtft_exit(void)
{
	func_in();

	platform_device_unregister(spi_qtft_device);
	platform_driver_unregister(&spi_qtft_driver);

	func_out();
}

module_init(spi_qtft_init);
module_exit(spi_qtft_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("SPI QVGA TFT LCD driver");