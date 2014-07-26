#ifndef TRANSFER_H630903
#define TRANSFER_H630903

#include "Arduino.h"
#include <util/crc16.h>

class Transfer {
	private:
	Stream *stream;
	uint8_t rxBuffer[128];
	uint8_t syncSequence[2];
	
	bool compareArrays(uint8_t* a, uint8_t* b, uint8_t length);
	
	public:
	Transfer();
	
	void setStream(Stream *_stream);
	
	bool synchronize();
	
	template <class T>
	void send(T *item) {
		uint8_t length = sizeof(T);
		uint16_t crc = 0xFFFF;
		stream->write(0xFF);
		stream->write(0xFA);
		for ( uint8_t i = 0 ; i < length ; i++ ) {
			stream->write(reinterpret_cast<uint8_t*>(item)[i]);
			crc = _crc_ccitt_update(crc,reinterpret_cast<uint8_t*>(item)[i]);
		}
		crc = ~crc;
		stream->write(reinterpret_cast<uint8_t*>(&crc)[0]);
		stream->write(reinterpret_cast<uint8_t*>(&crc)[1]);
	}
	
	template <class T>
	bool receive(T *item) {
		if ( !synchronize() ) {
			return false;
		}
		uint8_t length = sizeof(T);
		uint16_t crc = 0xFFFF;
		for ( uint8_t i = 0 ; i < length ; i++ ) {
			rxBuffer[i] = stream->read();
			crc = _crc_ccitt_update(crc,rxBuffer[i]);
		}
		crc = ~crc;
		uint16_t checksum;
		reinterpret_cast<uint8_t*>(&checksum)[0] = stream->read();
		reinterpret_cast<uint8_t*>(&checksum)[1] = stream->read();			
				
		if ( checksum != crc ) {
		  return false;
		}
		
		for ( uint8_t i = 0 ; i < length ; i++ ) {
		  reinterpret_cast<uint8_t*>(item)[i] = rxBuffer[i];
		}
		
		return true;
	}
	
};

#endif
