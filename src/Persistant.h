#ifndef PERSISTANT_H
#define PERSISTANT_H

#include <WProgram.h>
#include "NVM.h"

struct PersistantData {
	uint16_t currentWaypointIndex;
	uint16_t lastWaypointIndex;
	uint16_t commandCount;
	uint16_t telemetryCount;
	uint32_t telemetryPeriod;
	uint8_t forceMode;
	float forceHeading;
	float goalVoltage;
};

namespace Persistant {
	extern PersistantData data;

	void read();
	
	void write();
}

#endif
