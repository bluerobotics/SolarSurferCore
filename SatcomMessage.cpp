#include "SatcomMessage.h"
#include "struct/struct.h"
#include <util/crc16.h>

namespace {
	uint8_t buffer[128];
	uint8_t length;
	Stream *stream;
}

namespace SatcomMessage {
	void init(Stream *_stream) {
		stream = _stream;
	}

	void beginTransfer(uint8_t messageId) {
		buffer[0] = messageId;
		length = 1;
	}

	void send(float value) {
		Struct::pack<float>(&value,&buffer[length]);
		length += 4;
	}

	void endTransfer() {
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
	}
}
