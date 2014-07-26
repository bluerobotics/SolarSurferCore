#include <TinyGPS++.h>
#include "Airmar100WX.h"

namespace {
	Stream *stream;
	TinyGPSPlus nmea;
	TinyGPSCustom _pressure(nmea,"WIMDA",1); // inHg
	TinyGPSCustom _airTemperature(nmea,"WIMDA",5); // C
	TinyGPSCustom _windDirection(nmea,"WIMWV",1); // deg
	TinyGPSCustom _windSpeed(nmea,"WIMWV",3); // knots
}

namespace Airmar100WX {
	float apparentWindSpeed;
	float apparentWindDirection;
	float windSpeed;
	float windDirection;
	float airTemperature;
	float pressure;

	void init(Stream *_stream) {
		stream = _stream;
	}

	bool readRaw() {
		uint8_t maxChars = 128;

		while(stream->available() > 0 && maxChars-- > 0 ) {
			nmea.encode(stream->read());
		}

		if (_pressure.isUpdated() || _windSpeed.isUpdated() ) {
			pressure = atof(_pressure.value())*3386; // to Pa
			airTemperature = atof(_airTemperature.value());
			apparentWindSpeed = atof(_windSpeed.value())*0.514444444; // to m/s
			apparentWindDirection = atof(_windDirection.value());
			if (apparentWindDirection > 180) {
				apparentWindDirection -= 360;
			} else if (apparentWindDirection < -180) {
				apparentWindDirection += 360;
			}
			apparentWindDirection = -apparentWindDirection;
			return true;
		} else {
			return false;
		}
	}

	void convertToAbsolute(float groundSpeed, float course, float heading) {
		// Correct from relative to absolute speed here
		/*float relX, relY, vesselX, vesselY, absX, absY;

		relX = apparentWindSpeed*cos(radians(apparentWindDirection));
		relY = apparentWindSpeed*sin(radians(apparentWindDirection));

		float angle = radians(heading-course);
		if (angle > PI)
			angle -= 2*PI;
		else if (angle < -PI)
			angle += 2*PI;

		vesselX = groundSpeed*cos(angle);
		vesselY = groundSpeed*sin(angle);

		absX = relX+vesselX;
		absY = relY+vesselY;

		windSpeed = sqrt(absX*absX+absY*absY);
		windDirection = degrees(atan2(absX,absY));*/

		float a = apparentWindSpeed*sin(radians(apparentWindDirection+heading-course));
		float b = apparentWindSpeed*cos(radians(apparentWindDirection+heading-course))+groundSpeed;

		windSpeed = sqrt(a*a+b*b);
		windDirection = degrees(atan2(a,b))+course;

		if (windDirection > 180)
			windDirection -= 360;
		else if (windDirection <= -180)
			windDirection += 360;		

		/*windSpeed = sqrt(apparentWindSpeed^2+groundSpeed^2-2*apparentWindSpeed*groundSpeed*cos(radians(apparentWindDirection)));

		windDirection = acos((apparentWindSpeed*cos(radians(apparentWindDirection))-groundSpeed)/windSpeed);*/
	}
}