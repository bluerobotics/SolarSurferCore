#ifndef PERSISTANT_H
#define PERSISTANT_H

#include <WProgram.h>
#include "NVM.h"

struct PersistantData {
	uint16_t currentWaypointIndex;
	uint16_t lastWaypointIndex;
};

namespace Persistant {
	extern PersistantData data;

	void read();
	
	void write();
}

#endif
