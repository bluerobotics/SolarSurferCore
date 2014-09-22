#include "Navigator.h"

namespace Navigator {
	float getHeadingToLocation(Location *current, Location *waypoint) {
		float lat1 = radians(current->latitude);
		float lon1 = radians(current->longitude);
		float lat2 = radians(waypoint->latitude);
		float lon2 = radians(waypoint->longitude);
		float y = sin(lon2-lon1)*cos(lat2);
		float x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(lon2-lon1);
		return atan2(y,x);
	}
	
	float getDistanceToLocation(Location *current, Location *waypoint) {
		static const float radiusEarth = 6371000.0f; // meters
		
		float lat1 = radians(current->latitude);
		float lon1 = radians(current->longitude);
		float lat2 = radians(waypoint->latitude);
		float lon2 = radians(waypoint->longitude);
		
		float a,c;
		
		a = sin((lat2-lat1)/2)*sin((lat2-lat1)/2)+cos(lat1)*cos(lat2)*sin((lon2-lon1)/2)*sin((lon2-lon1)/2);
		c = 2*atan2(sqrt(a),sqrt(1-a));
		return radiusEarth*c;
	}

	float getAngleBetweenHeadings(float a1, float a2, bool isRad) {
		float angle = a1 - a2;

		// Unwrap angle to -180 to 180, -pi to pi
		if ( isRad ) {
			if ( angle > PI ) {
				angle -= 2*PI;
			} else if ( angle < -PI ) {
				angle += 2*PI;
			}
		} else {
			if ( angle > 180 ) {
				angle -= 360;
			} else if ( angle < -180 ) {
				angle += 360;
			}
		}

		return angle;
	}
}