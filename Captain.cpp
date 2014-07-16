#include "Captain.h"
#include "Navigator.h"
#include "Helmsman.h"
#include "Persistant.h"
#include "WaypointList.h"
#include "GPS_UBX.h"
#include "HMC5883.h"
#include "MPU6000.h"
#include "DCM.h"
#include "RemoteControl.h"

namespace Captain {
	static const float waypointAchievedRadius = 3.0f;
	
  float desiredCourse;
  float desiredPower;
  float distanceToWaypoint;
  Waypoint current;
  Waypoint waypoint;

	void init() {
		// Get the current waypoint.
		Persistant::read();
		// TEMPORARY RESET WAYPOINTS UPON RESTART::
		Persistant::data.currentWaypointIndex = 0;
		// ::::::::::::::::::::::::::::::::::::::::
		WaypointList::read(&waypoint,Persistant::data.currentWaypointIndex);

		//waypoint.location.latitude = 33.870696; // The house across from me
		//waypoint.location.longitude = -118.368667; // The house across from me	
	}
	
	void determineState() {
		// Determine state of the vessel


		// Determine state of the mission
		distanceToWaypoint = Navigator::getDistanceToLocation(&current.location,&waypoint.location);
		if (distanceToWaypoint < waypointAchievedRadius) {
			getNextWaypoint();
		}
	
	}
	
	void determineCourseAndPower() {
	  current.location.latitude = GPS_UBX::latitude;
	  current.location.longitude = GPS_UBX::longitude;
	  
	  desiredCourse = Navigator::getHeadingToLocation(&current.location,&waypoint.location);

	  if (waypoint.location.latitude != 0.0f && waypoint.location.latitude != 0.0f) {
	  	desiredPower = 60;
	  } else {
	  	desiredPower = 0;
	  }
	}
	
	void execute() {
		Helmsman::setHeading(degrees(desiredCourse));
		Helmsman::setPower(desiredPower);
		if ( RemoteControl::isManual() ) {
			Helmsman::executeManual(RemoteControl::getSteering(),RemoteControl::getPower());
		} else {
			Helmsman::execute(degrees(DCM::yaw),0);
		}
	}

	void getNextWaypoint() {
		Persistant::data.currentWaypointIndex += 1;
		Persistant::write();
		WaypointList::read(&waypoint,Persistant::data.currentWaypointIndex);
	}

}
