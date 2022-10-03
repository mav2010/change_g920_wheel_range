#!/usr/bin/env python3
import os
import pyudev
import time

context = pyudev.Context()
for udevice in context.list_devices(subsystem='input'):
 seat_id = udevice.get('ID_FOR_SEAT')
 print("seat id: %s" % seat_id)
 if 'DEVNAME' in udevice:
  if 'event' in udevice.get('DEVNAME'):
   usb_id = str(udevice.get('ID_VENDOR_ID')) + ':' + str(udevice.get('ID_MODEL_ID'))
   print("vendor_id: %s, product_id: %s, usb_id: %s, dev_name: %s" % (udevice.get('ID_VENDOR_ID'), udevice.get('ID_MODEL_ID'), usb_id, udevice.get('DEVNAME')))
 else:
  print("udevice.sys_path: %s" % udevice.sys_path)
  print("os.path.join(udevice.sys_path, 'device'): %s" % os.path.join(udevice.sys_path, 'device'))
  print("name: %s, dev_path: %s" % (udevice.get('NAME').strip('"'), os.path.realpath(os.path.join(udevice.sys_path, 'device'))))
