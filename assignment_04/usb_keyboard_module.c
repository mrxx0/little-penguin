// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/hid.h>

static int __init usb_init_module(void)
{
	pr_info("usb_keyboard_module : USB keyboard connected !\n");
	return 0;
}

static void __exit usb_cleanup_module(void)
{
	pr_info("usb_keyboard_module : USB keyboard disconnected !\n");
}

module_init(usb_init_module);
module_exit(usb_cleanup_module);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("chcoutur");
MODULE_DESCRIPTION("A simple hello world module.");
