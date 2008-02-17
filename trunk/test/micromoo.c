/* Name: microMoo.c
 * Project: MicroMoo based on usbtiny driver
 * Author: Tim Stegeman
 * Creation Date: 2008-02-16
 */

#include <stdio.h>
#include <string.h>
#include <usb.h>

#define USBDEV_VENDOR		0x16D0
#define USBDEV_PRODUCT		0x0034

#define USB_REQ_ECHO		0
#define USB_REQ_SET		1
#define USB_REQ_GET		2

static void usage(char *name)
{
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "  %s set <red> <green> <blue> - sets the color\n", name);
    fprintf(stderr, "  %s get                      - returns the current color\n", name);
}

int main(int argc, char **argv) {
	struct usb_bus      *bus;
	struct usb_device   *dev;
	usb_dev_handle *handle = NULL;
	unsigned char buffer[8];
	int nBytes;
	if(argc < 2){
		usage(argv[0]);
		exit(1);
	}

	usb_init();
	usb_find_busses();
	usb_find_devices();

	for (bus=usb_get_busses(); bus; bus=bus->next) {
		for (dev=bus->devices; dev; dev=dev->next) {
			if (dev->descriptor.idVendor == USBDEV_VENDOR && dev->descriptor.idProduct == USBDEV_PRODUCT) {
				handle = usb_open(dev);
				break;
			}
		}
		if (handle) break;
	}
	if (!handle) {
		fprintf(stderr, "Warning: cannot open USB device: %s\n", usb_strerror());
		exit(1);
	}

	if(strcmp(argv[1], "set") == 0 && argc >= 5) {
		unsigned char red = atoi(argv[2]);
		unsigned char green = atoi(argv[3]);
		unsigned char blue = atoi(argv[4]);
		nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, USB_REQ_SET, (red + (green << 8)), blue, (char *)buffer, sizeof(buffer), 2000);
		if (nBytes != 0){
			fprintf(stderr, "Error reading from USB device: %s\n", usb_strerror());
		}
	} else if (strcmp(argv[1], "get") == 0) {
		nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, USB_REQ_GET, 0, 0, (char *)buffer, sizeof(buffer), 2000);
		if (nBytes != 3) {
			fprintf(stderr, "Error reading from USB device: %s\n", usb_strerror());
		} else {
			printf("Red  : %i\nGreen: %i\nBlue : %i\nRGB  : #%2X%2X%2X\n", buffer[0], buffer[1], buffer[2], buffer[0], buffer[1], buffer[2]);	
		}
	} else {
		usage(argv[0]);
		exit(1);
	}
	usb_close(handle);
	return 0;
}
