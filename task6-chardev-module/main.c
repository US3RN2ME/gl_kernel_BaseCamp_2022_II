// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Chrdev module");
MODULE_AUTHOR("Tereshchenko Dmytro <buxdmo@gmail.com>");
MODULE_VERSION("1.0");

#undef pr_fmt
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#define CHRDEV_CLASS_NAME "chrdev-module"
#define CHRDEV_DEVICE_NAME "chrdev-module"
#define CHRDEV_BUFFER_SIZE 1024

static struct class *chrdev_class;
static struct device *chrdev_device;
static struct cdev chrdev_cdev;
static dev_t chrdev_dev;

static int chrdev_major;
static int chrdev_is_open;

static int chrdev_buffer_size;
static unsigned char chrdev_buffer[CHRDEV_BUFFER_SIZE];

/*===============================================================================================*/
static int chrdev_open(struct inode *inode, struct file *file)
{
    // TODO: Implement this function
    return 0;
}
/*===============================================================================================*/
static int chrdev_release(struct inode *inode, struct file *file)
{
    // TODO: Implement this function
    return 0;
}
/*===============================================================================================*/
static ssize_t chrdev_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    // TODO: Implement this function
    return 0;
}
/*===============================================================================================*/
static ssize_t chrdev_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{
    // TODO: Implement this function
    return 0;
}
/*===============================================================================================*/
static struct file_operations chrdev_fops = {
    .owner = THIS_MODULE,
    .open = chrdev_open,
    .release = chrdev_release,
    .read = chrdev_read,
    .write = chrdev_write,
};
/*===============================================================================================*/
static int __init chrdev_module_init(void)
{
	pr_info("module init\n");

	chrdev_major = alloc_chrdev_region(&chrdev_dev, 0, 1, CHRDEV_DEVICE_NAME);
	if (chrdev_major < 0) {
		pr_err("alloc_chrdev_region failed: %d\n", chrdev_major);
		return chrdev_major;
	}

	cdev_init(&chrdev_cdev, &chrdev_fops);
	if (cdev_add(&chrdev_cdev, chrdev_dev, 1) < 0) {
		pr_err("cdev_add failed\n");
		goto cdev_add_err;
	}

	chrdev_class = class_create(THIS_MODULE, CHRDEV_CLASS_NAME);
	if (IS_ERR(chrdev_class)) {
		pr_err("class_create failed\n");
		goto class_create_err;
	}

	chrdev_device = device_create(chrdev_class, NULL, chrdev_dev, NULL, CHRDEV_CLASS_NAME"0");
	if (IS_ERR(chrdev_device)) {
		pr_err("device_create failed\n");
		goto device_create_err;
	}

	pr_info("create device with major = %d, minor = %d\n", MAJOR(chrdev_dev), MINOR(chrdev_dev));

	return 0;

device_create_err:
	class_destroy(chrdev_class);
class_create_err:
	cdev_del(&chrdev_cdev);
cdev_add_err:
	unregister_chrdev_region(chrdev_dev, 1);

	return -1;
}
/*===============================================================================================*/
static void __exit chrdev_module_exit(void)
{
	pr_info("module exit\n");
	device_destroy(chrdev_class, chrdev_dev);
	class_destroy(chrdev_class);
	cdev_del(&chrdev_cdev);
	unregister_chrdev_region(chrdev_dev, 1);
}
/*===============================================================================================*/

module_init(chrdev_module_init);
module_exit(chrdev_module_exit);
