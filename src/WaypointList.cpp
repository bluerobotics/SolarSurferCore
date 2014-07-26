#include "WaypointList.h"
#include "NVM.h"

namespace {
	uint16_t getEEPROMAddressFromWaypointIndex(uint16_t index) {
		static const uint16_t eepromStartAddress = 200;
		static const uint16_t waypointSize = sizeof(Waypoint);
		
		return index*waypointSize+eepromStartAddress;
	}
}

namespace WaypointList {
	void write(uint16_t index, const Location &location) {
		uint16_t address = getEEPROMAddressFromWaypointIndex(index);
		Waypoint temp;
		temp.index = index;
		temp.location.latitude = location.latitude;
		temp.location.longitude = location.longitude;
		NVM::write<Waypoint>(temp,&address);
	}
	
	void read(Waypoint *waypoint,uint16_t index) {
		uint16_t address = getEEPROMAddressFromWaypointIndex(index);
		Waypoint temp = NVM::read<Waypoint>(&address);
		waypoint->index = temp.index;
		waypoint->location.latitude = temp.location.latitude;
		waypoint->location.longitude = temp.location.longitude;
	}
}

