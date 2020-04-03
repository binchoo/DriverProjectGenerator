#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#include "{header}"
#define DEV_NAME "{dev_name}"
MODULE_LICENSE({license});

static int {dev_name}_open(struct inode* inode, struct file* file) {

	return 0;
}

static int {dev_name}_release(struct inode* inode, struct file* file) {

	return 0;
}

static ssize_t {dev_name}_read(struct file* file, char* buf, size_t len, loff_t* lof) {

	return 0;
}

static ssize_t {dev_name}_write(struct file* file, const char* buf, size_t len, loff_t* lof) {

	return len;
}

static long {dev_name}_ioctl(struct file* file, unsigned int cmd, unsigned long arg) {

	switch(cmd) {
		case IOCTL_FUNC1:
			break;

		case IOCTL_FUNC2:
			break;

		case IOCTL_FUNC3:
			break;

		default :
			break;
	}
	return 0;
}

static struct file_operations {dev_name}_fops = {
	.open = {dev_name}_open,
	.release = {dev_name}_release,
	.read = {dev_name}_read,
	.write = {dev_name}_write,
	.unlocked_ioctl = {dev_name}_ioctl,
};

static dev_t dev_num;
static struct cdev* cd_cdev;

static int __init {dev_name}_init(void) {
	alloc_chrdev_region(&dev_num, 0, {dev_count}, DEV_NAME);
	cd_cdev = cdev_alloc();
	cdev_init(cd_cdev, &{dev_name}_fops);
	cdev_add(cd_cdev, dev_num, {dev_count});

	return 0;
}
module_init({dev_name}_init);

static void __exit {dev_name}_exit(void) {
	cdev_del(cd_cdev);
	unregister_chrdev_region(dev_num, {dev_count});
}
module_exit({dev_name}_exit);
