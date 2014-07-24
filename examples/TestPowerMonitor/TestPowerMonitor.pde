/* This file tests the data tranfer between for the SolarSurfer and the BLDC monitor 
* and the Power Monitor. It is currently written to run on an Atmega328p for testing 
* purposes. */

#include <WProgram.h>
#include <NewSoftSerial.h>
#include "../../Transfer.h"
#include "../../PowerMonitor.h"

NewSoftSerial nss1(8,9); // Connected to nss2

PowerMonitor pm(&nss1);

void setup() {
	Serial.begin(115200);
	nss1.begin(19200);
	
	Serial.println("Start.");
}

void loop() {
	  
  static long outputTimer;
  
  delay(250);

  pm.read();

  /*if ( nss1.available() > 0 ) {
  	Serial.print(nss1.read(),HEX);
	}*/

  if ( true && float(micros()-outputTimer)/1000000l > 0.1 ) {
    outputTimer = micros();

    Serial.write(27);       // ESC command
    Serial.print("[2J");    // clear screen command
    Serial.write(27);
    Serial.print("[H");     // cursor to home command
    for (uint8_t i = 0 ; i < 4 ; i++) {
      Serial.print(i,DEC);Serial.print(": ");
      Serial.print(pm.data.voltage[i],2); Serial.print(" V\t");
		  Serial.print(pm.data.current[i],2); Serial.print(" A\t\t");
		  Serial.print(pm.getPower(i),2); Serial.print(" W");
		  Serial.println("");
    }
  }
}
