#include "Captain.h"
#include "Navigator.h"
#include "Helmsman.h"
#include "GPS_UBX.h"
#include "HMC5883.h"
#include "MPU6000.h"
#include "DCM.h"
#include "RemoteControl.h"

namespace Captain {
  float desiredCourse;
  float distanceToWaypoint;
  Waypoint current;
  Waypoint waypoint;

	void init() {
		waypoint.location.latitude = 21.3114; // Hawaii
		waypoint.location.longitude = -157.7964; // Hawaii
	
		waypoint.location.latitude = 33.870696; // The house across from me
		waypoint.location.longitude = -118.368667; // The house across from me	
	}
	
	void determineState() {
	
	}
	
	void determineCourseAndPower() {
	  current.location.latitude = GPS_UBX::latitude;
	  current.location.longitude = GPS_UBX::longitude;
	  
	  desiredCourse = Navigator::getHeadingToLocation(&current.location,&waypoint.location);
	  distanceToWaypoint = Navigator::getDistanceToLocation(&current.location,&waypoint.location);
	}
	
	void execute() {
		Helmsman::setHeading(degrees(desiredCourse));
		Helmsman::setPower(60);
		if ( RemoteControl::isManual() ) {
			Helmsman::executeManual(RemoteControl::getSteering(),RemoteControl::getPower());
		} else {
			Helmsman::execute(degrees(DCM::yaw),60.0);
		}
	}
}
