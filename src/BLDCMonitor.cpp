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

float BLDCMonitor::getPower(uint8_t index) {
	switch(index) {
		case 0:
			return data.voltage*data.currentLeft;
		case 1:
			return data.voltage*data.currentRight;
	}
	return 0;
}

int16_t BLDCMonitor::getRPM(uint8_t index) {
	switch(index) {
		case 0:
			return data.rpmLeft;
		case 1:
			return data.rpmRight;
	}
	return 0;
}

float BLDCMonitor::getTotalPower() {
	return (data.currentLeft+data.currentRight)*data.voltage;
}
