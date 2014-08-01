#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/gpio.h>

/* 所有混杂设备的主设备号都为10
 * 混杂类目录 /sys/class/misc
 * 设备目录 /dev/blink_dev
 */

#define GPIO_NUM 17

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
	char on[2];
	
	gpio_direction_input(GPIO_NUM);
	on[0] = gpio_get_value(GPIO_NUM)? '1':'0';
	on[1] = '\n';
	printk(KERN_INFO "blink_dev_read: %c\n",on[0]);
	if(copy_to_user(up,on,2))
		return -EFAULT;
	return size;
}

static ssize_t blink_dev_write(struct file * filp, const char __user * up, size_t size, loff_t * loffp)
{
	char on[1];

	if(copy_from_user(on,up, 1 ))
		return -EFAULT;
	printk(KERN_INFO "blink_dev_write: %c\n",on[0]);
	gpio_direction_output(GPIO_NUM,0);
	gpio_set_value(GPIO_NUM,on[0]=='0'?0:1);
	return size;
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
	if(gpio_request(GPIO_NUM,"P1-11")<0)
		return -EBUSY;
	misc_register(&blink_dev);
	printk(KERN_INFO "blink_dev init GPIO=%d\n",GPIO_NUM);
	return 0;
}

static void __exit blink_dev_exit(void)
{
	misc_deregister(&blink_dev);
	gpio_free(GPIO_NUM);
	printk(KERN_INFO "blink_dev exit\n");
}

module_init(blink_dev_init);
module_exit(blink_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A blink_dev");
