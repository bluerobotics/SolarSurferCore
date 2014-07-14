#include "BLDCMonitor.h"

BLDCMonitor::BLDCMonitor(Stream *_stream) {
	transfer.setStream(_stream);
}
	
void BLDCMonitor::read() {
	static const uint32_t tryDelayMS = 5;
	uint8_t tries = 5;
	while ( tries-- && !transfer.receive(&data) ) {
		delay(tryDelayMS);
	}
}
