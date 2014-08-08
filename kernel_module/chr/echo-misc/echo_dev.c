#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

/* 所有混杂设备的主设备号都为10
 * 混杂类目录 /sys/class/misc
 * 设备目录 /dev/echo_dev
 */

static char echo_buf[50];

static int echo_dev_open(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "echo_dev open\n");
	return 0;
}

static int echo_dev_release(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "echo_dev release\n");
	return 0;
}

static ssize_t echo_dev_read(struct file * filp, char __user * up, size_t size, loff_t * loffp)
{
	int min = size<sizeof(echo_buf)? size:sizeof(echo_buf);
	printk(KERN_INFO "echo_dev_read size=%d\n",size);
	if(copy_to_user(up,echo_buf,min))
		return -EFAULT;
	return min;
}

static ssize_t echo_dev_write(struct file * filp, const char __user * up, size_t size, loff_t * loffp)
{
	int min = size<sizeof(echo_buf)? size:sizeof(echo_buf);
	printk(KERN_INFO "echo_dev_write size=%d\n",size);
	if(copy_from_user(echo_buf,up, min ))
		return -EFAULT;
	return min;
}

struct file_operations echo_dev_fops =
{
	.owner = THIS_MODULE,
	.open = echo_dev_open,
	.release = echo_dev_release,
	.write = echo_dev_write,
	.read = echo_dev_read,
};

static struct miscdevice echo_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "echo_dev",
	.fops = &echo_dev_fops,
};

static int __init echo_dev_init(void)
{
	misc_register(&echo_dev);
	printk(KERN_INFO "echo_dev init\n");
	return 0;
}

static void __exit echo_dev_exit(void)
{
	misc_deregister(&echo_dev);
	printk(KERN_INFO "echo_dev exit\n");
}

module_init(echo_dev_init);
module_exit(echo_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A echo_dev");
