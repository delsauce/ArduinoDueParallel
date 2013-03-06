/*
  Parallel.cpp

  This library implements a limited functionality parallel port on the Arduino
  DUE board (ARM Cortex-M3 based). It's more of an external memory interface
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

#include "Parallel.h"
//#include "smc.h"

// Data bus, 8-bits.  16-bit bus not supported because D8/D9 pins (PC10/PC11) are not connected on the DUE.
const PinDescription DataPins[]=
{
	{ PIOC, PIO_PC2A_D0,          ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },	// PIN 34
	{ PIOC, PIO_PC3A_D1,          ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },	// PIN 35
	{ PIOC, PIO_PC4A_D2,          ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },	// PIN 36
	{ PIOC, PIO_PC5A_D3,          ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },	// PIN 37
	{ PIOC, PIO_PC6A_D4,          ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },	// PIN 38
	{ PIOC, PIO_PC7A_D5,          ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },	// PIN 39
	{ PIOC, PIO_PC8A_D6,          ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },	// PIN 40
	{ PIOC, PIO_PC9A_D7,          ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }	// PIN 41
};	

// Not much you can do with the address pins, most aren't available on the stock DUE board.
// A0-A5 are available on PWM pins (PWM9-PWM4), but PC26 (PWM4) is wired to PA29 (why?)
// which is the NRD signal.  So if the read signal is needed (or SS1 for SPI), you can only use A0-A4.
// Assuming that you didn't need either of those, it wouldn't help too much because A6 (PC27) isn't
// connected to anything.  Long story short, the address bus might not be all that useful, except for
// for devices that use indirect addressing, such as many of the lower resolution LCD controllers.
const PinDescription AddressPins[]=
{
	{ PIOC, PIO_PC21A_A0,		  ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PWM9
	{ PIOC, PIO_PC22A_A1,		  ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PWM8
	{ PIOC, PIO_PC23A_A2,		  ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PWM7
	{ PIOC, PIO_PC24A_A3,		  ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PWM6
	{ PIOC, PIO_PC25A_A4,		  ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PWM5
	{ PIOC, PIO_PC26A_A5,		  ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }  // PWM4
};

// NOTE: the read signal NRD conflicts with the SPI chip select 1 -- make sure that pin isn't used if you need NRD
const PinDescription ReadPin =
	{ PIOA, PIO_PA29B_NRD,		  ID_PIOA, PIO_PERIPH_B, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER };  // PWM4

const PinDescription WritePin =
	{ PIOC, PIO_PC18A_NWE,        ID_PIOC, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }; // PIN 45

// Note that PB24 (NCS2) is not connected on the DUE board.
const PinDescription ChipSelectPins[]=
{	
	{ PIOA, PIO_PA6B_NCS0,        ID_PIOA, PIO_PERIPH_B, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // AD4
	{ PIOA, PIO_PA7B_NCS1,        ID_PIOA, PIO_PERIPH_B, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 31
	{ PIOB, PIO_PB24B_NCS2,       ID_PIOB, PIO_PERIPH_B, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // N/C	
	{ PIOB, PIO_PB27A_NCS3,       ID_PIOB, PIO_PERIPH_A, PIO_PULLUP, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PWM13 -- shared w/LED
};

// These are fixed addresses.  See datasheet.
const uint32_t chipSelectAddresses[] =
{
	0x60000000,
	0x61000000,
	0x62000000,
	0x63000000
};

void ParallelClass::begin(ParallelChipSelect_t cs, uint8_t numAddressLines, uint8_t readEnable, uint8_t writeEnable)
{	
	// Save the chip select
	_cs = cs;
	
	// Configure GPIOs
	// Data Bus
	for (int i=0; i < (sizeof(DataPins)/sizeof(DataPins[0])); i++)
	{
		PIO_Configure(DataPins[i].pPort,
			DataPins[i].ulPinType,
			DataPins[i].ulPin,
			DataPins[i].ulPinConfiguration);
	}
	
	// address bus
	if (numAddressLines > (sizeof(AddressPins)/sizeof(AddressPins[0])))
	{
		numAddressLines = (sizeof(AddressPins)/sizeof(AddressPins[0]));
	}
	
	for (int i=0; i < numAddressLines; i++)
	{
		PIO_Configure(AddressPins[i].pPort,
			AddressPins[i].ulPinType,
			AddressPins[i].ulPin,
			AddressPins[i].ulPinConfiguration);
	}		
	
	if (readEnable > 0)
	{
		PIO_Configure(ReadPin.pPort,
			ReadPin.ulPinType,
			ReadPin.ulPin,
			ReadPin.ulPinConfiguration);
	}
	
	if (writeEnable > 0)
	{
		PIO_Configure(WritePin.pPort,
			WritePin.ulPinType,
			WritePin.ulPin,
			WritePin.ulPinConfiguration);
	}
	
	// chip select
	if (_cs < PARALLEL_CS_NONE)
	{
		// save the chip select address to reduce overhead of read/write calls
		_addr = chipSelectAddresses[_cs];
		
		PIO_Configure(ChipSelectPins[_cs].pPort,
			ChipSelectPins[_cs].ulPinType,
			ChipSelectPins[_cs].ulPin,
			ChipSelectPins[_cs].ulPinConfiguration);
	}
	else 
	{
		// user doesn't want chip select, but we still need to memory map the bus
		// so any address will do.
		_addr = chipSelectAddresses[0];
	}
	
	// Enable module
	pmc_enable_periph_clk(ID_SMC);
	
	// set mode
	smc_set_mode(SMC, _cs, SMC_MODE_READ_MODE
		| SMC_MODE_WRITE_MODE
		| SMC_MODE_DBW_BIT_8);
}

// Configure the address setup time.  See datasheet for calculations
void ParallelClass::setAddressSetupTiming(	uint8_t cyclesBeforeNWE, 
											uint8_t cyclesBeforeNCSWrite,
											uint8_t cyclesBeforeNRD,
											uint8_t cyclesBeforeNCSRead)
{
	smc_set_setup_timing(SMC,_cs,SMC_SETUP_NWE_SETUP(cyclesBeforeNWE)
		| SMC_SETUP_NCS_WR_SETUP(cyclesBeforeNCSWrite)
		| SMC_SETUP_NRD_SETUP(cyclesBeforeNRD)
		| SMC_SETUP_NCS_RD_SETUP(cyclesBeforeNCSRead));
}

// Configure the pulse width of NCS, NWE, and NRD
void ParallelClass::setPulseTiming(	uint8_t cyclesNWEWidth,
									uint8_t cyclesNCSWidthWrite,
									uint8_t cyclesNRDWidth,
									uint8_t cyclesNCSWidthRead)
{
	smc_set_pulse_timing(SMC, _cs, SMC_PULSE_NWE_PULSE(cyclesNWEWidth)
		| SMC_PULSE_NCS_WR_PULSE(cyclesNCSWidthWrite)
		| SMC_PULSE_NRD_PULSE(cyclesNRDWidth)
		| SMC_PULSE_NCS_RD_PULSE(cyclesNCSWidthRead));
}

// Set the total read/write cycle time
void ParallelClass::setCycleTiming(uint8_t cyclesWriteTotal,
									uint8_t cyclesReadTotal)
{
	smc_set_cycle_timing(SMC, _cs, SMC_CYCLE_NWE_CYCLE(cyclesWriteTotal)
		| SMC_CYCLE_NRD_CYCLE(cyclesReadTotal));
}

// Set how the which signals latch data in the read and write modes (NCS or NRD/NWE).
void ParallelClass::setMode(ReadModeFlags_t readMode, WriteModeFlags_t writeMode)
{
	smc_set_mode(SMC, _cs, readMode
		| writeMode
		| SMC_MODE_DBW_BIT_8);	// we only support 8-bit bus
}

__attribute__((optimize("O0"))) void ParallelClass::write(uint32_t offset, uint8_t data)
{
	*((volatile uint8_t *)(_addr + (offset&0x00FFFFFF))) = data;
}

__attribute__((optimize("O0"))) uint8_t ParallelClass::read(uint32_t offset)
{
	return *((volatile uint8_t *)(_addr + (offset&0x00FFFFFF)));
}

// Create our object
ParallelClass Parallel = ParallelClass();
