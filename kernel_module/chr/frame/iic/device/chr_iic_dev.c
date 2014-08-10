#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include <linux/errno.h>
#include <linux/i2c.h>

#define BUS_NUM 2

struct i2c_adapter * adap=NULL;
struct i2c_client * client=NULL;

static struct i2c_board_info chr_iic_dev_board_info[] __initdata = {
	{
		I2C_BOARD_INFO("chr_iic_dev",0x10),
	}
};

static int __init chr_iic_dev_init(void)
{
	printk(KERN_INFO "++ chr_iic_dev_init ++\n");

	adap = i2c_get_adapter(BUS_NUM);
	if (!adap)
		return -ENODEV;
	printk(KERN_INFO "get adap (bus %d)\n",BUS_NUM);

	client = i2c_new_device(adap,chr_iic_dev_board_info);
	if(client)
		printk(KERN_INFO "add new device. name=%s, addr=%x \n",client->name, client->addr);
	else
		return -ENODEV;

	printk(KERN_INFO "-- chr_iic_dev_init --\n");
	return 0;
}

static void __exit chr_iic_dev_exit(void)
{
	printk(KERN_INFO "++ chr_iic_dev_exit ++\n");

	if(client)
	{
		i2c_unregister_device(client);
		printk(KERN_INFO "release device.\n");
	}

	if(adap)
	{
		i2c_put_adapter(adap);
		printk(KERN_INFO "release adap (bus %d)\n",BUS_NUM);
	}
	printk(KERN_INFO "-- chr_iic_dev_exit --\n");
}

module_init(chr_iic_dev_init);
module_exit(chr_iic_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h.wenjian@openrpi.org");
MODULE_DESCRIPTION("A chr_iic_dev");
