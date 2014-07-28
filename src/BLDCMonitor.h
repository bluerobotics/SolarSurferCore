#ifndef BLDCMONITOR_H
#define BLDCMONITOR_H

#include <WProgram.h>
#include "Transfer.h"

struct BLDCMonitorStruct {
	int16_t rpmLeft;
	int16_t rpmRight;
	float currentLeft;
	float currentRight;
	float voltage;
};

class BLDCMonitor {
	private:
	Transfer transfer;

	public:
	BLDCMonitorStruct data;
	
	BLDCMonitor(Stream *_stream);
	
	void read();

	float getTotalPower();
};

#endif