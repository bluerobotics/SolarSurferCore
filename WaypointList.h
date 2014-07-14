#ifndef WAYPOINTLIST_H
#define WAYPOINTLIST_H

#include <WProgram.h>
#include "Location.h"

struct Waypoint {
	uint16_t index;
	Location location;
};

namespace WaypointList {	
	void write(uint16_t index, const Location &location);
	
	void read(Waypoint *waypoint,uint16_t index);
}

#endif
