#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include "Arduino.h"

namespace RemoteControl {
	void init();

	void update();

	float getSteering();

	float getPower();

	bool isManual();

	bool isOther();
}

#endif
