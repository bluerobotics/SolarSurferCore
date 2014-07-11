#include "SatcomMessage.h"
#include "struct/struct.h"
#include <util/crc16.h>

#include "GPS_UBX.h"

namespace {
	uint8_t buffer[128];
	uint8_t length;
}

namespace SatcomMessage {
	void init() {

	}

	void getTelemPacket(uint8_t packetType) {
		uint8_t address = 0;
		//addToPacket<uint8_t>(Captain::mode,buffer,address);
		addToPacket<float>(GPS_UBX::latitude,buffer,address);
		addToPacket<float>(GPS_UBX::longitude,buffer,address);
		addToPacket<uint8_t>(GPS_UBX::fix,buffer,address);
		addToPacket<long>(GPS_UBX::time,buffer,address);
		addToPacket<int16_t>(GPS_UBX::altitude/100,buffer,address);
		addToPacket<int16_t>(GPS_UBX::course,buffer,address);
		length = address;
	}

	/*void endTransfer() {
		uint16_t crc = 0xFFFF;
		
		stream->write(0xFF);
		stream->write(0xFA);
		stream->write(length);
		for ( uint8_t i = 0 ; i < length ; i++ ) {
			stream->write(buffer[i]);
			crc = _crc_ccitt_update(crc,buffer[i]);
		}
		crc = ~crc;
		
		uint8_t crcbuff[2];
		Struct::pack<uint16_t>(&crc,&crcbuff[0]);
		stream->write(crcbuff[0]);
		stream->write(crcbuff[1]);
	}*/
}
