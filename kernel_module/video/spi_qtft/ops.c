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

extern struct fb_var_screeninfo spi_qtft_var_default;
extern struct fb_fix_screeninfo spi_qtft_fix_default;

ssize_t ops_read(struct fb_info *info, char __user *buf, size_t count, loff_t *ppos)
{
	func_in();
	// 在 drivers/vedio/fb_sys_fops.c 中定义
	fb_sys_read(info,buf,count,ppos);
	func_out();
	return 0;
}

ssize_t ops_write(struct fb_info *info, const char __user *buf, size_t count, loff_t *ppos)
{
	func_in();
	// 在 drivers/vedio/fb_sys_fops.c 中定义
	fb_sys_write(info,buf,count,ppos);
	func_out();
	return 0;
}

int ops_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
	func_in();
	*var = spi_qtft_var_default;
	func_out();
	return 0;
}

int ops_set_par(struct fb_info *info)
{
	func_in();
	func_out();
	return 0;
}

int ops_blank(int blank, struct fb_info *info)
{
	func_in();
	func_out();
	return 0;
}

int ops_setcolreg(unsigned regno, unsigned red, unsigned green, unsigned blue, unsigned transp, struct fb_info *info)
{
	int err=0;
	u32 v;
	func_in();

	if (regno >= 16)
	{
		err = 1;
		goto out;
	}

	v = (red 	<< info->var.red.offset) 	|
	    (green 	<< info->var.green.offset) 	|
	    (blue 	<< info->var.blue.offset)	|
	    (transp << info->var.transp.offset)	;

	((u32 *) (info->pseudo_palette))[regno] = v;

out:
	func_out();
	return err;
}

int ops_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg)
{
	func_in();

	func_out();
	return 0;
}

struct fb_ops spi_qtft_ops = 
{
	.owner = THIS_MODULE,
	
	.fb_read      = ops_read,
	.fb_write     = ops_write,

	.fb_check_var = ops_check_var,
	.fb_set_par   = ops_set_par,
	.fb_setcolreg = ops_setcolreg,
	.fb_blank     = ops_blank,
	.fb_ioctl     = ops_ioctl,
};
