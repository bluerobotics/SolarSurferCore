#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <WProgram.h>
#include "Location.h"

namespace Navigator {
	/** This function calculates the forward azimuth between the current location
	 * and the destination. The angle will change along the path so that the vehicle
	 * follows the great circle arc between the two points. */
	float getHeadingToLocation(Location *current, Location *waypoint);
	
	/** This function uses the haversine formula to calculate the great circle distance
	 * between two points. This will be used to determine if a waypoint has been achieved
	 * or not. */
	float getDistanceToLocation(Location *current, Location *waypoint);
}

#endif