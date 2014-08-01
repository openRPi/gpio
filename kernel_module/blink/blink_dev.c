#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

/* 所有混杂设备的主设备号都为10
 * 混杂类目录 /sys/class/misc
 * 设备目录 /dev/blink_dev
 */

#define GPIO_BASE	0x20200000
#define GPFSEL1		0x0004
#define GPSET0		0x001c
#define GPCLR0		0x0028

volatile int * gpio;
static char echo_buf[50];

static int blink_dev_open(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "blink_dev open\n");
	return 0;
}

static int blink_dev_release(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "blink_dev release\n");
	return 0;
}

static ssize_t blink_dev_read(struct file * filp, char __user * up, size_t size, loff_t * loffp)
{
	int min = size<sizeof(echo_buf)? size:sizeof(echo_buf);
	printk(KERN_INFO "blink_dev_read size=%d\n",size);
	if(copy_to_user(up,echo_buf,min))
		return -EFAULT;
	return min;
}

static ssize_t blink_dev_write(struct file * filp, const char __user * up, size_t size, loff_t * loffp)
{
	int min = size<sizeof(echo_buf)? size:sizeof(echo_buf);
	printk(KERN_INFO "blink_dev_write size=%d\n",size);
	if(copy_from_user(echo_buf,up, min ))
		return -EFAULT;
	return min;
}

struct file_operations blink_dev_fops =
{
	.owner = THIS_MODULE,
	.open = blink_dev_open,
	.release = blink_dev_release,
	.write = blink_dev_write,
	.read = blink_dev_read,
};

static struct miscdevice blink_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "blink_dev",
	.fops = &blink_dev_fops,
};

static int __init blink_dev_init(void)
{
	misc_register(&blink_dev);
	printk(KERN_INFO "blink_dev init\n");
	return 0;
}

static void __exit blink_dev_exit(void)
{
	misc_deregister(&blink_dev);
	printk(KERN_INFO "blink_dev exit\n");
}

module_init(blink_dev_init);
module_exit(blink_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A blink_dev");
