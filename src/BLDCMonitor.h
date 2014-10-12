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
  float thrust;
  int16_t pwmA;
  int16_t pwmB;
};

class BLDCMonitor {
	private:
	Transfer transfer;

	public:
	BLDCMonitorStruct data;
	
	BLDCMonitor(Stream *_stream);
	
	void read();

	float getPower(uint8_t index);

	int16_t getRPM(uint8_t index);

	float getTotalPower();
};

#endif