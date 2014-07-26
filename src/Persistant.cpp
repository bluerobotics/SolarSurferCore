#include "Persistant.h"

namespace Persistant {
	PersistantData data;
	
	void read() {
		uint16_t address = 0;
		data = NVM::read<PersistantData>(&address);
	}
	
	void write() {
		uint16_t address = 0;
		NVM::write<PersistantData>(data,&address);
	}
}