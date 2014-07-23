k#ifndef AIRMAR100WX_H
#define AIRMAR100WX_H

#include <WProgram.h>

namespace Airmar100WX {
	extern float apparentWindSpeed;
	extern float apparentWindDirection;
	extern float windSpeed;
	extern float windDirection;
	extern float airTemperature;
	extern float pressure;

	void init(Stream *_stream);

	bool readRaw();

	void convertToAbsolute(float groundSpeed, float course, float heading);
}

#endif