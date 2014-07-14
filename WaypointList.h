#ifndef WAYPOINTLIST_H
#define WAYPOINTLIST_H

#include <WProgram.h>
#include "Location.h"

struct Waypoint {
	uint16_t index;
	Location location;
};

namespace WaypointList {	
	void setWaypoint(uint16_t index, const Location &location);
	
	void getWaypoint(Waypoint *waypoint,uint16_t index);
}

#endif
