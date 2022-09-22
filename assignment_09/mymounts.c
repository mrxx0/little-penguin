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

static ssize_t mymounts_list(struct file *file, char __user *str, size_t len, loff_t *offset)
{
	// nsproxy is shared by tasks which share all namespaces and we want mnt_namespace
	struct mnt_namespace *mnt_ns = current->nsproxy->mnt_ns;
	struct mount *mnt;
	struct path mnt_path;
	char *path = kmalloc(PATH_MAX + 1, GFP_KERNEL);
	if (path == NULL)
	{
		kfree(path);
		return -ENOMEM;
	}
	memset(path_list, 0, sizeof(char) * PATH_MAX);
	list_for_each_entry(mnt, &mnt_ns->list, mnt_list) {
		mnt_path.mnt = &mnt->mnt;
		mnt_path.dentry = mnt->mnt.mnt_root;
		path = d_path(&mnt_path, path_list, PATH_MAX);
		//pr_info("%s", mnt->mnt_devname);
		pr_info("%-10s%s\n",mnt->mnt_devname, path);
		if (IS_ERR(path))
			return -1;
	//	pr_info("%s\n", path);
	}
	kfree(path);
	return 0;
}

static const struct proc_ops mymounts_procops = {
	.proc_read = &mymounts_list
};

static int __init init_mymounts(void)
{
	mymounts = proc_create("mymounts", 0444, NULL, &mymounts_procops);
	if (mymounts == NULL){
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
