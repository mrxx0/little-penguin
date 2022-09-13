#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/hid.h>

int probe(struct usb_interface *intf, const struct usb_device_id *id)
{
		printk(KERN_INFO "USB keyboard plugged in.\n");
		return 0;
}

void disconnect (struct usb_interface *intf)
{
	printk(KERN_INFO "USB keyboard plugged out.\n");
}

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

/*
 * Identifies the USB device driver to usbcore
 */
static struct usb_driver chcoutur_usb_driver = {
	.name = "chcoutur_usb_driver",
	.id_table = id_table,
	.probe = probe,
	.disconnect = disconnect,
};

static int __init usb_init_module(void)
{
		printk(KERN_INFO "Hello world!\n");
		usb_register(&chcoutur_usb_driver);
		return 0;
}

static void __exit usb_cleanup_module(void)
{
		printk(KERN_INFO "Cleaning up module.\n");
		usb_deregister(&chcoutur_usb_driver);
}

module_init(usb_init_module);
module_exit(usb_cleanup_module);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("chcoutur");
MODULE_DESCRIPTION("A simple module to load a USB keyboard.");
