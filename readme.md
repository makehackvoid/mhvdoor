mhvdoor
=======

Because every hackerspace needs an Arduinolike board on their door.

Ours is an MHVboard which is based off the Metaboard (because every
hackerspace needs to make their own Arduino clone). It is currently plugged
in to the LTSP thin client sitting on the shelf where it is getting its
power, but does not communicate to it yet.

Currently the mhvdoorduino implements a basic occupancy gauage. It has 4
Sure Electronics 32x8 LED matricies which get updated once a second. If, at
the beginning of that second, the PIR senses movement one more LED will be
lit, if the PIR is not sensing movement, one less LED will be lit.

Pins
====
Sure displays
-------------
wclk B0
rclk B1
data B2
CS1  D4
CS2  D5
CS3  D6
CS4  D7 (yes this is a derp, when you are using the upload jumper the 4th
display gets garbage.)

PIR sensor
----------
The PIR sensor is hooked in to C0 with the internal pullup turned on. The
logic level is inverted, but the short SENSOR macro handles that.

Hacking
=======
If you want to hack on it read the do-ocracy sign!

All the code is currently contained in a single c file and it can be
compiled with 'make' and programmed with 'make program'.
