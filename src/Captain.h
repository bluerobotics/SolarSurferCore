#ifndef CAPTAIN_H
#define CAPTAIN_H

#include <WProgram.h>
#include "WaypointList.h"
#include "BLDCMonitor.h"
#include "PowerMonitor.h"

namespace Captain {
  extern float desiredCourse;
  extern float desiredPower;
  extern float distanceToWaypoint;
  extern float headingToWaypoint;
  extern Waypoint current;
  extern Waypoint waypoint;
  extern Waypoint previous;

  extern bool forceThrustersOff;
  extern bool forceHeading;
  extern bool forcePositionHold;

	void init(BLDCMonitor *_bldcMonitor,PowerMonitor *_powerMonitor);
	
	void determineState();
	
	void determineCourseAndPower();
	
	void execute();

	void getNextWaypoint();

	void refreshWaypoint();

	uint8_t getStatus(uint8_t index);
}

#endif
