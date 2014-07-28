/* This file tests the data tranfer between for the SolarSurfer and the BLDC monitor 
* and the Power Monitor. It is currently written to run on an Atmega328p for testing 
* purposes. */

#include <WProgram.h>
#include <NewSoftSerial.h>
#include "../../src/Transfer.h"

NewSoftSerial nss1(2,3); // Connected to nss2

Transfer ts();
Transfer t1();

struct Data {
	uint8_t a;
	uint16_t b;
	int16_t c;
	uint32_t d;
	int32_t e;
	float f;
};

Data example;
Data ds;
Data d1;

void printData(Data &data) {
	Serial.print(data.a,DEC);Serial.print('\t');
	Serial.print(data.b,DEC);Serial.print('\t');
	Serial.print(data.c,DEC);Serial.print('\t');
	Serial.print(data.d,DEC);Serial.print('\t');
	Serial.print(data.e,DEC);Serial.print('\t');
	Serial.print(data.f,1);Serial.print('\t');
}

void setup() {
	Serial.begin(19200);
	nss1.begin(19200);
	
	ts.setStream(&Serial);
	t1.setStream(&nss1);
	
	example.a = 1;
	example.b = 2;
	example.c = 3;
	example.d = 4;
	example.e = 5;
	example.f = 6.1;
	
	Serial.println("Start.");
}

void loop() {
	Serial.println("Sending example data from NSS1 to Serial");
	t1.send(&example);
	ts.receive(&ds);
	Serial.println("Serial's Data:");
	printData(ds);
	
	for (;;);
}
