# Jamific
# Uses sounds data from serial port to control the Colorific BTLE bulb
#
# Author William Garrido
# FriedCircuits
# Needs serial port and bluez
# Ref: https://github.com/adafruit/BLE_Colorific/blob/master/colorific.py

import colorsys
import math
import sys
import time
import serial
import pexpect

#port = '/dev/ttyACM0'
port = '/dev/ttyAMA0'
#ser = serial.Serial(port,  115200)
ser = serial.Serial(port, timeout=.1, baudrate=921600)

HUE_RANGE = (0.0, 1.0) # Tuple with the minimum and maximum hue values for a
		       # cycle. Stick with 0 to 1 to cover all hues.
SATURATION = 1.0 # Color saturation for hues (1 is full color).
VALUE = 1.0 # Color value for hues (1 is full value).

# Get bulb address from command parameters.
if len(sys.argv) != 2:
	print 'Error must specify bulb address as parameter!'
	print 'Usage: sudo python colorific.py <bulb address>'
	print 'Example: sudo python colorific.py 5C:31:3E:F2:16:13'
	sys.exit(1)
bulb = sys.argv[1]

# Run gatttool interactively.
gatt = pexpect.spawn('gatttool -I')

# Connect to the device.
gatt.sendline('connect {0}'.format(bulb))
gatt.expect('Connection successful')

# Setup range of hue value and start at minimum hue.
hue_min, hue_max = HUE_RANGE
hue = hue_max

# Enter main loop.
print 'Press Ctrl-C to quit.'

hue = hue_min+math.modf(hue)[0]

while True:
	try:
		#data = float(ser.readline())
		#data = ser.readline()
		ser.flushInput()
		data = ser.read(1);
		#print data
		if data == '.':
			#print "test"
			r = ser.read().encode("hex")
			g = ser.read().encode("hex")
			b = ser.read().encode("hex")
			#print b
			gatt.sendline('char-write-cmd 0x0028 58010301ff00{0:02X}{1:02X}{2:02X}'.format(int(r),int(g),int(b)))	
	except Exception:
		pass
	#print data
	#r, g, b = map(lambda x: int(x*255.0), colorsys.hsv_to_rgb(hue, SATURATION, data))
	#r1,g1,b1 = data.split(",")
	#r = int(r1)
	#g = int(g1)
	#b = int(b1)
	#gatt.sendline('char-write-cmd 0x0028 58010301ff00{0:02X}{1:02X}{2:02X}'.format(int(r1), int(g1), int(b1)))



