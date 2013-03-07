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
Address Bus:

A0	PC21	PWM9
A1	PC22	PWM8
A2	PC23	PWM7
A3	PC24	PWM6
A4	PC25	PWM5
A5	PC26	PWM4
A6	PC27	N/C
A7	PC28	PWM3
A8	PC29	PWM10
A9	PC30	RX LED
A10	PD0	PIN 25
A11	PD1	PIN 26
A12	PD2	PIN 27
A13	PD3	PIN 28
A14	PD4	TXD0
A15	PD5	RXD0
A16	PD6	PIN 29
A17	PD7	PWM11
A18	PA25	MISO
A19	PA26	MOSI
A20	PA27	SPCK
A21	PD8	PWM12
A22	PD9	PIN 30

Here is the data bus:

D0	PC2	PIN 34
D1	PC3	PIN 35
D2	PC4	PIN 36
D3	PC5	PIN 37
D4	PC6	PIN 38
D5	PC7	PIN 39
D6	PC8	PIN 40
D7	PC9	PIN 41
D8	PC10	N/C
D9	PC11	N/C
D10	PC12	PIN 51
D11	PC13	PIN 50
D12	PC14	PIN 49
D13	PC15	PIN 48
D14	PC16	PIN 47
D15	PC17	PIN 46

And the control signals:

NRD	PA29	SS1/PWM4 (also tied to PC26 on PCB, which is A5)
NWE	PC18	PIN 45
NCS0	PA6	AD4
NCS1	PA7	PIN 31
NCS2	PB24	N/C
NCS3	PB27	PWM13
