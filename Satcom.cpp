#include "Satcom.h"
/*
namespace {
	Stream &stream;
	IridiumSBD isbd(stream, 12);
}

namespace Satcom {
	void init(Stream &_stream) {
		stream = _stream;
		isbd.attachConsole(Serial);
		isbd.attachDiags(Serial);
		isbd.setPowerProfile(1); // 1 == low power
		isbd.begin();
	}
	
	int16_t getSignalQuality() {
		int16_t signalQuality;
		uint8_t err = isbd.getSignalQuality(signalQuality);
		return signalQuality;
	}
	
	bool sendReceivePacket(uint8_t *packet, uint8_t length,uint8_t *rxBuffer,uint8_t rxBufferSize) {
		uint8_t err = isbd.sendReceiveSBDBinary(packet, length, rxBuffer, rxBufferSize);
		if ( err != 0 )
			return false;
		else
			return true;
	}
}
*/