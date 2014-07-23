#include "PowerMonitor.h"

PowerMonitor::PowerMonitor(Stream *_stream) {
	transfer.setStream(_stream);
}
	
void PowerMonitor::read() {
	static const uint32_t tryDelayMS = 5;
	uint8_t tries = 5;
	while ( tries-- && !transfer.receive(&data) ) {
		delay(tryDelayMS);
	}	
}

float PowerMonitor::getPower(uint8_t index) {
	return data.voltage[index]*data.current[index];
}