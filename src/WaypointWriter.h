#ifndef WAYPOINTWRITER_H
#define WAYPOINTWRITER_H

#include <WProgram.h>
#include "WaypointList.h"

namespace {
	static const uint8_t radius[] = {6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6,
																	6};
	static const Location loc[] = {
{33.96830,-118.50756},
{33.96830,-118.50756},
{33.96829,-118.51517},
{33.97491,-118.51535},
{33.97495,-118.50758},
{33.97213,-118.51125},
{33.97215,-118.53658},
{33.96391,-118.60538},
{33.93778,-118.63647},
{33.96055,-118.60094},
{33.93459,-118.63204},
{33.95715,-118.59668},
{33.93179,-118.62769},
{33.95376,-118.59261},
{33.92915,-118.62353},
{33.95059,-118.58870},
{33.96395,-118.51127}};
}

namespace WaypointWriter {
	
	void write() {
		uint16_t numWaypoints = sizeof(loc)/sizeof(Location);

		Serial.println("Writing Waypoints...");
		Serial.print("Number of Waypoints: ");Serial.println(numWaypoints,DEC);
		for (uint16_t i = 0 ; i < WaypointList::maxWaypoints ; i++) {
			Serial.print("Writing waypoint ");Serial.println(i,DEC);
			if (i < numWaypoints) {
				WaypointList::write(i,radius[i],loc[i]);
			} else {
				Location blank = {0.0f,0.0f};
				WaypointList::write(i,0,blank);
			}
			
		}

		Serial.println("Confirming Waypoints:");
		for (uint16_t i = 0 ; i < WaypointList::maxWaypoints ; i++) {
			Waypoint wp;
			WaypointList::read(&wp,i);
			Serial.print(wp.index,DEC);Serial.print(" ");
			Serial.print(wp.radius,DEC);Serial.print(" ");
			Serial.print(wp.location.latitude,6);Serial.print(" ");
			Serial.print(wp.location.longitude,6);Serial.println("");
			delay(20);
		}

		Serial.println("Done. Must reprogram.");
		for(;;);
	}
}

#endif
