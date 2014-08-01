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
	void write(uint16_t index, uint8_t radius, const Location &location) {
		uint16_t address = getEEPROMAddressFromWaypointIndex(index);
		Waypoint temp;
		temp.index = index;
		temp.radius = radius;
		temp.location.latitude = location.latitude;
		temp.location.longitude = location.longitude;
		NVM::write<Waypoint>(temp,&address);
	}
	
	void read(Waypoint *waypoint,uint16_t index) {
		uint16_t address = getEEPROMAddressFromWaypointIndex(index);
		Waypoint temp = NVM::read<Waypoint>(&address);
		waypoint->index = temp.index;
		waypoint->radius = temp.radius;
		waypoint->location.latitude = temp.location.latitude;
		waypoint->location.longitude = temp.location.longitude;
	}

	/** Calculates a XOR checksum of the waypoint stuct so that it is easy to verify
	 * if the correct waypoint is begin pursued without sending lots of bytes. */
	uint8_t getChecksum(const Waypoint *waypoint) {
		uint8_t checksum = 0xFF;
		for (uint8_t i = 0 ; i < sizeof(Waypoint) ; i++ ) {
			checksum ^= reinterpret_cast<const uint8_t*>(waypoint)[i];
		}
		return checksum;
	}
}

