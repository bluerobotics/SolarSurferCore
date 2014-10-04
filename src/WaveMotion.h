#ifndef WAVEMOTION_H
#define WAVEMOTION_H

#include <WProgram.h>

namespace WaveMotion {
	extern float rollPitchRange;
	extern float swellHeight;
	extern float swellPeriod;

	void init();

	void calculate();
}

#endif