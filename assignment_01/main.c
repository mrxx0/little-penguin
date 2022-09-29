// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
	pr_info("Hello world!\n");
	return 0;
}

void cleanup_module(void)
{
	pr_info("Cleaning up module.\n");
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("chcoutur");
MODULE_DESCRIPTION("A simple hello world module.");
