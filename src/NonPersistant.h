#ifndef NONPERSISTANT_H
#define NONPERSISTANT_H

#include <WProgram.h>

struct NonPersistantData {
	uint8_t inCallback;
	uint8_t lastSignalStrength;
	uint8_t lastISBDError;
};

namespace NonPersistant {
	extern NonPersistantData data;
}

#endif
