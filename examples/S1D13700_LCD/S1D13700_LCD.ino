/*
  This example uses the Parallel library to control the CFAG320240DX family
  of LCDs from CrystalFontz.  This LCD is a monochrome 320 x 240 pixel unit 
  with an EPSON S1D13700 controller.  The sketch will initialize the 
  controller then draw some vertical lines on the display.

  This sketch is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Parallel.h>

#define LCDWIDTH 	320
#define LCDHEIGHT	240
#define BPP		1

int lcdOn = 46;
int lcdReset = 44;
uint8_t count = 0;


void setup() {
  pinMode(lcdOn, OUTPUT);
  digitalWrite(lcdOn, 1);

  // Configure parallel bus for NCS1, A0, and NWE (no read signal)
  Parallel.begin(PARALLEL_BUS_WIDTH_8, PARALLEL_CS_1, 1, 0, 1);

  // Configure conservative bus timings.  This could be pushed faster...
  Parallel.setAddressSetupTiming(5,1,5,1);
  Parallel.setPulseTiming(50,60,50,60);
  Parallel.setCycleTiming(110,110);

  // Toggle the reset line
  pinMode(lcdReset, OUTPUT);
  digitalWrite(lcdReset, 1);
  delay(3);
  digitalWrite(lcdReset, 0);
  delay(3);
  digitalWrite(lcdReset, 1);
  delay(10);

  configureLCD();
}

void loop() {
}

void configureLCD() {
  Parallel.write(0x01, 0x40);
  Parallel.write(0x00, 0x30);	// no origin comp, single panel, 8pix char 
                                // height, internal CGROM
  Parallel.write(0x00, 0x87);	// 8 pix char width
  Parallel.write(0x00, 0x07);	// 8 pix char height (why twice?)
  Parallel.write(0x00, 0x27);	// 40 chars wide
  Parallel.write(0x00, 0x39);	
  Parallel.write(0x00, 0xEF);	// 240 lines high
  Parallel.write(0x00, 0x28);	// horizontal address range 40 (?)
  Parallel.write(0x00, 0x00);

  Parallel.write(0x01, 0x44);	// Screen block start addresses
  Parallel.write(0x00, 0x00);	// Block 1 @ 0x0
  Parallel.write(0x00, 0x00);
  Parallel.write(0x00, 0xEF);	// 240 lines
  Parallel.write(0x00, 0x60);	// Block 2 start address
  Parallel.write(0x00, 0x09);	// 0x0960
  Parallel.write(0x00, 0xEF);	// 240 lines
  Parallel.write(0x00, 0x00);	// Block 3 start address (not used)
  Parallel.write(0x00, 0x00);
  Parallel.write(0x00, 0x00);	// Block 4 start address
  Parallel.write(0x00, 0x00);

  Parallel.write(0x01, 0x5A);	// Horizontal Scroll
  Parallel.write(0x00, 0x00);	  // 0 bits
	
  Parallel.write(0x01, 0x5B);	// OVRLY
  Parallel.write(0x00, 0x01);	// three layers, layer 3 text, layer 1 text, XOR
	
  Parallel.write(0x01, 0x5D);	// cursor mode
  Parallel.write(0x00, 0x07);	// 8 bits wide
  Parallel.write(0x00, 0x87);	// Block style, 8 bits high
  
  // Cursor direction	
  Parallel.write(0x01, 0x4C);

  // clear text layer
  Parallel.write(0x01, 0x46);
  Parallel.write(0x00, 0x00);
  Parallel.write(0x00, 0x00);
  
  Parallel.write(0x01, 0x42);
  for (int i=0; i < 40*30; i++)
  {
	  Parallel.write(0x00, ' ');
  }
	
  // draw vertical lines 
  Parallel.write(0x01, 0x46);
  Parallel.write(0x00, 0x60);
  Parallel.write(0x00, 0x09);
	
  Parallel.write(0x01, 0x42);
  for (int i=0; i < ((LCDWIDTH*LCDHEIGHT)/(8/BPP)); i++)	
  {
	Parallel.write(0x00, 0x55);
  }
  
  // Turn display on	
  Parallel.write(0x01, 0x59);		// DISP_ON
  Parallel.write(0x00, 0x16);
}

