#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <WProgram.h>
#include <util/crc16.h>
#include "Messages.h"

namespace Msg {
	extern MessageType::Status status;
}

namespace MessageManager {
	extern uint8_t txBuffer[];
	extern uint8_t rxBuffer[];
	extern size_t txLength;
	extern size_t rxLength;
	
	void init();
	
	void updateFields();

	template <class T>
	void serialize(T* messageStruct) {
		uint16_t crc = 0xFFFF;
		txLength = sizeof(T);
		for ( uint8_t i = 0 ; i < txLength ; i++ ) {
			txBuffer[i] = reinterpret_cast<uint8_t*>(messageStruct)[i];
			crc = _crc_ccitt_update(crc,txBuffer[i]);
		}
		crc = ~crc;
		txBuffer[txLength] = reinterpret_cast<uint8_t*>(&crc)[0];
		txBuffer[txLength+1] = reinterpret_cast<uint8_t*>(&crc)[1];
		txLength += 2;
	}
	
	uint8_t* getRXBuffer();
	
	const uint8_t* getTXBuffer();	
	
	size_t& getRXBufferLength();

  size_t getTXBufferLength();
}

#endif
