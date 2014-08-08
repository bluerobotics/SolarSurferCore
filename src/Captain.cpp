#include "Captain.h"
#include "Navigator.h"
#include "Helmsman.h"
#include "Persistant.h"
#include "WaypointList.h"
#include "GPS_UBX.h"
#include "HMC5883.h"
#include "MPU6000.h"
#include "DCM.h"
#include "APM.h"
#include "RemoteControl.h"

namespace {
	BLDCMonitor* bldc;
  PowerMonitor* power;

  float desiredPowerErrorIntegral;

  float desiredPowerController(float error,float dt) {
		static const float Kp = 50.0;
		static const float Ki = 10.0;
		static const float iMax = 10.0;
		
		desiredPowerErrorIntegral = constrain(desiredPowerErrorIntegral+error*dt,-iMax,iMax);
		
		return Kp*error + Ki*desiredPowerErrorIntegral;
  }
}

namespace Captain {
	static const float waypointAchievedRadius = 3.0f;
	
  float desiredCourse;
  float desiredPower;
  float distanceToWaypoint;
  Waypoint current;
  Waypoint waypoint;

	void init(BLDCMonitor *_bldcMonitor,PowerMonitor *_powerMonitor) {
		bldc = _bldcMonitor;
		power = _powerMonitor;

		// Get the current waypoint
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
		static uint32_t lastTime;
		float dt = (millis()-lastTime)/1000.0f;
		lastTime = millis();

	  current.location.latitude = GPS_UBX::latitude;
	  current.location.longitude = GPS_UBX::longitude;
	  
	  desiredCourse = Navigator::getHeadingToLocation(&current.location,&waypoint.location);

	  if (waypoint.location.latitude != 0.0f && waypoint.location.latitude != 0.0f) {
	  	static const float desiredVoltage = 13.2;
	  	
	  	desiredPower = -desiredPowerController(desiredVoltage-APM::getCorrectedVoltage(),dt);
	  } else {
	  	desiredPowerErrorIntegral = 0;
	  	desiredPower = 0;
	  }

	  if ( RemoteControl::isManual() ) {
	  	desiredPowerErrorIntegral = 0;
	  }
	}
	
	void execute() {
		Helmsman::setHeading(degrees(desiredCourse));
		Helmsman::setPower(desiredPower);
		if ( RemoteControl::isManual() ) {
			Helmsman::executeManual(RemoteControl::getSteering(),RemoteControl::getPower());
		} else {
			Helmsman::execute(degrees(DCM::yaw),bldc->getTotalPower());
		}
	}

	void getNextWaypoint() {
		Persistant::data.currentWaypointIndex += 1;
		Persistant::write();
		WaypointList::read(&waypoint,Persistant::data.currentWaypointIndex);
	}

	void refreshWaypoint() {
		WaypointList::read(&waypoint,Persistant::data.currentWaypointIndex);	
	}

	uint8_t getStatus(uint8_t index) {
		switch(index) {
			case 1:
				return 0;
			case 2:
				return 0;
			default:
				return 0;
		}
	}
}
