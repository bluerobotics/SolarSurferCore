#ifndef SATCOMMESSAGE_H
#define SATCOMMESSAGE_H

#include <WProgram.h>
#include "struct/struct.h"

namespace SatcomMessage {
	const static uint8_t status = 1;
	const static uint8_t image = 2;

	void init();

	void getTelemPacket(uint8_t packetType);

	template <class T>
	void addToPacket(T item, uint8_t buffer[], uint8_t &address) {
		for ( uint8_t i = 0 ; i < sizeof(T) ; i++ ) {
      buffer[address+i] = reinterpret_cast<uint8_t*>(&item)[i];
    }
		address =+ sizeof(T);
	}
}

#endif
