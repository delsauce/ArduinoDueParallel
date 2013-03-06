Arduino-Due-Parallel
====================

Library to enable the External Memory Bus/Static Memory Controller on the 
Arduino Due board. It's more of an external memory interface than a true 
parallel port. 

This library implements a limited functionality parallel port on the Arduino 
DUE board (ARM Cortex-M3 based).  
    
The DUE board pins out the data bus on the extended digital headers along with 
the control signals NC1 and NWR.  Some of the address signals are connected to 
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
