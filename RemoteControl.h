#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <WProgram.h>

namespace RemoteControl {
	void init();

	void update();

	float getSteering();

	float getPower();

	bool isManual();
}

#endif
