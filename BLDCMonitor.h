#ifndef BLDCMONITOR_H
#define BLDCMONITOR_H

#include <WProgram.h>
#include "Transfer.h"

struct BLDCMonitorStruct {
	int16_t rpmA;
	int16_t rpmB;
	float currentA;
	float currentB;
	float voltage;
};

class BLDCMonitor {
	private:
	Transfer transfer;

	public:
	BLDCMonitorStruct data;
	
	BLDCMonitor(Stream *_stream);
	
	void read();
};

#endif