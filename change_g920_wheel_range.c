#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int main (int argc, char *argv[])
{
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;
	struct udev_device *p_dev;
	int range;

	/* Check that an integer argument was passed and is between 40 and 900. */
	if (argc != 2) {
		printf("Please pass a wheel range as parameter between 40 and 900\n");
		exit(1);
	} 
	range = atoi(argv[1]);
	if (range < 40 || range > 900) {
		printf("Please pass a wheel range as parameter between 40 and 900\n");
		exit(1);
	} 

	/* Create the udev object */
	udev = udev_new();
	if (!udev) {
		printf("Can't create udev\n");
		exit(1);
	}
	
	/* Create a list of the devices in the 'input' subsystem, with ID_INPUT_JOYSTICK==1. */
	printf("Searching for Logitech G920 wheel...\n");
	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "hid");
	//udev_enumerate_add_match_property(enumerate, "ID_INPUT_JOYSTICK", "1");
	udev_enumerate_scan_devices(enumerate);
	devices = udev_enumerate_get_list_entry(enumerate);

	/* Identify the Logitech G920 wheel device (046d:c262). */
	udev_list_entry_foreach(dev_list_entry, devices) {
		const char *sys_path;
		
		sys_path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, sys_path);

		//printf("Sys Node Path: %s\n", sys_path);
		//printf("Device Node Path: %s\n", udev_device_get_devnode(dev));
		
        /* Check if we found a G920 wheel, then update range */
		p_dev = udev_device_get_parent_with_subsystem_devtype(
		       dev,
		       "usb",
		       "usb_device");
		
		/* Check that we actually found a parent device and that VID/PID match to a G920 */
		if ( p_dev && !strcmp(udev_device_get_sysattr_value(p_dev,"idVendor"), "046d") && !strcmp(udev_device_get_sysattr_value(p_dev, "idProduct"), "c262")) {
			printf("Logitech G920 identified with serial %s\n", udev_device_get_sysattr_value(p_dev, "serial"));
			printf("Current wheel range: %s\n", udev_device_get_sysattr_value(dev, "range"));

			/* Build the full sys_fs path to range element */
			char full_path[PATH_MAX];
			strcpy(full_path, sys_path);
			strcat(full_path, "/range");
			
			printf("Setting wheel range to: %i\n", range);

			/* Write new wheel range to range element */
			FILE *range_file;
			//printf("sys_fs path to range object: %s\n", full_path);
			range_file = fopen(full_path, "w");
			if (range_file != NULL) {
				fprintf(range_file, argv[1]);
				fclose(range_file);
			}
		}
		
		udev_device_unref(dev);
	}

	/* Free the enumerator object */
	udev_enumerate_unref(enumerate);
	udev_unref(udev);

	return 0;       
}
