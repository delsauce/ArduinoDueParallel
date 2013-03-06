/*
  Parallel.h

  This library implements a limited functionality parallel port on the Arduino
  DUE board (ARM Cortex-M3 based).  It's more of an external memory interface
  than a true parallel port.
  
  The DUE board pins out the data bus on the extended digital headers along 
  with the control signals NC1 and NWR.  Some of the address signals are 
  connected to the PWM pins (A0-A5), but a full address bus is unavailable.  
  There is also conflict between the SS1 pin for SPI and the NRD signal used 
  for the parallel bus.  In short the hardware wasn't designed for use with
  external parallel memories.  
  
  The library does allow connection to some of the lower resolution LCD
  controllers that used index addressing and can speed up read/write times 
  considerably.
  
  To use this library, place the files in a folder under the libraries 
  directory in your sketches folder.

  This library is free software; you can redistribute it and/or
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

#ifndef PARALLEL_H
#define PARALLEL_H

#include "Arduino.h"
#include "smc.h"

typedef enum 
{
	// Note that chip select 2 not connected on DUE board.
	PARALLEL_CS_0 = 0,	// AD4
	PARALLEL_CS_1 = 1,	// PIN 31
	PARALLEL_CS_3 = 3,	// PWM13
	PARALLEL_CS_NONE
} ParallelChipSelect_t;

typedef enum 
{
	READ_MODE_NCS_CTRL = SMC_MODE_READ_MODE_NCS_CTRL,
	READ_MODE_NRD_CTRL = SMC_MODE_READ_MODE_NRD_CTRL,		// Default
} ReadModeFlags_t;

typedef enum 
{
	WRITE_MODE_NCS_CTRL = SMC_MODE_WRITE_MODE_NCS_CTRL,
	WRITE_MODE_NWE_CTRL = SMC_MODE_WRITE_MODE_NWE_CTRL		// Default
} WriteModeFlags_t;	

// See SAM3X data sheet in the Static Memory Controller section.  Each chip 
// select above corresponds to these physical addresses 
extern const uint32_t chipSelectAddresses[];

class ParallelClass {
public:
  ParallelClass() { };
  void begin(ParallelChipSelect_t cs, uint8_t numAddressLines, uint8_t readEnable, uint8_t writeEnable);
  
  // Configure the address setup time.  See datasheet for calculations.
  void setAddressSetupTiming(	uint8_t cyclesBeforeNWE, 
								uint8_t cyclesBeforeNCSWrite,
								uint8_t cyclesBeforeNRD,
								uint8_t cyclesBeforeNCSRead);
  
  // Configure the pulse width of NCS, NWE, and NRD. See datasheet for calculations.
  void setPulseTiming(	uint8_t cyclesNWEWidth,
						uint8_t cyclesNCSWidthWrite,
						uint8_t cyclesNRDWidth,
						uint8_t cyclesNCSWidthRead);
  
  // Set the total read/write cycle time. See datasheet for calculations.
  void setCycleTiming(	uint8_t cyclesWriteTotal,
						uint8_t cyclesReadTotal);
									
  // Set how the which signals latch data in the read and write modes (NCS or NRD/NWE).
  void setMode(ReadModeFlags_t readMode, WriteModeFlags_t writeMode);
  
  void write(uint32_t offset, uint8_t data) ;
  uint8_t read(uint32_t offset);  

private:
  ParallelChipSelect_t _cs;
  uint32_t _addr;
};

extern ParallelClass Parallel;

#endif
