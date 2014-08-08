#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/io.h>

/* 所有混杂设备的主设备号都为10
 * 混杂类目录 /sys/class/misc
 * 设备目录 /dev/chr_misc_dev
 */

static int chr_misc_dev_open(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "chr_misc_dev open\n");
	return 0;
}

static int chr_misc_dev_release(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "chr_misc_dev release\n");
	return 0;
}

struct file_operations chr_misc_dev_fops =
{
	.owner = THIS_MODULE,
	.open = chr_misc_dev_open,
	.release = chr_misc_dev_release,
};

static struct miscdevice chr_misc_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "chr_misc_dev",
	.fops = &chr_misc_dev_fops,
};

static int __init chr_misc_dev_init(void)
{
	misc_register(&chr_misc_dev);
	printk(KERN_INFO "chr_misc_dev init\n");
	return 0;
}

static void __exit chr_misc_dev_exit(void)
{
	misc_deregister(&chr_misc_dev);
	printk(KERN_INFO "chr_misc_dev exit\n");
}

module_init(chr_misc_dev_init);
module_exit(chr_misc_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A chr_misc_dev");
