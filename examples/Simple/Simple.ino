/*
  This example implements the most basic usage of the Parallel
  library.  Eight data lines and no control signals are configured.
  The operate with the chip's default timings (data lines do not
  hold state indefinately).

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
int led = 13;
uint8_t count = 0;

void setup() {
  pinMode(led, OUTPUT);

  // Configure parallel bus with no control signals and default timings
  Parallel.begin(PARALLEL_BUS_WIDTH_8, PARALLEL_CS_NONE, 0, 0, 0);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                // wait for a second

  // Write some data out to the parallel port.
  Parallel.write(0, count++);
}

