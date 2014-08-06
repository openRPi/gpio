#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

static struct cdev chr_dev;
static struct class * chr_dev_class;
static dev_t ndev;

static int chr_udev_dev_open(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "chr_udev_dev open\n");
	return 0;
}

static int chr_udev_dev_release(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "chr_udev_dev release\n");
	return 0;
}

struct file_operations chr_udev_dev_fops =
{
	.owner = THIS_MODULE,
	.open = chr_udev_dev_open,
	.release = chr_udev_dev_release,
};

static int __init chr_udev_dev_init(void)
{
	int ret;
	cdev_init(&chr_dev,&chr_udev_dev_fops);
	ret = alloc_chrdev_region(&ndev,0,1,"mknod_dev");
	if(ret<0)
		return ret;
	printk(KERN_INFO "chr_udev_dev init, major=%d, minor=%d\n",MAJOR(ndev),MINOR(ndev));
	ret = cdev_add(&chr_dev,ndev,1);
	if(ret<0)
		return ret;

	chr_dev_class = class_create(THIS_MODULE,"chr_udev_dev");
	device_create(chr_dev_class,NULL,ndev,NULL,"chr_udev_dev");

	return 0;
}

static void __exit chr_udev_dev_exit(void)
{
	cdev_del(&chr_dev);
	unregister_chrdev_region(ndev,1);
	device_destroy(chr_dev_class,ndev);
	printk(KERN_INFO "chr_udev_dev exit\n");
}

module_init(chr_udev_dev_init);
module_exit(chr_udev_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A chr_udev_dev");
