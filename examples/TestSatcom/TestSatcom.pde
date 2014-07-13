/* This file tests the communication system for the SolarSurfer. It is currently written
* to run on an Atmega328p for testing purposes. */

#include <WProgram.h>
#include <NewSoftSerial.h>
#include "../../SatcomMessage.h"
#include "../../IridiumSBD.h"

NewSoftSerial nss(10, 11); //RX, TX
IridiumSBD isbd(nss, 12);
static const int ledPin = 13;

bool ISBDCallback()
{
   digitalWrite(ledPin, (millis() / 1000) % 2 == 1 ? HIGH : LOW);
   return true;
}

void setup() {
	SatcomMessage::init();

	int signalQuality = -1;

  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  nss.begin(19200);
  
  Serial.println("Setting up isbd");

  isbd.attachConsole(Serial);
  isbd.attachDiags(Serial);
  isbd.setPowerProfile(1); // 1 == low power
  //isbd.begin();
  
  Serial.println("Setup done");

  int err;
  err = isbd.getSignalQuality(signalQuality);
  if (err != 0)
  {
    Serial.print("SignalQuality failed: error ");
    Serial.println(err);
    return;
  }

  Serial.print("Signal quality is ");
  Serial.println(signalQuality);

  /*Serial.println("Hey, it worked!");
  Serial.print("Messages left: ");
  Serial.println(isbd.getWaitingMessageCount());*/
}

void loop() {

	SatcomMessage::formTelemPacket(SatcomMessage::testStatus);
	uint16_t length = SatcomMessage::getTXBufferLength();
	const uint8_t *data = SatcomMessage::getTXBuffer();
	Serial.print("Packet Length: ");Serial.println(length);
	Serial.println("Packet Data:");	
	for ( uint8_t i = 0 ; i < length ; i++ ) {
		Serial.print(data[i],HEX);Serial.print(" ");
		if ( i != 0 && i % 8 == 0 ) {
			Serial.println("");
		}
	}
	
	int16_t err;
	//err = isbd.sendReceiveSBDBinary(SatcomMessage::getTXBuffer(),SatcomMessage::getTXBufferLength(),SatcomMessage::getRXBuffer(),SatcomMessage::getRXBufferLength());
  if (err != 0)
  {
    Serial.print("sendSBDText failed: error ");
    Serial.println(err);
    return;
  }
	
  while ( true ) {
  	digitalWrite(ledPin, HIGH);
  }
}
