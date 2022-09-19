// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>

static struct dentry *directory_entry = NULL;
static struct dentry *id_entry = NULL;
static struct dentry *jiffies_entry = NULL;
static struct dentry *foo_entry = NULL;


static ssize_t id_read(struct file *file, char __user *str, size_t len, loff_t *offset)
{
	return simple_read_from_buffer(str, len, offset, "chcoutur", strlen("chcoutur"));
}

static ssize_t id_write(struct file *file, const char __user *str, size_t len, loff_t *offset)
{
	char tmp_str[9];
	ssize_t size = simple_write_to_buffer(tmp_str, sizeof(tmp_str), offset, str, len);

	if (size == strlen("chcoutur") && memcmp(tmp_str, str, size) == 0)
		return len;
	else
		return -EINVAL;
}

static struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write
};

static ssize_t jiffies_read(struct file *file, char __user *str, size_t len, loff_t *offset)
{
	volatile unsigned long j = jiffies;
	char tmp[64];
	ssize_t size = snprintf(tmp, sizeof(tmp), "%lu", j);
	return simple_read_from_buffer(str, len, offset, tmp, size);
}

static ssize_t jiffies_write(struct file *file, const char __user *str, size_t len, loff_t *offset)
{
	return len;
}
static struct file_operations jiffies_fops = {
	.owner = THIS_MODULE,
	.read = jiffies_read,
	.write = jiffies_write
};

DEFINE_MUTEX (foo_mutex);
static char foo_buff[PAGE_SIZE];
static size_t foo_size = 0;

static ssize_t foo_read(struct file *file, char __user *str, size_t len, loff_t *offset)
{
	ssize_t size;

	mutex_lock(&foo_mutex);
	size = simple_read_from_buffer(str, len, offset, foo_buff, foo_size);
	mutex_unlock(&foo_mutex);
	return size;
}

static ssize_t foo_write(struct file *file, const char __user *str, size_t len, loff_t *offset)
{
	size_t size = 0;

	mutex_lock(&foo_mutex);
	size = simple_write_to_buffer(foo_buff, sizeof(foo_buff), offset, str, len);
	foo_size = size;
	mutex_unlock(&foo_mutex);
	return size;
}

static struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write
};
static void clean(void)
{
	debugfs_remove(id_entry);
	debugfs_remove(jiffies_entry);
	debugfs_remove(foo_entry);
	debugfs_remove(directory_entry);
}

static int __init init_debugfs(void)
{
	pr_info("Init successful !\n");

	directory_entry = debugfs_create_dir("fortytwo", NULL);
	if (!directory_entry)
	{
		return -1;
	}
	id_entry = debugfs_create_file("id", 0666, directory_entry, NULL, &id_fops);
	if (!id_entry)
	{
		return -1;
	}
	jiffies_entry = debugfs_create_file("jiffies", 0444, directory_entry, NULL, &jiffies_fops);
	if (!jiffies_entry)
	{
		return -1;
	}
	foo_entry = debugfs_create_file("foo", 0644, directory_entry, NULL, &foo_fops);
	if (!foo_entry)
	{
		return -1;
	}
	return 0;
}

static void __exit cleanup_debugfs(void)
{
	pr_info("Cleanup successful !\n");
	clean();
}

module_init(init_debugfs);
module_exit(cleanup_debugfs);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("chcoutur");
MODULE_DESCRIPTION("A simple debugfs module.");
