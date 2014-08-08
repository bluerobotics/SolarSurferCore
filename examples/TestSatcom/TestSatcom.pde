/* This file tests the communication system for the SolarSurfer. It is currently written
* to run on an Atmega328p for testing purposes. */

#include <WProgram.h>
#include <NewSoftSerial.h>
#include "../../src/MessageManager.h"
#include "../../src/Messages.h"
#include "../../src/IridiumSBD.h"
#include "../../src/BLDCMonitor.h"
#include "../../src/PowerMonitor.h"

NewSoftSerial nssBLDC(100,101);
NewSoftSerial nssPM(102,103);
NewSoftSerial nss(10, 11); //RX, TX
IridiumSBD isbd(nss, 12);
static const int ledPin = 13;

BLDCMonitor bldc(&nssBLDC);
PowerMonitor pm(&nssPM);

bool ISBDCallback()
{
  static long timer;
  if ( millis() - timer > 1000 ) {
    timer = millis();
    Serial.println("Callback");
  }
  digitalWrite(ledPin, (millis() / 1000) % 2 == 1 ? HIGH : LOW);
  return true;
}

void setup() {
	MessageManager::init(&bldc,&pm);

	int signalQuality = -1;

  pinMode(ledPin, OUTPUT);

  Serial.begin(56700);
  nss.begin(19200);

  Serial.println("Press 's'");

  while (true) {
    uint8_t input = Serial.read();
    if (input == 's') {
      break;
    }
  }
  
  Serial.println("Setting up isbd");

  isbd.attachConsole(Serial);
  //isbd.attachDiags(Serial);
  isbd.useMSSTMWorkaround(false);
  isbd.setPowerProfile(1); // 1 == low power
  isbd.setMinimumSignalQuality(2); // 2+ is recommended for sending
  //isbd.begin();
  
  Serial.println("Setup done");

  int err;
  //err = isbd.getSignalQuality(signalQuality);
  if (err != 0)
  {
    Serial.print("SignalQuality failed: error ");
    Serial.println(err);
    return;
  }

  Serial.print("Signal quality is ");
  Serial.println(signalQuality);
}

void loop() {

	MessageManager::updateFields();
	MessageManager::serialize(&Msg::cmdcontrol);
	uint16_t length = MessageManager::getTXBufferLength();
	const uint8_t *data = MessageManager::getTXBuffer();

	Serial.print("Packet Length: ");Serial.println(length);
	Serial.println("Packet Data:");	
	for ( uint8_t i = 0 ; i < length ; i++ ) {
    if ( data[i] < 0x10 ) {
      Serial.print("0");
    }
		Serial.print(data[i],HEX);Serial.print("\t");
		if ( (i+1) % 8 == 0 ) {
			Serial.println("");
		}
	}
  Serial.println("");

  Serial.println("Packet Data As It Will Be Received:"); 
  for ( uint8_t i = 0 ; i < length ; i++ ) {
    if ( data[i] < 0x10 ) {
      Serial.print("0");
    }
    Serial.print(data[i],HEX);
  }
  Serial.println("");
	
	int16_t err;
	//err = isbd.sendReceiveSBDBinary(MessageManager::getTXBuffer(),MessageManager::getTXBufferLength(),MessageManager::getRXBuffer(),MessageManager::getRXBufferLength());
  if (err != 0)
  {
    Serial.print("sendSBDText failed: error ");
    Serial.println(err);
    return;
  }
  
  Serial.println("Hey, it worked!");
  Serial.print("Messages left: ");
  Serial.println(isbd.getWaitingMessageCount());
	
  while ( true ) {
  	digitalWrite(ledPin, HIGH);
  }
}
