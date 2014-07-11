#ifndef THRUSTER_H
#define THRUSTER_H

#include <WProgram.h>

namespace Thruster {
	static const uint8_t left = 0;
	static const uint8_t right = 1;

	void init();
	
	void set(uint8_t thruster, int16_t setting);
	
	int16_t get(uint8_t thruster);
}

#endif