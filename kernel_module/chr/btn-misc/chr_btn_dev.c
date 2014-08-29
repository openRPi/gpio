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

static int btn_list[] = {4,17,18,27,22,23,24,25};

static irqreturn_t btn_interrput(int irq, void *dev)
{
	printk(KERN_INFO "++ btn_interrput ++\n");
	printk(KERN_INFO "IRQ=%d, GPIO_NUM=%d\n",irq, irq_to_gpio(irq));
	printk(KERN_INFO "-- btn_interrput --\n");
	return IRQ_HANDLED;
}

static int __init chr_btn_dev_init(void)
{
	printk(KERN_INFO "++ chr_btn_dev init ++\n");

	for(int i=0;i<ARRAY_SIZE(btn_list);i++)
	{
		int btn_irq = gpio_to_irq(btn_list[i]);
		if(request_irq(	btn_irq,
						btn_interrput,
						IRQF_TRIGGER_RISING,
						"chr_btn_dev",
						NULL))
		{
			printk(KERN_ERR "Can't register IRQ %d\n",btn_irq);
			return -EIO;
		}
		else
			printk(KERN_ERR "Register IRQ %d\n",btn_irq);
	}

	printk(KERN_INFO "-- chr_btn_dev init --\n");
	return 0;
}

static void __exit chr_btn_dev_exit(void)
{
	int btn_irq;

	printk(KERN_INFO "++ chr_btn_dev exit ++\n");

	for(int i=0;i<ARRAY_SIZE(btn_list);i++)
	{
		int btn_irq = gpio_to_irq(btn_list[i]);
		free_irq(btn_irq,NULL);
		printk(KERN_INFO "release IRQ %d\n",btn_irq);
	}

	printk(KERN_INFO "-- chr_btn_dev exit --\n");
}

module_init(chr_btn_dev_init);
module_exit(chr_btn_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A chr_btn_dev");
