#include "MessageManager.h"
#include <util/crc16.h>

#include "GPS_UBX.h"
#include "DCM.h"
#include "Captain.h"
#include "RemoteControl.h"
#include "Thruster.h"
#include "APM.h"
#include "Persistant.h"
#include "NonPersistant.h"
#include "DS18XXX.h"
#include "WaveMotion.h"

namespace {
	BLDCMonitor *bldc;
	PowerMonitor *power;
}

namespace Msg {
	MessageType::tlmshortStatus tlmshortStatus;
	MessageType::tlmlongStatus tlmlongStatus;
	MessageType::tlmdiagnostic tlmdiagnostic;
	MessageType::cmdcontrol cmdcontrol;
}

namespace MessageManager {
	uint8_t txBuffer[MM_TX_BUFFER_SIZE];
	uint8_t rxBuffer[MM_RX_BUFFER_SIZE];
	size_t txLength;
	size_t rxLength;
	
	void init(BLDCMonitor *_bldc,PowerMonitor *_power) {
	  bldc = _bldc;
	  power = _power;
	}
	
	void updateFields() {
#ifdef MM_TEST_ONLY
		Msg::cmdcontrol._version                 =         2;
		Msg::cmdcontrol._format                  =         3;
		Msg::cmdcontrol.telemetryPeriod         =         2;
		Msg::cmdcontrol.forceMode               =         0;
		Msg::cmdcontrol.forceHeading            =         0;
		Msg::cmdcontrol.goalVoltage             =         13200;
		Msg::cmdcontrol.forceCurrentWaypointIndex =       0;
		Msg::cmdcontrol.waypointID1             =         0;
		Msg::cmdcontrol.waypointRadius1         =         3;
		Msg::cmdcontrol.waypointLat1            =         33.962725f;
		Msg::cmdcontrol.waypointLon1            =         -118.454250f;
		Msg::cmdcontrol.waypointID2             =         0;
		Msg::cmdcontrol.waypointRadius2         =         3;
		Msg::cmdcontrol.waypointLat2            =         33.962018f;
		Msg::cmdcontrol.waypointLon2            =         -118.455902f;
		Msg::cmdcontrol.waypointID3             =         0;
		Msg::cmdcontrol.waypointRadius3         =         3;
		Msg::cmdcontrol.waypointLat3            =         33.962725f;
		Msg::cmdcontrol.waypointLon3            =         -118.454250f;
		Msg::cmdcontrol.waypointID4             =         0;
		Msg::cmdcontrol.waypointRadius4         =         3;
		Msg::cmdcontrol.waypointLat4            =         33.962018f;
		Msg::cmdcontrol.waypointLon4            =         -118.455902f;

#else		

		Msg::tlmshortStatus._version											=					2;
		Msg::tlmshortStatus._format												=					2;
		Msg::tlmshortStatus.time													=					GPS_UBX::time;
		Msg::tlmshortStatus.fix														=					GPS_UBX::fix;
		Msg::tlmshortStatus.latitude											=					GPS_UBX::latitude;
		Msg::tlmshortStatus.longitude 										=					GPS_UBX::longitude;
		Msg::tlmshortStatus.altitude											=					GPS_UBX::altitude/2;
		Msg::tlmshortStatus.course												=					GPS_UBX::course*128/180;
		Msg::tlmshortStatus.heading												=					degrees(DCM::yaw)*128/180;
		Msg::tlmshortStatus.currentWaypointIndex					=					Captain::waypoint.index;
		Msg::tlmshortStatus.currentWaypointLatitude				=					Captain::waypoint.location.latitude;
		Msg::tlmshortStatus.currentWaypointLongitude			=					Captain::waypoint.location.longitude;
		Msg::tlmshortStatus.status1												=					Captain::getStatus(1);
		Msg::tlmshortStatus.status2												=					Captain::getStatus(2);		
		Msg::tlmshortStatus.commandCount									=					Persistant::data.commandCount;
		Msg::tlmshortStatus.telemetryCount		       			=					Persistant::data.telemetryCount;
		Msg::tlmshortStatus.v_load												=         APM::getCorrectedVoltage()*1000;
		Msg::tlmshortStatus.p_load 												=					APM::getPower();
		Msg::tlmshortStatus.p_left												=					bldc->getPower(0);
		Msg::tlmshortStatus.p_right												=					bldc->getPower(1);
		Msg::tlmshortStatus.rpm_left											=					bldc->getRPM(0)/25;
		Msg::tlmshortStatus.rpm_right											=					bldc->getRPM(1)/25;
		Msg::tlmshortStatus.rollPitchRange								=					WaveMotion::rollPitchRange*5;
		Msg::tlmshortStatus.swellPeriod										=					WaveMotion::swellPeriod;
		Msg::tlmshortStatus.swellHeight										=					WaveMotion::swellHeight*4;
		Msg::tlmshortStatus.tempWater			    			    	=					DS18XXX::temperature*6;
		//Msg::tlmshortStatus.extra													=					{0,0,0,0};

		Msg::tlmdiagnostic._version						=					2;
		Msg::tlmdiagnostic._format					  =				  5;
		Msg::tlmdiagnostic.gpsTime						=					GPS_UBX::time;
		Msg::tlmdiagnostic.gpsLatitude				=					GPS_UBX::latitude;
		Msg::tlmdiagnostic.gpsLongitude				=					GPS_UBX::longitude;
		Msg::tlmdiagnostic.gpsAltitude				=					GPS_UBX::altitude;
		Msg::tlmdiagnostic.gpsGroundSpeed			=					GPS_UBX::groundSpeed;
		Msg::tlmdiagnostic.gpsCourse					=					degrees(GPS_UBX::course);
		Msg::tlmdiagnostic.wpIndex						=					Captain::waypoint.index;
		Msg::tlmdiagnostic.wpLatitude					=					Captain::waypoint.location.latitude;
		Msg::tlmdiagnostic.wpLongitude				=					Captain::waypoint.location.longitude;
		Msg::tlmdiagnostic.waypointRadius 		=					Captain::waypoint.radius;		
		Msg::tlmdiagnostic.distanceToWaypoint	=					Captain::distanceToWaypoint;
		Msg::tlmdiagnostic.headingToWaypoint	=					degrees(Captain::headingToWaypoint);
		Msg::tlmdiagnostic.heading						=					degrees(DCM::yaw);
		Msg::tlmdiagnostic.desiredCourse			=					degrees(Captain::desiredCourse);
		Msg::tlmdiagnostic.prevIndex					=					Captain::previous.index;
		Msg::tlmdiagnostic.prevLatitude	  		=					Captain::previous.location.latitude;
		Msg::tlmdiagnostic.prevLongitude	  	=					Captain::previous.location.longitude;
		Msg::tlmdiagnostic.roll								=					degrees(DCM::roll);
		Msg::tlmdiagnostic.pitch							=					degrees(DCM::pitch);
		Msg::tlmdiagnostic.desiredPower				=					Captain::desiredPower;
		Msg::tlmdiagnostic.leftThruster				=					Thruster::get(Thruster::left);
		Msg::tlmdiagnostic.rightThruster			=					Thruster::get(Thruster::right);
		Msg::tlmdiagnostic.isManual						=					RemoteControl::isManual();
		Msg::tlmdiagnostic.isOther						=					RemoteControl::isOther();	
		Msg::tlmdiagnostic.rcSteering					=					RemoteControl::getSteering();
		Msg::tlmdiagnostic.rcPower						=					RemoteControl::getPower();
		Msg::tlmdiagnostic.voltageAPM         =         APM::getCorrectedVoltage();
		Msg::tlmdiagnostic.powerAPM           =         APM::getPower();
		Msg::tlmdiagnostic.powerThrusters     =         bldc->getTotalPower();
		Msg::tlmdiagnostic.rpmLeft            =         bldc->getRPM(0);
		Msg::tlmdiagnostic.rpmRight           =         bldc->getRPM(0);
		Msg::tlmdiagnostic.cmdtelemetryPeriod =         Persistant::data.telemetryEnum;
		Msg::tlmdiagnostic.cmdforceMode       =         Persistant::data.forceMode;
		Msg::tlmdiagnostic.forceThrustersOff	=					Captain::forceThrustersOff;
		Msg::tlmdiagnostic.forceHeading				=					Captain::forceHeading;
		Msg::tlmdiagnostic.forceHoldPosition	=					Captain::forcePositionHold;
		Msg::tlmdiagnostic.forceSeaweedRemoval=					Captain::forceSeaweedRemoval;
		Msg::tlmdiagnostic.cmdforceHeading    =         Persistant::data.forceHeading*128/180;
    Msg::tlmdiagnostic.cmdgoalVoltage     =         Persistant::data.goalVoltage*1000;
    Msg::tlmdiagnostic.cmdforceCurrentWaypointIndex = Persistant::data.currentWaypointIndex;
		Msg::tlmdiagnostic.inCallback         =         NonPersistant::data.inCallback;
		Msg::tlmdiagnostic.satcomSignal       =         NonPersistant::data.lastSignalStrength;
		Msg::tlmdiagnostic.satcomErrorCode    =         NonPersistant::data.lastISBDError;
		Msg::tlmdiagnostic.telemetryCount			=					Persistant::data.telemetryCount;
		Msg::tlmdiagnostic.commandCount				=					Persistant::data.commandCount;
		Msg::tlmdiagnostic.timeTillNextSatcom =         NonPersistant::data.timeTillNextSatcom;
		Msg::tlmdiagnostic.status1						=					Captain::getStatus(1);
		Msg::tlmdiagnostic.status2						=					Captain::getStatus(2);
		Msg::tlmdiagnostic.rollPitchRange     =         WaveMotion::rollPitchRange;
		Msg::tlmdiagnostic.swellPeriod        =         WaveMotion::swellPeriod;
		Msg::tlmdiagnostic.swellHeight        =         WaveMotion::swellHeight;
		Msg::tlmdiagnostic.tempAir            =         -1;
		Msg::tlmdiagnostic.tempWater          =         DS18XXX::temperature;
		Msg::tlmdiagnostic.pressure           =         -1;
		Msg::tlmdiagnostic.pH                 =         -1;
		Msg::tlmdiagnostic.salinity           =         -1;
		Msg::tlmdiagnostic.windSpeed          =         -1;
		Msg::tlmdiagnostic.windDirection      =         -1;
#endif
	}

	void processCommand() {
		// Telemetry period
		if ( Msg::cmdcontrol.telemetryPeriod > 0 ) {
			Persistant::data.telemetryEnum = Msg::cmdcontrol.telemetryPeriod;
		}
		switch ( Msg::cmdcontrol.telemetryPeriod ) {
			case 0:
				break;
			case 1:
				Persistant::data.telemetryPeriod = 2.5*60000;
				break;
			case 2:
			  Persistant::data.telemetryPeriod = 5*60000;
			  break;
			case 3:
			  Persistant::data.telemetryPeriod = 10*60000;
			  break;
			case 4:
			  Persistant::data.telemetryPeriod = 20*60000;
			  break;
			case 5:
			  Persistant::data.telemetryPeriod = 30*60000;
			  break;
			case 6:
			  Persistant::data.telemetryPeriod = 45*60000;
			  break;
			case 7:
			  Persistant::data.telemetryPeriod = 60*60000;
			  break;
			case 8:
			  Persistant::data.telemetryPeriod = 90*60000;
			  break;
			case 9:
			  Persistant::data.telemetryPeriod = 120*60000;
			  break;
			case 10:
			  Persistant::data.telemetryPeriod = 180*60000;
			  break;
			case 11:
			  Persistant::data.telemetryPeriod = 360*60000;
			  break;
			default:
				Persistant::data.telemetryPeriod = 30*60000;
		}

		// Force mode
		Persistant::data.forceMode = Msg::cmdcontrol.forceMode;

		// Force heading and voltage
		Persistant::data.forceHeading = Msg::cmdcontrol.forceHeading*180.0f/128.0f;

		if ( Msg::cmdcontrol.goalVoltage > 1000 ) {
			Persistant::data.goalVoltage = Msg::cmdcontrol.goalVoltage*0.001;
		}

		// Force current waypoint index
		if ( Msg::cmdcontrol.forceCurrentWaypointIndex > 0 ) {
			Persistant::data.lastWaypointIndex = Msg::cmdcontrol.forceCurrentWaypointIndex; // Prevents tracking weird vectors.
			Persistant::data.currentWaypointIndex = Msg::cmdcontrol.forceCurrentWaypointIndex;
			Captain::refreshWaypoint();
		}

		Persistant::write();

		// Update waypoints if necessary
		Location tempLoc;
		if ( Msg::cmdcontrol.waypointID1 > 0 ) {
			tempLoc.latitude      = Msg::cmdcontrol.waypointLat1;
			tempLoc.longitude     = Msg::cmdcontrol.waypointLon1;
			WaypointList::write(Msg::cmdcontrol.waypointID1,Msg::cmdcontrol.waypointRadius1,tempLoc);
		}
		if ( Msg::cmdcontrol.waypointID2 > 0 ) {
			tempLoc.latitude      = Msg::cmdcontrol.waypointLat2;
			tempLoc.longitude     = Msg::cmdcontrol.waypointLon2;
			WaypointList::write(Msg::cmdcontrol.waypointID2,Msg::cmdcontrol.waypointRadius2,tempLoc);
		}
		if ( Msg::cmdcontrol.waypointID3 > 0 ) {
			tempLoc.latitude      = Msg::cmdcontrol.waypointLat3;
			tempLoc.longitude     = Msg::cmdcontrol.waypointLon3;
			WaypointList::write(Msg::cmdcontrol.waypointID3,Msg::cmdcontrol.waypointRadius3,tempLoc);
		}
		if ( Msg::cmdcontrol.waypointID4 > 0 ) {
			tempLoc.latitude      = Msg::cmdcontrol.waypointLat4;
			tempLoc.longitude     = Msg::cmdcontrol.waypointLon4;
			WaypointList::write(Msg::cmdcontrol.waypointID4,Msg::cmdcontrol.waypointRadius4,tempLoc);
		}
		Captain::refreshWaypoint();
	}
	
	uint8_t* getRXBuffer() {
		return rxBuffer;
	}
	
	const uint8_t* getTXBuffer() {
		return txBuffer;
	}
	
	size_t getTXBufferLength() {
		return txLength;
	}
}
