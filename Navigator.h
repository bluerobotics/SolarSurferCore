#include <WProgram.h>
#include "Location.h"

namespace Navigator {
	/** This function calculates the forward azimuth between the current location
	 * and the destination. The angle will change along the path so that the vehicle
	 * follows the great circle arc between the two points. */
	float getHeadingToLocation(Location *current, Location *waypoint);
}
