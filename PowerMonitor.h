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
	PowerMonitorStruct data;
	
	PowerMonitor(Stream *_stream);
	
	void read();
};

#endif