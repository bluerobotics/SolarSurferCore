#ifndef MESSAGES_H630903
#define MESSAGES_H630903

/* This file was automatically generated. DO NOT EDIT! */

#include "Arduino.h"

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
	int16_t altitude; // 
	int16_t course; // centidegrees, degrees, 0,100
	int16_t heading; // centidegrees, degrees, 0,100
	int8_t p_solar; // 
	int8_t p_batt; // 
	uint8_t p_left; // 
	uint8_t p_right; // 
	uint16_t rotations; // 
	uint8_t rpm_left; // 
	uint8_t rpm_right; // 
	uint16_t swellHeight; // 
	uint16_t commandCount; // 
	uint8_t tempAir; // 
	uint8_t tempWater; // 
	uint16_t pressure; // 
	uint16_t pH; // 
	uint16_t telemetryCount; // 
	uint16_t checksum; // 
};
struct cmdcontrol {
	uint8_t version; // 
	uint8_t format; // 
	uint8_t telemetryPeriod; // 0: 1; 1: 5; 2: 10; 3: 20; 4: 30; 5: 45; 6: 60; 7: 90; 8: 120; 9: 180; 10: 360; 11: 720; 12: 1440; 13: 2880; 
	uint8_t forceMode; // 
	int16_t forceHeading; // 
	uint16_t forceCurrentWaypointIndex; // 
	uint16_t waypointID1; // 
	float waypointLat1; // 
	float waypointLon1; // 
	uint16_t waypointID2; // 
	float waypointLat2; // 
	float waypointLon2; // 
	uint16_t waypointID3; // 
	float waypointLat3; // 
	float waypointLon3; // 
	uint16_t waypointID4; // 
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
	uint16_t wpIndex; // 
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
