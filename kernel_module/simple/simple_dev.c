#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/io.h>

/* 所有混杂设备的主设备号都为10
 * 混杂类目录 /sys/class/misc
 * 设备目录 /dev/simple_dev
 */

static int simple_dev_open(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "simple_dev open\n");
	return 0;
}

static int simple_dev_release(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "simple_dev release\n");
	return 0;
}

struct file_operations simple_dev_fops =
{
	.owner = THIS_MODULE,
	.open = simple_dev_open,
	.release = simple_dev_release,
};

static struct miscdevice simple_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "simple_dev",
	.fops = &simple_dev_fops,
};

static int __init simple_dev_init(void)
{
	misc_register(&simple_dev);
	printk(KERN_INFO "simple_dev init\n");
	return 0;
}

static void __exit simple_dev_exit(void)
{
	misc_deregister(&simple_dev);
	printk(KERN_INFO "simple_dev exit\n");
}

module_init(simple_dev_init);
module_exit(simple_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A simple_dev");
