#ifndef CAPTAIN_H
#define CAPTAIN_H

#include <WProgram.h>
#include "WaypointList.h"

namespace Captain {
  extern float desiredCourse;
  extern float distanceToWaypoint;
  extern Waypoint current;
  extern Waypoint waypoint;

	void init();
	
	void determineState();
	
	void determineCourseAndPower();
	
	void execute();

	void getNextWaypoint();
}

#endif
