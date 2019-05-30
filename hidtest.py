#import hid

## Driver process for Windows. ##
# Download libusb32 and install (not sure if necessary? Probably not.)
# Download Zadig and run:
#  Find "Teensyduino RawHID (Interface 0)" in the drop down menu.
#  Change the Driver to "libusb-win32" ("libusbK" also seems to work (or maybe both need to be applied?))
#  Check the USB ID listed is 16C0 0486 00.
#  Click "Replace Driver."

#  Install pyusb using pip install pyusb.

## Install process for Linux: TODO. ##

import usb.core
import time

VENDOR_TEENSY = 0x16C0
PRODUCT_RAWHID = 0x0486

INPUT_STREAM_ENDPOINT_ID = 0x83
OUTPUT_STREAM_ENDPOINT_ID = 0x04

DATA_STREAM_BUFFER = [0] * 64;

print("Starting")

device = usb.core.find(idVendor = VENDOR_TEENSY, idProduct = PRODUCT_RAWHID)
#configuration = device.get_active_configuration()
#interface = configuration[(0,0)]
#endpoint = interface[0]

#device.set_configuration()


for x in range(0, 50):
    try:
        print(device.read(INPUT_STREAM_ENDPOINT_ID, 64, 2100)) # Endpoint ID = 0x83, Read 64 bytes, Timeout after 2100 ms.
    except usb.core.USBError:
        print("Issue reading packet.")

    DATA_STREAM_BUFFER[0] = x % 4;
    

    try:
        print(device.write(OUTPUT_STREAM_ENDPOINT_ID, DATA_STREAM_BUFFER, 500))
    except usb.core.USBError:
        print("Issue sending packet")



print("Ended")
