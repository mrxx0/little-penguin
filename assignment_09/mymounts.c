// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/nsproxy.h>
#include <../fs/mount.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>

static struct proc_dir_entry *mymounts;
char path_list[PATH_MAX];

static void get_size(struct mnt_namespace *mnt_ns, int *size)
{
	struct mount *mnt;
	struct path mnt_path;
	size_t len;
	size_t maxlen;
	char *path;

	maxlen = 0;
	len = 0;
	path = NULL;
	list_for_each_entry(mnt, &mnt_ns->list, mnt_list) {
		if (mnt->mnt_id != 1) {
			len = strlen(mnt->mnt_devname);
			if (len > maxlen)
				maxlen = len;
			mnt_path.mnt = &mnt->mnt;
			mnt_path.dentry = mnt->mnt.mnt_root;
			path = d_path(&mnt_path, path_list, PATH_MAX);
			size[0] += strlen(path);
		}
	}
	size[1] = maxlen + 8;
	size[0] += (size[1] + 1) * (mnt_ns->mounts - 1);
}

static ssize_t mymounts_list(struct file *file, char __user *str, size_t len, loff_t *offset)
{
	// nsproxy is shared by tasks which share all namespaces and we want mnt_namespace
	struct mnt_namespace *mnt_ns = current->nsproxy->mnt_ns;
	struct mount *mnt;
	struct path mnt_path;
	char *path;
	char *output;
	int cursor;
	int size[2];
	ssize_t ret;

	path = NULL;
	cursor = 0;
	size[0] = 0; // size[0] is the total len of the print
	size[1] = 0; // size[1] is the len to padd each result (depends on the longest mnt_devname)
	ret = 0;
	// needed in case a mnt_devname is too long
	get_size(mnt_ns, size);
	output = kmalloc(size[0], GFP_KERNEL);
	if (!output)
		return -ENOMEM;
	memset(path_list, 0, sizeof(char) * PATH_MAX);
	memset(output, 0, size[0]);
	list_for_each_entry(mnt, &mnt_ns->list, mnt_list) {
		if (mnt->mnt_id != 1) {
			mnt_path.mnt = &mnt->mnt;
			mnt_path.dentry = mnt->mnt.mnt_root;
			path = d_path(&mnt_path, path_list, PATH_MAX);
			if (IS_ERR(path)) {
				kfree(output);
				return -1;
			}
			cursor += sprintf(output + cursor, "%-*s%s\n", size[1],
					mnt->mnt_devname, path);
		}
	}
	ret = simple_read_from_buffer(str, len, offset, output, size[0]);
	kfree(output);
	return ret;
}

static const struct proc_ops mymounts_procops = {
	.proc_read = &mymounts_list
};

static int __init init_mymounts(void)
{
	mymounts = proc_create("mymounts", 0444, NULL, &mymounts_procops);
	if (!mymounts) {
		pr_err("Error while creating file.\n");
		return -1;
	}
	pr_info("mymounts : Init succesful !\n");
	return 0;
}

static void __exit cleanup_mymounts(void)
{
	proc_remove(mymounts);
	pr_info("mymounts : Cleanup successful !\n");
}

module_init(init_mymounts);
module_exit(cleanup_mymounts);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("chcoutur");
MODULE_DESCRIPTION("A simple mount listing module.");

