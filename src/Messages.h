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
	uint8_t test_bitmap[40]; // 
	int8_t test_convert; // counts, V, 0,1
	uint16_t checksum; // 
};
struct tlmstatus {
	uint8_t version; // 
	uint8_t format; // 
	float latitude; // 
	float longitude; // 
	uint8_t fix; // 
	uint32_t time; // 
	int8_t altitude; // 
	int8_t course; // 180/128 degrees, degrees, 1.40625,0
	int8_t heading; // 180/128 degrees, degrees, 1.40625,0
	uint16_t v_load; // 
	uint16_t v_solar; // 
	int8_t p_solar; // 
	int8_t p_load; // 
	uint8_t p_left; // 
	uint8_t p_right; // 
	uint16_t rotations; // 
	uint8_t rpm_left; // 
	uint8_t rpm_right; // 
	uint8_t rollPitchRange; // 
	uint8_t swellPeriod; // 
	uint8_t swellHeight; // 
	uint8_t tempAir; // 
	uint8_t tempWater; // 
	int8_t pressure; // 
	int8_t pH; // 
	int8_t salinity; // 
	uint16_t windSpeed; // 
	int8_t windDirection; // 
	uint16_t status; // 
	uint8_t currentWaypointIndex; // 
	uint16_t commandCount; // 
	uint16_t telemetryCount; // 
	uint16_t checksum; // 
};
struct cmdcontrol {
	uint8_t version; // 
	uint8_t format; // 
	uint8_t telemetryPeriod; // 0: 1; 1: 5; 2: 10; 3: 20; 4: 30; 5: 45; 6: 60; 7: 90; 8: 120; 9: 180; 10: 360; 11: 720; 12: 1440; 13: 2880; 
	uint8_t forceMode; // 
	int8_t forceHeading; // 
	uint16_t forceGoalVoltage; // 
	uint8_t forceCurrentWaypointIndex; // 
	uint8_t waypointID1; // 
	uint8_t waypointRadius1; // 
	float waypointLat1; // 
	float waypointLon1; // 
	uint8_t waypointID2; // 
	uint8_t waypointRadius2; // 
	float waypointLat2; // 
	float waypointLon2; // 
	uint8_t waypointID3; // 
	uint8_t waypointRadius3; // 
	float waypointLat3; // 
	float waypointLon3; // 
	uint8_t waypointID4; // 
	uint8_t waypointRadius4; // 
	float waypointLat4; // 
	float waypointLon4; // 
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
	uint16_t checksum; // 
};

}

#endif
