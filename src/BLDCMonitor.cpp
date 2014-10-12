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
			return data.voltage*data.currentA;
		case 1:
			return data.voltage*data.currentB;
	}
	return 0;
}

int16_t BLDCMonitor::getRPM(uint8_t index) {
	switch(index) {
		case 0:
			return data.rpmA;
		case 1:
			return data.rpmB;
	}
	return 0;
}

float BLDCMonitor::getTotalPower() {
	return (data.currentA+data.currentB)*data.voltage;
}
