// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/hid.h>

/*
 * Use to support hotplugging for a specific device
 */
static const struct usb_device_id id_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{}
};

/*
 * Allow hotplug and which modules work with what hardware devices
 */
MODULE_DEVICE_TABLE(usb, id_table);

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
