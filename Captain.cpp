#include "Captain.h"
#include "Navigator.h"
#include "Helmsman.h"
#include "Location.h"
#include "GPS_UBX.h"
#include "HMC5883.h"
#include "MPU6000.h"
#include "DCM.h"
#include "RemoteControl.h"

namespace {
	Location current;
  Location wp;
}

namespace Captain {
  float desiredCourse;
  float distanceToWaypoint;

	void init() {
		wp.latitude = 21.3114; // Hawaii
		wp.longitude = -157.7964; // Hawaii
	
		wp.latitude = 33.870696; // The house across from me
		wp.longitude = -118.368667; // The house across from me	
	}
	
	void determineState() {
	
	}
	
	void determineCourseAndPower() {
	  current.latitude = GPS_UBX::latitude;
	  current.longitude = GPS_UBX::longitude;
	  
	  desiredCourse = Navigator::getHeadingToLocation(&current,&wp);
	  distanceToWaypoint = Navigator::getDistanceToLocation(&current,&wp);
	  
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
