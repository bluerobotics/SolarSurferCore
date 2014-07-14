#include "MessageManager.h"
#include "struct/struct.h"
#include <util/crc16.h>

#include "GPS_UBX.h"

#define TEST_ONLY

namespace Msg {
	MessageType::tlmstatus tlmstatus;
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
		Msg::tlmstatus.format						=					0;
		Msg::tlmstatus.latitude					=					GPS_UBX::latitude;
		Msg::tlmstatus.longitude 				=					GPS_UBX::longitude;
		Msg::tlmstatus.fix							=					GPS_UBX::fix;
		Msg::tlmstatus.time							=					GPS_UBX::time;
		Msg::tlmstatus.altitude					=					GPS_UBX::altitude;
		Msg::tlmstatus.course						=					GPS_UBX::course;
		Msg::tlmstatus.heading					=					900;
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
