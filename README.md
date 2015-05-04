Jamrific
================

This is a little project that attempts to use the Colorific bulbs as an RGB Audio light show. 
There are two implementations included. One using Python and the other C++. Python did a pretty good job
but I thought C++ would be faster with reduced delay. In the end, it wasn't the case but C++ can be faster
if the BlueZ library is implemented directly instead of calling Gatttool. If I have some time, I can try
but maybe someone who has worked with BlueZ can attempt.

In the end I was able to tweak Python to perform pretty well but there is one bug - it stops after a while 
and you have to restart it to resume. Not sure if it is random, too much data, or invalid data during certain audio types. 

There is still a slight delay but that might be just the nature of the beast. It's a race of the speed sound versus the speed of 
processing the audio, setting the bulb colour over Bluetooth and the light reaching your eyes. 
Sound VS Electrons VS Light... ha!

Shifting the colour processing from Python to the Teensy helped a ton. I think what is slow is processing the serial
input and sending over BT to the bulb. Overall, it's not too bad. That is until you have to restart Python. 

If anything this can be a starting point for someone. At a minimum if it can be made more stable in its current 
form, I can see it being usable.


References
================
Based on the work by Tony DiCola

https://learn.adafruit.com/reverse-engineering-a-bluetooth-low-energy-light-bulb/explore-gatt

Some of the other sites I used in my research travels

To setup higher baud rate over 115200

https://www.raspberrypi.org/forums/viewtopic.php?f=44&t=73673

Found a way to use gatttool non-interactively

http://stackoverflow.com/questions/29326597/how-to-use-gatttool-non-interactive-mode

Same Issue I had in C, can't go faster than 115200

https://www.raspberrypi.org/forums/viewtopic.php?f=44&t=44810

Possible example to get started with BlueZ in C

https://github.com/carsonmcdonald/bluez-experiments/blob/master/experiments/scantest.c

Some sites I found while trying to get the HM-10 option working

http://allmydroids.blogspot.com/2014/10/hm-10-hm-11-ble-bluetooth-master.html
http://stackoverflow.com/questions/27633216/hm10-ble-change-characteristic-value-at-command-arduino

For future reference

https://github.com/IanHarvey/bluepy

Hardware
================

BT 4.0 USB module for RPi  (https://www.adafruit.com/products/1327)

Teensy 3.1

Audio Shield

TFT LCD (Optional for FFT display)

Raspberry Pi 2, but might be okay on the original.

Colorific Bulb (http://www.amazon.com/Colorific-Controlled-Dimmable-Android-Bluetooth/dp/B00KG1JYCI)


It uses a Teensy 3.1 with the Audio shield. The UART on the Teensy is connected directly to the RPi UART.
The baud rate is set at 912600, however I'm not sure if it really helps that much. You need to set-up boot/config.txt and turn off getty from
controlling the hardware UART. The connection is much more stable with hardware UART versus USB to serial. 

I tried to set-up the HM-10 to control the bulb directly which would of been much faster and more efficient. 
But I could never get it to work. I can get the HM-10 to connect but can't figure out how to write to different
characteristics than the default serial one. This would be a great solution. I emailed the creator to see if it would be 
possible. It is the CCC2540/1 after all so I am sure somehow it is possible. 



Software
=========================

There are two parts to the software: the Teensy sketch, which was based on rheslip20 work with the TFT FFT display
https://www.youtube.com/watch?v=CblOUmK3oKM

In this case the display is optional. I want to make a PCB with everything in place and portable with the RPi.

At the bottom of the sketch you can control how fast the data is sent. Originally I was throttling but now
I just send at full speed and let Python drop what it can't process. 

All of the code needs clean up from testing and commenting.


Installation
=========================

-Get BlueZ installed

-Use Tony's guide to get you set-up with BlueZ and the bulb tested

-Makes changes to get the higher baud rate on the RPi


-Set-up Teensy with Audio shield and test with sketch

-Make sure you can see the serial input from the Teensy with Minicom or Screen






To Do
================

Code clean up and comments

Support multiple bulbs

Try direct integration with BlueZ in C++

Improve colour patterns or take better advantage of the FFT data



Notes
================

While writing the readme I was looking up the link for the bulb on Amazon and I found these other bulbs. 
One apparently has disco mode, and the other music sync. Maybe worth looking into. Ah well, this was a still a fun project and I still have some hair left!

http://www.amazon.com/Mudder-Bluetooth-Functions-Smartphone-Controlled/dp/B00QHV9BF6

http://www.amazon.com/Flux-Bluetooth-LED-Light-Bulb/dp/B00GWBBZ2I




License: All source code and designs are released under 

Creative Commons ShareAlike 

CC BY-SA

![CC BY-NC-SA](http://i.creativecommons.org/l/by-sa/3.0/88x31.png)