CC = gcc
CFLAGS = -g -Wall

udev_examples:
	$(CC) $(CFLAGS) -o udev_enumerate udev_enumerate.c -ludev
	$(CC) $(CFLAGS) -o change_g920_wheel_range change_g920_wheel_range.c -ludev
	
default: udev_examples

all: default

clean:
	rm -f udev_enumerate change_g920_wheel_range
