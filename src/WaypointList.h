#ifndef WAYPOINTLIST_H
#define WAYPOINTLIST_H

#include <WProgram.h>
#include "Location.h"

struct Waypoint {
	uint16_t index;
	uint8_t radius;
	Location location;
};

namespace WaypointList {	
	static const uint16_t maxWaypoints = 256; // 256 x 10 bytes = 2560 bytes

	void write(uint16_t index, uint8_t radius, const Location &location);
	
	void read(Waypoint *waypoint,uint16_t index);

	uint8_t getChecksum(const Waypoint *waypoint);
}

#endif
