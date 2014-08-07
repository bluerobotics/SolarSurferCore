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

	void writeDefaults() {
		data.currentWaypointIndex        =        0;
		data.lastWaypointIndex           =        0;
                data.commandCount                =        0;
                data.telemetryCount              =        0;
                data.telemetryEnum               =        2;
                data.telemetryPeriod             =        10*60000;
		data.forceMode                   =        0;
		data.forceHeading                =        0.0f;
                data.goalVoltage                 =        13.2f;
		write();
        }
}
