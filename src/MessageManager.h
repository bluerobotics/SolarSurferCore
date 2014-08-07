#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <WProgram.h>
#include <util/crc16.h>
#include "Messages.h"
#include "BLDCMonitor.h"
#include "PowerMonitor.h"

#define MM_TX_BUFFER_SIZE 340
#define MM_RX_BUFFER_SIZE 270

namespace Msg {
	extern MessageType::tlmstatus tlmstatus;
	extern MessageType::tlmdiagnostic tlmdiagnostic;
	extern MessageType::cmdcontrol cmdcontrol;
}

namespace MessageManager {
	extern uint8_t txBuffer[];
	extern uint8_t rxBuffer[];
	extern size_t txLength;
	extern size_t rxLength;
	
	void init(BLDCMonitor *_bldc,PowerMonitor *_power);
	
	void updateFields();

	void processCommand();

	template <class T>
	void serialize(T* messageStruct) {
		uint16_t crc = 0xFFFF;
		txLength = sizeof(T);
		for ( uint8_t i = 0 ; i < txLength - 2 ; i++ ) {
			txBuffer[i] = reinterpret_cast<uint8_t*>(messageStruct)[i];
			// Josh is using a Node CRC library and it seems to be using the XModem CRC16
			// protocol. This should match his checksum.
			crc = _crc_xmodem_update(crc,txBuffer[i]);
		}
		txBuffer[txLength-2] = reinterpret_cast<uint8_t*>(&crc)[0];
		txBuffer[txLength-1] = reinterpret_cast<uint8_t*>(&crc)[1];
	}

	template <class T>
	bool deserialize(T* commandStruct) {
		uint16_t crc = 0xFFFF;
		rxLength = sizeof(T);
		// First validate the checksum
		for ( uint8_t i = 0 ; i < rxLength - 2 ; i++ ) {
		  crc = _crc_xmodem_update(crc,rxBuffer[i]);
		}
		if ( crc != reinterpret_cast<uint16_t*>(&rxBuffer[rxLength-2])[0] ) {
			return false;
		}
		// If successful, continue to copy buffer to struct
		for ( uint8_t i = 0 ; i < rxLength ; i++ ) {
			reinterpret_cast<uint8_t*>(&commandStruct)[i] = rxBuffer[i];
		}
		return true;
	}
	
	uint8_t* getRXBuffer();
	
	const uint8_t* getTXBuffer();	

  size_t getTXBufferLength();
}

#endif
