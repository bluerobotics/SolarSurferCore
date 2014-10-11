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

  uint8_t status1;
  uint8_t status2;

  float desiredPowerErrorIntegral;

  uint32_t positionHoldingTimer;

  float desiredPowerController(float error,float dt) {
		static const float Kp = 50.0;
		static const float Ki = 10.0;
		static const float iMax = 10.0;
		
		desiredPowerErrorIntegral = constrain(desiredPowerErrorIntegral+error*dt,-iMax,iMax);
		
		return Kp*error + Ki*desiredPowerErrorIntegral;
  }
}

namespace Captain {	
  float desiredCourse;
  float desiredPower;
  float distanceToWaypoint;
  float headingToWaypoint;
  Waypoint current;
  Waypoint previous;
  Waypoint waypoint;
  Waypoint holding;

  bool forceThrustersOff;
  bool forceHeading;
  bool forcePositionHold;

	void init(BLDCMonitor *_bldcMonitor,PowerMonitor *_powerMonitor) {
		bldc = _bldcMonitor;
		power = _powerMonitor;

		// Get the current waypoint
		WaypointList::read(&waypoint,Persistant::data.currentWaypointIndex);
		WaypointList::read(&previous,Persistant::data.lastWaypointIndex);

		//waypoint.location.latitude = 33.870696; // The house across from me
		//waypoint.location.longitude = -118.368667; // The house across from me	
	}
	
	void determineState() {
		// Determine mode

		if ( Persistant::data.forceMode & (1<<0) ) {
			// Thrusters off
			forceThrustersOff = true;
		} else {
			forceThrustersOff = false;
		}

		if ( Persistant::data.forceMode & (1<<1) ) {
			// Force Heading
			forceHeading = true;
		} else {
			forceHeading = false;
		}

		if ( Persistant::data.forceMode & (1<<2) ) {
			// Force Hold Position
			if ( !forcePositionHold ) {
				// Set current position as holding position
				holding = current;
				positionHoldingTimer = millis();
			}
			forcePositionHold = true;
		} else {
			forcePositionHold = false;
		}

		// Determine state of the vessel

		status1 = 0;
		status2 = 0;

		// Status 1:
		// 0 - Null
		// 1 - GPS Okay
		// 2 - IMU Okay
		// 3 - Compass Okay
		// 4 - BLDC Monitor Okay
		// 5 - Satcom Okay
		// 6 - RC Radio Connected

		// GPS
		static uint32_t lastGPSTime;
		static uint32_t lastGPSCheck;
		static bool gpsLost;
		if ( millis() - lastGPSCheck > 10000 ) {
			lastGPSCheck = millis();
			if ( GPS_UBX::time == lastGPSTime ) {
				gpsLost = true;
			} else {
				gpsLost = false;
			}
			lastGPSTime = GPS_UBX::time;
		}
		if ( !gpsLost ) {
			status1 |= (1 << 1);
		}

		// IMU
		static float lastAccel;
		static uint32_t lastIMUCheck;
		static bool imuLost;
		if ( millis() - lastIMUCheck > 10000 ) {
			lastIMUCheck = millis();
			if ( MPU6000::accelX == lastAccel ) {
				imuLost = true;
			} else {
				imuLost = false;
			}
			lastAccel = MPU6000::accelX;
		}
		if ( !imuLost ) {
			status1 |= (1 << 2);
		}

		// Compass
		static float lastMagX;
		static uint32_t lastMagCheck;
		static bool magLost;
		if ( millis() - lastMagCheck > 10000 ) {
			lastMagCheck = millis();
			if ( HMC5883::mag_x == lastMagX ) {
				magLost = true;
			} else {
				magLost = false;
			}
			lastMagX = HMC5883::mag_x;
		}
		if ( !magLost ) {
			status1 |= (1 << 3);
		}

		// Status 2:
		// 0 - Null
		// 1 - Thruster 1 Okay
		// 2 - Thruster 2 Okay
		// 3 - Thruster 3 Okay
		// 4 - Thruster 4 Okay
		// 5 - Navigation Box Dry
		// 6 - Battery Box Dry
		// 7 - 


		// Determine state of the mission
		distanceToWaypoint = Navigator::getDistanceToLocation(&current.location,&waypoint.location);
		if (distanceToWaypoint < waypoint.radius) {
			getNextWaypoint();
		}

		headingToWaypoint = Navigator::getHeadingToLocation(&current.location,&waypoint.location);
	}
	
	void determineCourseAndPower() {
		static const float trackingCorrectionGain = 4.0; // degrees/rad of correction per degree/rad of error (constrained)
		static const float maxTrackingCorrectionAngle = radians(20); // 20 deg

		static uint32_t lastTime;
		float dt = (millis()-lastTime)/1000.0f;
		lastTime = millis();

	  current.location.latitude = GPS_UBX::latitude;
	  current.location.longitude = GPS_UBX::longitude;

	  // New navigation code
	  float headingPrevToNext = Navigator::getHeadingToLocation(&previous.location,&waypoint.location);
	  float headingHereToNext = Navigator::getHeadingToLocation(&current.location,&waypoint.location);

	  float trackingCorrectionAngle = trackingCorrectionGain*Navigator::getAngleBetweenHeadings(headingHereToNext,headingPrevToNext);
	  trackingCorrectionAngle = constrain(trackingCorrectionAngle,-maxTrackingCorrectionAngle,maxTrackingCorrectionAngle);

	  if (previous.index == 0) {
	  	trackingCorrectionAngle = 0;
	  }

	  desiredCourse = headingHereToNext + trackingCorrectionAngle;
	  
	  // Old navigation code
	  //desiredCourse = Navigator::getHeadingToLocation(&current.location,&waypoint.location);

	  if (waypoint.location.latitude != 0.0f && waypoint.location.latitude != 0.0f) {
	  	static const float desiredVoltage = 13.2;
	  	
	  	desiredPower = -desiredPowerController(desiredVoltage-APM::getCorrectedVoltage(),dt);
	  } else {
	  	desiredPowerErrorIntegral = 0;
	  	desiredPower = 0;
	  }

	  // Force cases
	  static const uint32_t positionHoldPause = 120000; // ms
	  static const uint32_t positionHoldRadius = 6; // m

	  if ( forcePositionHold ) {
	  	if ( millis() - positionHoldingTimer < positionHoldPause ) {
	  		desiredPower = 0;
	  	} else {
	  		if ( Navigator::getDistanceToLocation(&current.location,&holding.location) < positionHoldRadius ) {
	  			positionHoldingTimer = millis();
	  		}
	  	}
	  }

	  if ( forceHeading ) {
	  	desiredCourse = Persistant::data.forceHeading;
	  }

	  if ( forceThrustersOff ) {
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
		Persistant::data.lastWaypointIndex = Persistant::data.currentWaypointIndex;
		Persistant::data.currentWaypointIndex += 1;
		Persistant::write();
		WaypointList::read(&waypoint,Persistant::data.currentWaypointIndex);
		WaypointList::read(&previous,Persistant::data.lastWaypointIndex);
	}

	void refreshWaypoint() {
		WaypointList::read(&waypoint,Persistant::data.currentWaypointIndex);
		WaypointList::read(&previous,Persistant::data.lastWaypointIndex);		
	}

	uint8_t getStatus(uint8_t index) {
		switch(index) {
			case 1:
				return status1;
			case 2:
				return status2;
			default:
				return 0;
		}
	}
}
