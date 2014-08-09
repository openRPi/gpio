#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

/* 所有混杂设备的主设备号都为10
 * 混杂类目录 /sys/class/misc
 * 设备目录 /dev/buf_dev
 */

static char buf_array[50];

static int buf_dev_open(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "buf_dev open\n");
	return 0;
}

static int buf_dev_release(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "buf_dev release\n");
	return 0;
}

static ssize_t buf_dev_read(struct file * filp, char __user * up, size_t size, loff_t * off)
{
	int min = (sizeof(buf_array) - filp->f_ops) - off < size ? : size ;
	if(copy_to_user(up, buf_array + filp->f_ops + off, min))
		return -EINVAL;
	printk(KERN_INFO "buf_dev read. request_size=%d, request_off=%d, f_ops=%d, real_readsize=%d\n",size,off,filp->f_ops,min);
	return min;
}

static ssize_t buf_dev_write(struct file * filp, const char __user * up, size_t size, loff_t * off)
{
	int min = (sizeof(buf_array) - filp->f_ops) - off < size ? : size ;
	if(copy_from_user(buf_array + filp->f_ops, up, min))
		return -EINVAL;
	printk(KERN_INFO "buf_dev write. request_size=%d, request_off=%d, f_ops=%d, real_writesize=%d\n",size,off,filp->f_ops,min);
	return min;
}

static loff_t bur_dev_llseek(struct file * filp, loff_t off, int where)
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
			newpos = sizeof(buf_array) + off;
			break;

		default :
			return -EINVAL;
	}
	if (newpos < 0 || newpos > sizeof(buf_array)-1)
		return -EINVAL;

	filp->f_pos = newpos;
	printk(KERN_INFO "buf_dev llseek. f_pos=%d\n",filp->f_pos);
	return newpos;
}

struct file_operations buf_dev_fops =
{
	.owner = THIS_MODULE,
	.open = buf_dev_open,
	.release = buf_dev_release,
	.write = buf_dev_write,
	.read = buf_dev_read,
	.llseek = bur_dev_llseek,
};

static struct miscdevice buf_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "buf_dev",
	.fops = &buf_dev_fops,
};

static int __init buf_dev_init(void)
{
	misc_register(&buf_dev);
	printk(KERN_INFO "buf_dev init\n");
	return 0;
}

static void __exit buf_dev_exit(void)
{
	misc_deregister(&buf_dev);
	printk(KERN_INFO "buf_dev exit\n");
}

module_init(buf_dev_init);
module_exit(buf_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A buf_dev");
