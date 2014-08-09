#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/gpio.h>

/* 所有混杂设备的主设备号都为10
 * 混杂类目录 /sys/class/misc
 * 设备目录 /dev/blink_array_dev
 */

#define LED_ON 1
#define LED_OFF 0

static int gpio_array[8] = {4,17,18,27,22,23,24,25};
static char *gpio_array_name[8] = {
	"P1-7",
	"P1-11",
	"P1-12",
	"P1-13",
	"P1-15",
	"P1-16",
	"P1-18",
	"P1-22",
};

static int blink_array_dev_open(struct inode * inode, struct file * filp)
{
	printk(KERN_INFO "blink_array_dev open\n");
	return 0;
}

static int blink_array_dev_release(struct inode * inode, struct file * filp)
{
	printk(KERN_INFO "blink_array_dev release\n");
	return 0;
}

static ssize_t blink_array_dev_read(struct file * filp, char __user * up, size_t size, loff_t * off)
{
	int value=0;
	int gpio_num=0;
	int re=0;

	if(*off<0 || *off>=sizeof(gpio_array))
	{
		re = -EINVAL;
		goto exit_read_no_gpio;
	}

	gpio_num = gpio_array[*off];

	if(gpio_request(gpio_num,gpio_array_name[*off])<0)
	{	
		re = -EBUSY;
		goto exit_read_no_gpio;
	}

	gpio_direction_input(gpio_num);
	value = gpio_get_value(gpio_num);

	printk(KERN_INFO "blink_array_dev_read. Pin=%s, value=%s\n",gpio_array_name[*off],value==LED_ON?"on":"off");

	if(copy_to_user(up,value==LED_ON?"1":"0",1))
	{	
		re = -EFAULT;
		goto exit_read;
	}

	re = 1;

	exit_read:
		gpio_free(gpio_num);
	exit_read_no_gpio:
		return re;
}

static ssize_t blink_array_dev_write(struct file * filp, const char __user * up, size_t size, loff_t * off)
{
	char value_chr;
	int re=0;
	int gpio_num = gpio_array[*off];

	if(copy_from_user(&value_chr,up, 1 ))
	{	
		re = -EFAULT;
		goto exit_write_no_gpio;
	}
	printk(KERN_INFO "blink_array_dev_write. Pin=%s value=%s\n",gpio_array_name[*off],value_chr=='0'?"off":"on");

	gpio_direction_output(gpio_num,LED_OFF);
	gpio_set_value(gpio_num,value_chr=='0'?LED_OFF:LED_ON);

	re = 1;

	exit_write:
		gpio_free(gpio_num);
	exit_write_no_gpio:
		return re;
}

static loff_t blink_array_dev_llseek(struct file * filp, loff_t off, int where)
{
	loff_t newpos;

	switch(where)
	{
		case 0: /* SEEK_SET */
			newpos = off;
			break;

		case 1: /* SEEK_CUR */
			newpos = filp->f_pos + off;
			break;

		case 2: /* SEEK_END */
			newpos = sizeof(gpio_array) + off;
			break;

		default :
			return -EINVAL;
	}
	if (newpos < 0 || newpos > sizeof(gpio_array)-1)
		return -EINVAL;

	filp->f_pos = newpos;
	printk(KERN_INFO "blink_array_dev llseek. f_pos=%d\n",(int)(filp->f_pos));
	return newpos;
}

struct file_operations blink_array_dev_fops =
{
	.owner = THIS_MODULE,
	.open = blink_array_dev_open,
	.release = blink_array_dev_release,
	.write = blink_array_dev_write,
	.read = blink_array_dev_read,
	.llseek  = blink_array_dev_llseek,
};

static struct miscdevice blink_array_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "blink_array_dev",
	.fops = &blink_array_dev_fops,
};

static int __init blink_array_dev_init(void)
{
	misc_register(&blink_array_dev);
	printk(KERN_INFO "blink_array_dev init\n");
	return 0;
}

static void __exit blink_array_dev_exit(void)
{
	misc_deregister(&blink_array_dev);
	printk(KERN_INFO "blink_array_dev exit\n");
}

module_init(blink_array_dev_init);
module_exit(blink_array_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A blink_array_dev");
