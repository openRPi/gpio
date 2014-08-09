#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

static struct cdev chr_dev;
static dev_t ndev;

static int chr_mknod_dev_open(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "chr_mknod_dev open\n");
	return 0;
}

static int chr_mknod_dev_release(struct inode * ip, struct file * fp)
{
	printk(KERN_INFO "chr_mknod_dev release\n");
	return 0;
}

struct file_operations chr_mknod_dev_fops =
{
	.owner = THIS_MODULE,
	.open = chr_mknod_dev_open,
	.release = chr_mknod_dev_release,
};

static int __init chr_mknod_dev_init(void)
{
	int ret;
	cdev_init(&chr_dev,&chr_mknod_dev_fops);
	ret = alloc_chrdev_region(&ndev,0,1,"mknod_dev");
	if(ret<0)
		return ret;
	printk(KERN_INFO "chr_mknod_dev init, major=%d, minor=%d\n",MAJOR(ndev),MINOR(ndev));
	ret = cdev_add(&chr_dev,ndev,1);
	if(ret<0)
		return ret;
	return 0;
}

static void __exit chr_mknod_dev_exit(void)
{
	cdev_del(&chr_dev);
	unregister_chrdev_region(ndev,1);
	printk(KERN_INFO "chr_mknod_dev exit\n");
}

module_init(chr_mknod_dev_init);
module_exit(chr_mknod_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A chr_mknod_dev");
