mhvdoor
=======

Because every hackerspace needs an Arduinolike board on their door.

Ours is an MHVboard which is based off the Metaboard (because every
hackerspace needs to make their own Arduino clone). It is currently plugged
in to the LTSP thin client sitting on the shelf where it is getting its
power.

Currently the mhvdoorduino does not very much. It has 4 Sure Electronics
32x8 LED matricies vertically stacked end to end. All it is doing ATM is
bouncing a line up and down the displays.

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

Hacking
=======
If you want to hack on it read the do-ocracy sign!

All the code is currently contained in a single c file and it can be
compiled with 'make' and programmed with 'make upload'.
