ArduinoDueParallel
====================

Library to enable the External Memory Bus/Static Memory Controller on the 
Arduino Due board. It's more of an external memory interface than a true 
parallel port. 

The DUE board pins out the data bus on the extended digital headers along with 
the control signals NCS1 and NWR.  Some of the address signals are connected to 
the PWM pins (A0-A5), but a full address bus is unavailable.  There is also 
conflict between the SS1 pin for SPI and the NRD signal used for the parallel 
bus.  In short the hardware wasn't designed for use with external parallel 
memories.  
		  
The library does allow connection to some of the lower resolution LCD 
controllers that used index addressing and can speed up read/write times 
considerably.
		  
To use this library, place the files in a folder under the libraries directory 
in your sketches folder.

See the examples folder for more usage.

PINOUT
======
D0 - PIN 34
D1 - PIN 35
D2 - PIN 36
D3 - PIN 37
D4 - PIN 38
D5 - PIN 39
D6 - PIN 40
D7 - PIN 41

A0 - PWM9
A1 - PWM8
A2 - PWM7
A3 - PWM6
A4 - PWM5
A5 - PWM4
A6 - N/C  :( - Other pins above this are mapped, but I didn't bother mapping
										NRD - PWM4
NWE - PIN45

NCS0 - AD4
NCS1 - PIN31
NCS2 - N/C
NCS3 - PWM13

