#include "MessageManager.h"
#include <util/crc16.h>

#include "GPS_UBX.h"
#include "DCM.h"
#include "Captain.h"
#include "RemoteControl.h"
#include "Thruster.h"

//#define TEST_ONLY

namespace Msg {
	MessageType::tlmstatus tlmstatus;
	MessageType::tlmdiagnostic tlmdiagnostic;
}

namespace MessageManager {
	uint8_t txBuffer[128];
	uint8_t rxBuffer[128];
	size_t txLength;
	size_t rxLength;
	
	void init() {

	}
	
	void updateFields() {
#ifndef TEST_ONLY
		Msg::tlmstatus.version					=					1;
		Msg::tlmstatus.format						=					3;
		Msg::tlmstatus.latitude					=					GPS_UBX::latitude;
		Msg::tlmstatus.longitude 				=					GPS_UBX::longitude;
		Msg::tlmstatus.fix							=					GPS_UBX::fix;
		Msg::tlmstatus.time							=					GPS_UBX::time;
		Msg::tlmstatus.altitude					=					GPS_UBX::altitude;
		Msg::tlmstatus.course						=					GPS_UBX::course;
		Msg::tlmstatus.heading					=					degrees(DCM::yaw)*10;
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

		Msg::tlmdiagnostic.version						=					1;
		Msg::tlmdiagnostic.format							=					5;
		Msg::tlmdiagnostic.gpsTime						=					GPS_UBX::time;
		Msg::tlmdiagnostic.gpsLatitude				=					GPS_UBX::latitude;
		Msg::tlmdiagnostic.gpsLongitude				=					GPS_UBX::longitude;
		Msg::tlmdiagnostic.gpsAltitude				=					GPS_UBX::altitude;
		Msg::tlmdiagnostic.gpsGroundSpeed			=					GPS_UBX::groundSpeed;
		Msg::tlmdiagnostic.gpsCourse					=					GPS_UBX::course;
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

#else
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
		Msg::tlmstatus.telemetryRate		=					2;
		Msg::tlmstatus.telemetryCount		=					4;
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
