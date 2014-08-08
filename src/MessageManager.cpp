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

namespace {
	BLDCMonitor *bldc;
	PowerMonitor *power;
}

namespace Msg {
	MessageType::tlmstatus tlmstatus;
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

		Msg::tlmstatus.version									=					1;
		Msg::tlmstatus.format										=					2;
		Msg::tlmstatus.latitude									=					-118.3123456;
		Msg::tlmstatus.longitude 								=					33.4123456;
		Msg::tlmstatus.fix											=					3;
		Msg::tlmstatus.time											=					60;
		Msg::tlmstatus.altitude									=					2;
		Msg::tlmstatus.course										=					90*128/180;
		Msg::tlmstatus.heading									=					95*128/180;
		Msg::tlmstatus.v_load										=         13.201*1000;
		Msg::tlmstatus.v_solar        				  =         13.563*1000;
		Msg::tlmstatus.p_solar									=					110;
		Msg::tlmstatus.p_load 									=					90;
		Msg::tlmstatus.p_left										=					43;
		Msg::tlmstatus.p_right									=					44;
		Msg::tlmstatus.rotations								=					20;
		Msg::tlmstatus.rpm_left									=					4000/25;
		Msg::tlmstatus.rpm_right								=					4200/25;
		Msg::tlmstatus.rollPitchRange						=					(3*5);
		Msg::tlmstatus.swellPeriod							=					14;
		Msg::tlmstatus.swellHeight							=					(2.25*4);
		Msg::tlmstatus.tempAir									=					(20+20)*4;
		Msg::tlmstatus.tempWater			        	=					(25*6);
		Msg::tlmstatus.pressure				        	=					(103300-101300)/100;
		Msg::tlmstatus.pH								        =					(8.2-7.0)*50;
		Msg::tlmstatus.salinity									=					(35-27.5)*10;
		Msg::tlmstatus.windSpeed								=					200;
		Msg::tlmstatus.windDirection						=					60*128/180;
		Msg::tlmstatus.status1									=					0x01;
		Msg::tlmstatus.status2									=					0x00;
		Msg::tlmstatus.currentWaypointIndex			=					1;
		Msg::tlmstatus.currentWaypointChecksum	=					2;
		Msg::tlmstatus.commandCount							=					0;
		Msg::tlmstatus.telemetryCount		        =					1;
		Msg::tlmstatus.extra1										=					1;

#else		

		Msg::tlmstatus.version									=					1;
		Msg::tlmstatus.format										=					2;
		Msg::tlmstatus.latitude									=					GPS_UBX::latitude;
		Msg::tlmstatus.longitude 								=					GPS_UBX::longitude;
		Msg::tlmstatus.fix											=					GPS_UBX::fix;
		Msg::tlmstatus.time											=					GPS_UBX::time/1000/60;
		Msg::tlmstatus.altitude									=					GPS_UBX::altitude/2;
		Msg::tlmstatus.course										=					GPS_UBX::course*128/180;
		Msg::tlmstatus.heading									=					degrees(DCM::yaw)*128/180;
		Msg::tlmstatus.v_load										=         APM::getCorrectedVoltage()*1000;
		Msg::tlmstatus.v_solar        				  =         0.0*1000;
		Msg::tlmstatus.p_solar									=					0;
		Msg::tlmstatus.p_load 									=					APM::getPower();
		Msg::tlmstatus.p_left										=					0;
		Msg::tlmstatus.p_right									=					0;
		Msg::tlmstatus.rotations								=					0;
		Msg::tlmstatus.rpm_left									=					0;
		Msg::tlmstatus.rpm_right								=					0;
		Msg::tlmstatus.rollPitchRange						=					0;
		Msg::tlmstatus.swellPeriod							=					0;
		Msg::tlmstatus.swellHeight							=					0;
		Msg::tlmstatus.tempAir									=					0;
		Msg::tlmstatus.tempWater			        	=					0;
		Msg::tlmstatus.pressure				        	=					0;
		Msg::tlmstatus.pH								        =					0;
		Msg::tlmstatus.salinity									=					0;
		Msg::tlmstatus.windSpeed								=					0;
		Msg::tlmstatus.windDirection						=					0;
		Msg::tlmstatus.status1									=					0x00;
		Msg::tlmstatus.status2									=					0x00;
		Msg::tlmstatus.currentWaypointIndex			=					Captain::waypoint.index;
		Msg::tlmstatus.currentWaypointChecksum	=					2;
		Msg::tlmstatus.commandCount							=					0;
		Msg::tlmstatus.telemetryCount		        =					1;
		Msg::tlmstatus.extra1										=					1;

		Msg::tlmdiagnostic.version						=					1;
		Msg::tlmdiagnostic.format							=					5;
		Msg::tlmdiagnostic.gpsTime						=					GPS_UBX::time;
		Msg::tlmdiagnostic.gpsLatitude				=					GPS_UBX::latitude;
		Msg::tlmdiagnostic.gpsLongitude				=					GPS_UBX::longitude;
		Msg::tlmdiagnostic.gpsAltitude				=					GPS_UBX::altitude;
		Msg::tlmdiagnostic.gpsGroundSpeed			=					GPS_UBX::groundSpeed;
		Msg::tlmdiagnostic.gpsCourse					=					degrees(GPS_UBX::course);
		Msg::tlmdiagnostic.wpIndex						=					Captain::waypoint.index;
		Msg::tlmdiagnostic.wpLatitude					=					Captain::waypoint.location.latitude;
		Msg::tlmdiagnostic.wpLongitude				=					Captain::waypoint.location.longitude;
		Msg::tlmdiagnostic.roll								=					degrees(DCM::roll);
		Msg::tlmdiagnostic.pitch							=					degrees(DCM::pitch);
		Msg::tlmdiagnostic.heading						=					degrees(DCM::yaw);
		Msg::tlmdiagnostic.distanceToWaypoint	=					Captain::distanceToWaypoint;
		Msg::tlmdiagnostic.desiredCourse			=					degrees(Captain::desiredCourse);
		Msg::tlmdiagnostic.desiredPower				=					Captain::desiredPower;
		Msg::tlmdiagnostic.leftThruster				=					Thruster::get(Thruster::left);
		Msg::tlmdiagnostic.rightThruster			=					Thruster::get(Thruster::right);
		Msg::tlmdiagnostic.isManual						=					RemoteControl::isManual();
		Msg::tlmdiagnostic.isOther						=					RemoteControl::isOther();	
		Msg::tlmdiagnostic.rcSteering					=					RemoteControl::getSteering();
		Msg::tlmdiagnostic.rcPower						=					RemoteControl::getPower();
		Msg::tlmdiagnostic.voltageAPM         =         APM::getCorrectedVoltage();
		Msg::tlmdiagnostic.voltageSolar       =         power->data.voltage[PowerMonitor::SolarToCC];
		Msg::tlmdiagnostic.voltageBattery     =         power->data.voltage[PowerMonitor::CCtoBattery];
		Msg::tlmdiagnostic.voltageThrusters   =         bldc->data.voltage;
		Msg::tlmdiagnostic.powerAPM           =         APM::getPower();
		Msg::tlmdiagnostic.powerThrusters     =         bldc->getTotalPower();
		Msg::tlmdiagnostic.powerSolar         =         power->getPower(PowerMonitor::SolarToCC);
		Msg::tlmdiagnostic.powerCharge        =         power->getPower(PowerMonitor::CCtoBattery)-power->getPower(PowerMonitor::BatteryToLoad);
		Msg::tlmdiagnostic.powerLoad          =         power->getPower(PowerMonitor::BatteryToLoad);
		Msg::tlmdiagnostic.cmdtelemetryPeriod =         Persistant::data.telemetryEnum;
		Msg::tlmdiagnostic.cmdforceMode       =         Persistant::data.forceMode;
		Msg::tlmdiagnostic.cmdforceHeading    =         Persistant::data.forceHeading*128/180;
                Msg::tlmdiagnostic.cmdgoalVoltage     =         Persistant::data.goalVoltage*1000;
                Msg::tlmdiagnostic.cmdforceCurrentWaypointIndex = Persistant::data.currentWaypointIndex;
		Msg::tlmdiagnostic.inCallback         =         NonPersistant::data.inCallback;
		Msg::tlmdiagnostic.satcomErrorCode    =         NonPersistant::data.lastISBDError;
#endif
	}

	void processCommand() {
		// Telemetry period
		Persistant::data.telemetryEnum = Msg::cmdcontrol.telemetryPeriod;
		switch ( Msg::cmdcontrol.telemetryPeriod ) {
			case 0:
				break;
			case 1:
			  Persistant::data.telemetryPeriod = 5*60000;
			  break;
			case 2:
			  Persistant::data.telemetryPeriod = 10*60000;
			  break;
			case 3:
			  Persistant::data.telemetryPeriod = 20*60000;
			  break;
			case 4:
			  Persistant::data.telemetryPeriod = 30*60000;
			  break;
			case 5:
			  Persistant::data.telemetryPeriod = 45*60000;
			  break;
			case 6:
			  Persistant::data.telemetryPeriod = 60*60000;
			  break;
			case 7:
			  Persistant::data.telemetryPeriod = 90*60000;
			  break;
			case 8:
			  Persistant::data.telemetryPeriod = 120*60000;
			  break;
			case 9:
			  Persistant::data.telemetryPeriod = 180*60000;
			  break;
			case 10:
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
			Persistant::data.currentWaypointIndex = Msg::cmdcontrol.forceCurrentWaypointIndex;
			Captain::refreshWaypoint();
		}

		// Update waypoints if necessary
		Location tempLoc;
		if ( Msg::cmdcontrol.waypointID1 > 0 && Msg::cmdcontrol.waypointRadius1 > 0 ) {
			tempLoc.latitude      = Msg::cmdcontrol.waypointLat1;
			tempLoc.longitude     = Msg::cmdcontrol.waypointLon1;
			WaypointList::write(Msg::cmdcontrol.waypointID1,Msg::cmdcontrol.waypointRadius1,tempLoc);
		}
		if ( Msg::cmdcontrol.waypointID2 > 0 && Msg::cmdcontrol.waypointRadius2 > 0 ) {
			tempLoc.latitude      = Msg::cmdcontrol.waypointLat2;
			tempLoc.longitude     = Msg::cmdcontrol.waypointLon2;
			WaypointList::write(Msg::cmdcontrol.waypointID2,Msg::cmdcontrol.waypointRadius2,tempLoc);
		}
		if ( Msg::cmdcontrol.waypointID3 > 0 && Msg::cmdcontrol.waypointRadius3 > 0 ) {
			tempLoc.latitude      = Msg::cmdcontrol.waypointLat3;
			tempLoc.longitude     = Msg::cmdcontrol.waypointLon3;
			WaypointList::write(Msg::cmdcontrol.waypointID3,Msg::cmdcontrol.waypointRadius3,tempLoc);
		}
		if ( Msg::cmdcontrol.waypointID4 > 0 && Msg::cmdcontrol.waypointRadius4 > 0 ) {
			tempLoc.latitude      = Msg::cmdcontrol.waypointLat4;
			tempLoc.longitude     = Msg::cmdcontrol.waypointLon4;
			WaypointList::write(Msg::cmdcontrol.waypointID4,Msg::cmdcontrol.waypointRadius4,tempLoc);
		}
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
