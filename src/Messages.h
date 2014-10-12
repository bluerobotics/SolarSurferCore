#ifndef MESSAGES_H630903
#define MESSAGES_H630903

/* This file was automatically generated. DO NOT EDIT! */

#include <WProgram.h>

namespace MessageType {

struct teststring {
	uint8_t _version; // 
	uint8_t _format; // 
	char message[46]; // 
	uint16_t _checksum; // 
};
struct testmixed {
	uint8_t _version; // 
	uint8_t _format; // 
	uint8_t test_uint8_t; // 
	int8_t test_int8_t; // 
	uint8_t test_enum; // 0: this; 1: that; 
	uint8_t test_bitmap; // 
	int8_t test_convert; // undefined, undefined, 0,1
	uint16_t _checksum; // 
};
struct tlmshortStatus {
	uint8_t _version; // 
	uint8_t _format; // 
	uint32_t time; // undefined, undefined, 0,1
	uint8_t fix; // 0: No Fix; 1: Dead Reckoning Only; 2: 2D-Fix; 3: 3D-Fix; 4: GPS+Dead Reckoning; 5: Time Only; 
	float latitude; // undefined, undefined, 0,1
	float longitude; // undefined, undefined, 0,1
	int8_t altitude; // undefined, undefined, 0,2
	int8_t course; // undefined, undefined, 0,1.40625
	int8_t heading; // undefined, undefined, 0,1.40625
	uint8_t currentWaypointIndex; // undefined, undefined, 0,1
	float currentWaypointLatitude; // undefined, undefined, 0,1
	float currentWaypointLongitude; // undefined, undefined, 0,1
	uint8_t status1; // 
	uint8_t status2; // 
	uint16_t commandCount; // undefined, undefined, 0,1
	uint16_t telemetryCount; // undefined, undefined, 0,1
	uint16_t v_load; // undefined, undefined, 0,0.001
	uint8_t p_load; // undefined, undefined, 0,1
	uint8_t p_left; // undefined, undefined, 0,1
	uint8_t p_right; // undefined, undefined, 0,1
	uint8_t rpm_left; // undefined, undefined, 0,25
	uint8_t rpm_right; // undefined, undefined, 0,25
	uint8_t rollPitchRange; // undefined, undefined, 0,0.2
	uint8_t swellPeriod; // undefined, undefined, 0,1
	uint8_t swellHeight; // undefined, undefined, 0,0.25
	uint8_t tempWater; // undefined, undefined, 0,0.1666666667
	uint8_t extra[4]; // 
	uint16_t _checksum; // 
};
struct tlmlongStatus {
	uint8_t _version; // 
	uint8_t _format; // 
	uint32_t time; // undefined, undefined, 0,1
	uint8_t fix; // 0: No Fix; 1: Dead Reckoning Only; 2: 2D-Fix; 3: 3D-Fix; 4: GPS+Dead Reckoning; 5: Time Only; 
	float latitude; // undefined, undefined, 0,1
	float longitude; // undefined, undefined, 0,1
	int8_t altitude; // undefined, undefined, 0,2
	int8_t course; // undefined, undefined, 0,1.40625
	int8_t heading; // undefined, undefined, 0,1.40625
	uint8_t currentWaypointIndex; // undefined, undefined, 0,1
	float currentWaypointLatitude; // undefined, undefined, 0,1
	float currentWaypointLongitude; // undefined, undefined, 0,1
	uint8_t status1; // 
	uint8_t status2; // 
	uint16_t commandCount; // undefined, undefined, 0,1
	uint16_t telemetryCount; // undefined, undefined, 0,1
	uint16_t v_load; // undefined, undefined, 0,0.001
	uint8_t p_load; // undefined, undefined, 0,1
	uint8_t p_left; // undefined, undefined, 0,1
	uint8_t p_right; // undefined, undefined, 0,1
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
	uint8_t extra[47]; // 
	uint16_t _checksum; // 
};
struct cmdcontrol {
	uint8_t _version; // 
	uint8_t _format; // 
	uint8_t telemetryPeriod; // undefined, undefined, 0,10: No Change; 1: 2.5; 2: 5; 3: 10; 4: 20; 5: 30; 6: 45; 7: 60; 8: 90; 9: 120; 10: 180; 11: 360; 12: 720; 13: 1440; 14: 2880; 
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
	uint16_t _checksum; // 
};
struct tlmimage {
	uint8_t _version; // 
	uint8_t _format; // 
	uint8_t num; // 
	uint8_t total; // 
	uint8_t data[294]; // 
	uint16_t _checksum; // 
};
struct tlmdiagnostic {
	uint8_t _version; // 
	uint8_t _format; // 
	uint32_t gpsTime; // 
	float gpsLatitude; // 
	float gpsLongitude; // 
	float gpsAltitude; // 
	float gpsGroundSpeed; // 
	float gpsCourse; // 
	uint8_t wpIndex; // 
	float wpLatitude; // 
	float wpLongitude; // 
	uint8_t waypointRadius; // 
	float distanceToWaypoint; // 
	float headingToWaypoint; // 
	float heading; // 
	float desiredCourse; // 
	uint8_t prevIndex; // 
	float prevLatitude; // 
	float prevLongitude; // 
	float roll; // 
	float pitch; // 
	float desiredPower; // 
	uint16_t leftThruster; // 
	uint16_t rightThruster; // 
	uint8_t isManual; // 
	uint8_t isOther; // 
	int16_t rcSteering; // 
	int16_t rcPower; // 
	float voltageAPM; // 
	float powerAPM; // 
	float powerThrusters; // 
	int16_t rpmLeft; // 
	int16_t rpmRight; // 
	uint8_t cmdtelemetryPeriod; // 0: No Change; 1: 2.5; 2: 5; 3: 10; 4: 20; 5: 30; 6: 45; 7: 60; 8: 90; 9: 120; 10: 180; 11: 360; 12: 720; 13: 1440; 14: 2880; 
	uint8_t cmdforceMode; // 
	uint8_t forceThrustersOff; // 
	uint8_t forceHeading; // 
	uint8_t forceHoldPosition; // 
	uint8_t forceSeaweedRemoval; // 
	int8_t cmdforceHeading; // undefined, undefined, 0,1.40625
	uint16_t cmdgoalVoltage; // undefined, undefined, 0,0.001
	uint8_t cmdforceCurrentWaypointIndex; // 
	uint8_t inCallback; // 
	uint8_t satcomSignal; // 
	uint8_t satcomErrorCode; // 0: SUCCESS; 1: ALREADY AWAKE; 2: SERIAL FAILURE; 3: PROTOCOL ERROR; 4: CANCELLED; 5: NO MODEM DETECTED; 6: SBDIX FATAL ERROR; 7: SEND/RECEIVE TIMEOUT; 8: RX OVERFLOW; 9: REENTRANT; 10: IS ASLEEP; 11: NO SLEEP PIN; 
	uint16_t telemetryCount; // 
	uint16_t commandCount; // 
	uint16_t timeTillNextSatcom; // 
	uint8_t status1; // 
	uint8_t status2; // 
	float rollPitchRange; // 
	float swellPeriod; // 
	float swellHeight; // 
	float tempAir; // 
	float tempWater; // 
	float pressure; // 
	float pH; // 
	float salinity; // 
	float windSpeed; // 
	float windDirection; // 
	uint16_t _checksum; // 
};

}

#endif
