#include "MessageManager.h"
#include <util/crc16.h>

#include "GPS_UBX.h"
#include "DCM.h"
#include "Captain.h"
#include "RemoteControl.h"
#include "Thruster.h"
#include "APM.h"

namespace {
	BLDCMonitor *bldc;
	PowerMonitor *power;
}

namespace Msg {
	MessageType::tlmstatus tlmstatus;
	MessageType::tlmdiagnostic tlmdiagnostic;
}

namespace MessageManager {
	uint8_t txBuffer[64];
	uint8_t rxBuffer[64];
	size_t txLength;
	size_t rxLength;
	
	void init(BLDCMonitor *_bldc,PowerMonitor *_power) {
	  bldc = _bldc;
	  power = _power;
	}
	
	void updateFields() {
#ifdef MM_TEST_ONLY

		Msg::tlmstatus.version					=					1;
		Msg::tlmstatus.format						=					0;
		Msg::tlmstatus.latitude					=					33.4;
		Msg::tlmstatus.longitude 				=					-118.8;
		Msg::tlmstatus.fix							=					1;
		Msg::tlmstatus.time							=					12345678;
		Msg::tlmstatus.altitude					=					0;
		Msg::tlmstatus.course						=					900;
		Msg::tlmstatus.heading					=					1800;
		Msg::tlmstatus.p_solar					=					120;
		Msg::tlmstatus.p_batt						=					15;
		Msg::tlmstatus.p_left						=					49;
		Msg::tlmstatus.p_right					=					51;
		Msg::tlmstatus.rotations				=					123;
		Msg::tlmstatus.rpm_left					=					100;
		Msg::tlmstatus.rpm_right				=					101;
		Msg::tlmstatus.swellHeight			=					53;
		Msg::tlmstatus.commandCount			=					1;
		Msg::tlmstatus.tempAir					=					25;
		Msg::tlmstatus.tempWater				=					22;
		Msg::tlmstatus.pressure					=					10130;
		Msg::tlmstatus.pH								=					820;
		Msg::tlmstatus.telemetryCount		=					4;

#else		

		Msg::tlmstatus.version									=					1;
		Msg::tlmstatus.format										=					3;
		Msg::tlmstatus.latitude									=					GPS_UBX::latitude;
		Msg::tlmstatus.longitude 								=					GPS_UBX::longitude;
		Msg::tlmstatus.fix											=					GPS_UBX::fix;
		Msg::tlmstatus.time											=					GPS_UBX::time;
		Msg::tlmstatus.altitude									=					GPS_UBX::altitude/2;
		Msg::tlmstatus.course										=					GPS_UBX::course*128/180;
		Msg::tlmstatus.heading									=					degrees(DCM::yaw)*128/180;
		Msg::tlmstatus.v_load										=         APM::getCorrectedVoltage()*1000;
		Msg::tlmstatus.v_solar        				  =         0.0*1000;
		Msg::tlmstatus.p_solar									=					120;
		Msg::tlmstatus.p_load 									=					APM::getPower();
		Msg::tlmstatus.p_left										=					49;
		Msg::tlmstatus.p_right									=					51;
		Msg::tlmstatus.rotations								=					123;
		Msg::tlmstatus.rpm_left									=					100;
		Msg::tlmstatus.rpm_right								=					101;
		Msg::tlmstatus.rollPitchRange						=					4;
		Msg::tlmstatus.swellPeriod							=					10;
		Msg::tlmstatus.swellHeight							=					53;
		Msg::tlmstatus.tempAir									=					25;
		Msg::tlmstatus.tempWater			        	=					22;
		Msg::tlmstatus.pressure				        	=					30;
		Msg::tlmstatus.pH								        =					-2;
		Msg::tlmstatus.salinity									=					100;
		Msg::tlmstatus.windSpeed								=					2;
		Msg::tlmstatus.windDirection						=					100;
		Msg::tlmstatus.status										=					0x0000;
		Msg::tlmstatus.currentWaypointIndex			=					Captain::waypoint.index;
		Msg::tlmstatus.commandCount							=					1;
		Msg::tlmstatus.telemetryCount		        =					4;

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
#endif
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
	
	size_t& getRXBufferLength() {
		return rxLength;
	}
}
