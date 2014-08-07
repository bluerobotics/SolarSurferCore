#ifndef MESSAGES_H630903
#define MESSAGES_H630903

/* This file was automatically generated. DO NOT EDIT! */

#include <WProgram.h>

namespace MessageType {

struct teststring {
	uint8_t version; // 
	uint8_t format; // 
	char message[46]; // 
	uint16_t checksum; // 
};
struct testmixed {
	uint8_t version; // 
	uint8_t format; // 
	uint8_t test_uint8_t; // 
	int8_t test_int8_t; // 
	uint8_t test_enum; // 0: this; 1: that; 
	uint8_t test_bitmap; // 
	int8_t test_convert; // undefined, undefined, 0,1
	uint16_t checksum; // 
};
struct tlmstatus {
	uint8_t version; // 
	uint8_t format; // 
	float latitude; // undefined, undefined, 0,1
	float longitude; // undefined, undefined, 0,1
	uint8_t fix; // 0: No Fix; 1: Dead Reckoning Only; 2: 2D-Fix; 3: 3D-Fix; 4: GPS+Dead Reckoning; 5: Time Only; 
	uint16_t time; // undefined, undefined, 0,1
	int8_t altitude; // undefined, undefined, 0,2
	int8_t course; // undefined, undefined, 0,1.40625
	int8_t heading; // undefined, undefined, 0,1.40625
	uint16_t v_load; // undefined, undefined, 0,0.001
	uint16_t v_solar; // undefined, undefined, 0,0.001
	int8_t p_solar; // undefined, undefined, 0,1
	int8_t p_load; // undefined, undefined, 0,1
	uint8_t p_left; // undefined, undefined, 0,1
	uint8_t p_right; // undefined, undefined, 0,1
	uint16_t rotations; // undefined, undefined, 0,100000
	uint8_t rpm_left; // undefined, undefined, 0,25
	uint8_t rpm_right; // undefined, undefined, 0,25
	uint8_t rollPitchRange; // undefined, undefined, 0,0.2
	uint8_t swellPeriod; // undefined, undefined, 0,1
	uint8_t swellHeight; // undefined, undefined, 0,0.25
	uint8_t tempAir; // undefined, undefined, -20,0.25
	uint8_t tempWater; // undefined, undefined, 0,0.1666666667
	int8_t pressure; // undefined, undefined, 101300,100
	int8_t pH; // undefined, undefined, 7,0.02
	int8_t salinity; // undefined, undefined, 27.5,0.1
	uint16_t windSpeed; // undefined, undefined, 0,0.01
	int8_t windDirection; // undefined, undefined, 0,1.40625
	uint8_t status1; // 
	uint8_t status2; // 
	uint8_t currentWaypointIndex; // undefined, undefined, 0,1
	uint8_t currentWaypointChecksum; // undefined, undefined, 0,1
	uint16_t commandCount; // undefined, undefined, 0,1
	uint16_t telemetryCount; // undefined, undefined, 0,1
	uint8_t extra1; // undefined, undefined, 0,1
	uint16_t checksum; // 
};
struct cmdcontrol {
	uint8_t version; // 
	uint8_t format; // 
	uint8_t telemetryPeriod; // 0: No Change; 1: 5; 2: 10; 3: 20; 4: 30; 5: 45; 6: 60; 7: 90; 8: 120; 9: 180; 10: 360; 11: 720; 12: 1440; 13: 2880; 
	uint8_t forceMode; // 
	int8_t forceHeading; // undefined, undefined, 0,1.40625
	uint16_t goalVoltage; // undefined, undefined, 0,0.001
	uint8_t forceCurrentWaypointIndex; // 
	uint8_t waypointID1; // 
	uint8_t waypointRadius1; // undefined, undefined, 0,1
	float waypointLat1; // undefined, undefined, 0,1
	float waypointLon1; // undefined, undefined, 0,1
	uint8_t waypointID2; // 
	uint8_t waypointRadius2; // undefined, undefined, 0,1
	float waypointLat2; // undefined, undefined, 0,1
	float waypointLon2; // undefined, undefined, 0,1
	uint8_t waypointID3; // 
	uint8_t waypointRadius3; // undefined, undefined, 0,1
	float waypointLat3; // undefined, undefined, 0,1
	float waypointLon3; // undefined, undefined, 0,1
	uint8_t waypointID4; // 
	uint8_t waypointRadius4; // undefined, undefined, 0,1
	float waypointLat4; // undefined, undefined, 0,1
	float waypointLon4; // undefined, undefined, 0,1
	uint16_t checksum; // 
};
struct tlmimage {
	uint8_t version; // 
	uint8_t format; // 
	uint8_t num; // 
	uint8_t total; // 
	uint8_t data[294]; // 
	uint16_t checksum; // 
};
struct tlmdiagnostic {
	uint8_t version; // 
	uint8_t format; // 
	uint32_t gpsTime; // 
	float gpsLatitude; // 
	float gpsLongitude; // 
	float gpsAltitude; // 
	float gpsGroundSpeed; // 
	float gpsCourse; // 
	uint8_t wpIndex; // 
	uint8_t waypointRadius; // 
	float wpLatitude; // 
	float wpLongitude; // 
	float roll; // 
	float pitch; // 
	float heading; // 
	float distanceToWaypoint; // 
	float desiredCourse; // 
	float desiredPower; // 
	uint16_t leftThruster; // 
	uint16_t rightThruster; // 
	uint8_t isManual; // 
	uint8_t isOther; // 
	int16_t rcSteering; // 
	int16_t rcPower; // 
	float voltageAPM; // 
	float voltageSolar; // 
	float voltageBattery; // 
	float voltageThrusters; // 
	float powerAPM; // 
	float powerThrusters; // 
	float powerSolar; // 
	float powerCharge; // 
	float powerLoad; // 
	uint8_t cmdtelemetryPeriod; // 0: No Change; 1: 5; 2: 10; 3: 20; 4: 30; 5: 45; 6: 60; 7: 90; 8: 120; 9: 180; 10: 360; 11: 720; 12: 1440; 13: 2880; 
	uint8_t cmdforceMode; // 
	int8_t cmdforceHeading; // undefined, undefined, 0,1.40625
	uint16_t cmdgoalVoltage; // undefined, undefined, 0,0.001
	uint8_t cmdforceCurrentWaypointIndex; // 
	uint16_t checksum; // 
};

}

#endif
