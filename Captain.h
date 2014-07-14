#ifndef CAPTAIN_H
#define CAPTAIN_H

#include <WProgram.h>

namespace Captain {
  extern float desiredCourse;
  extern float distanceToWaypoint;

	void init();
	
	void determineState();
	
	void determineCourseAndPower();
	
	void execute();
}

#endif
