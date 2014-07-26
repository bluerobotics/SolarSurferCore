#ifndef WAYPOINTLIST_H
#define WAYPOINTLIST_H

#include "Arduino.h"
#include "Location.h"

struct Waypoint {
	uint16_t index;
	Location location;
};

namespace WaypointList {	
	static const uint16_t maxWaypoints = 256; // 256 x 9 bytes = 2304 bytes

	void write(uint16_t index, const Location &location);
	
	void read(Waypoint *waypoint,uint16_t index);
}

#endif
