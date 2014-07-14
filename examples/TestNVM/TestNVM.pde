/* This file tests the non-volatile memory for the SolarSurfer. It is currently written
* to run on an Atmega328p for testing purposes. */

#include <WProgram.h>
#include "../../Persistant.h"
#include "../../WaypointList.h"

Waypoint w[5];

void setup() {
	Serial.begin(115200);
}

void loop() {
	Serial.println("p - print, r - read, w - write");
	while (Serial.available() == 0) {}
	
	uint8_t input;
	input = Serial.read();
	
	switch (input) {
		case 'r':
			Persistant::read();
			for (uint8_t i = 0 ; i < 5 ; i++ ) {
				WaypointList::read(&w[i],i);
			}
			break;
		case 'w':
			Persistant::data.currentWaypointIndex = 4;
			Persistant::data.lastWaypointIndex = 3;
			Persistant::write();
			for (uint8_t i = 0 ; i < 5 ; i++ ) {
				Location loc;
				loc.latitude = 33.4+i;
				loc.longitude = -118.4-i;
				WaypointList::write(i,loc);
			}
			break;
		case 'p':
			Serial.println("============================");
			Serial.println("Data:");
			Serial.print("Current Waypoint Index: ");Serial.println(Persistant::data.currentWaypointIndex,DEC);
			Serial.print("Last Waypoint Index: ");Serial.println(Persistant::data.lastWaypointIndex,DEC);
			for (uint8_t i = 0 ; i < 5 ; i++ ) {
				Serial.print("Waypoint Index: ");Serial.println(w[i].index,DEC);
				Serial.print("Waypoint Lat: ");Serial.println(w[i].location.latitude,6);
				Serial.print("Waypoint Lon: ");Serial.println(w[i].location.longitude,6);
			}
			Serial.println("============================");
			break;
		default:
			Serial.println("Invalid choice.");
	}
}
