#ifndef POWERMONITOR_H
#define POWERMONITOR_H

#include <WProgram.h>
#include "Transfer.h"

struct PowerMonitorStruct {
	float voltage[4];
	float current[4];
};

class PowerMonitor {
	private:
	Transfer transfer;

	public:
	// Indices of the power monitor
	static const uint8_t CCtoSolar   = 0;
	static const uint8_t CCtoBattery = 1;
	static const uint8_t CCtoLoad    = 2;

	PowerMonitorStruct data;
	
	PowerMonitor(Stream *_stream);
	
	void read();
};

#endif