#include "Airmar100WX.h"

namespace {
	Stream *stream;
}

namespace Airmar100WX {
	float windSpeed;
	float windDirection;
	float airTemperature;
	float pressure;

	void init(Stream *_stream) {
		stream = _stream;
	}

	void readRaw() {
		// Read Airmar sensor here
	}

	void convertToAbsolute(float groundSpeed, float course, float heading) {
		// Correct from relative to absolute speed here
		float relX, relY, vesselX, vesselY, absX, absY;

		relX = windSpeed*cos(radians(windDirection));
		relY = windSpeed*sin(radians(windDirection));

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
		windDirection = atan2(absY,absX);
	}
}