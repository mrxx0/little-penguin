// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

static ssize_t device_read(struct file *file, char __user *str, size_t len, loff_t *offset)
{
	return simple_read_from_buffer(str, len, offset, "chcoutur", strlen("chcoutur"));
}

static ssize_t device_write(struct file *file, const char __user *str, size_t len, loff_t *offset)
{
	char tmp_str[9];
	ssize_t size = simple_write_to_buffer(tmp_str, sizeof(tmp_str), offset, str, len);

	if (size == strlen("chcoutur") && memcmp(tmp_str, str, size) == 0)
		return len;
	else
		return -EINVAL;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &fops
};

static int __init init_driver(void)
{
	if (misc_register(&misc) == 0) {
		pr_info("misc_char_driver : Driver [%s] registered with success !\n", misc.name);
		return 0;
	}
	pr_info("misc_char_driver : Failed to register device [%s] ...\n", misc.name);
	return -1;
}

static void __exit cleanup_driver(void)
{
	misc_deregister(&misc);
	pr_info("misc_char_driver : Driver [%s] deregistered with success !\n", misc.name);
}

module_init(init_driver);
module_exit(cleanup_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("chcoutur");
MODULE_DESCRIPTION("A simple misc char device driver.");
