/*
 *	interrupt button
 *	
 *	
 *	Copyright (C) 2014 concefly <h.wenjian@openrpi.org>
 *	Copyright (C) 2014 openRPi
 *	
 *		代码遵循GNU协议
 *	
 *	文档：
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define BTN_NUM	17

static irqreturn_t btn_interrput(int irq, void *dev)
{
	printk(KERN_INFO "++ btn_interrput ++\n");
	printk(KERN_INFO "irq=%d \n",irq);
	printk(KERN_INFO "-- btn_interrput --\n");
	return IRQ_HANDLED;
}

struct file_operations chr_btn_dev_fops =
{
	.owner = THIS_MODULE,
};

static struct miscdevice chr_btn_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "chr_btn_dev",
	.fops = &chr_btn_dev_fops,
};

static int __init chr_btn_dev_init(void)
{
	int btn_irq;

	printk(KERN_INFO "++ chr_btn_dev init ++\n");

	btn_irq = gpio_to_irq(BTN_NUM);

	if(request_irq(	btn_irq,
					btn_interrput,
					IRQF_TRIGGER_RISING,
					"chr_btn_dev",
					NULL))
	{
		printk(KERN_ERR "Can't register IRQ %d\n",btn_irq);
		return -EIO;
	}
	printk(KERN_INFO "register IRQ %d\n",btn_irq);

	misc_register(&chr_btn_dev);

	printk(KERN_INFO "-- chr_btn_dev init --\n");
	return 0;
}

static void __exit chr_btn_dev_exit(void)
{
	int btn_irq;

	printk(KERN_INFO "++ chr_btn_dev exit ++\n");

	btn_irq = gpio_to_irq(BTN_NUM);

	free_irq(btn_irq,NULL);
	printk(KERN_INFO "release IRQ %d\n",btn_irq);

	misc_deregister(&chr_btn_dev);

	printk(KERN_INFO "-- chr_btn_dev exit --\n");
}

module_init(chr_btn_dev_init);
module_exit(chr_btn_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A chr_btn_dev");
