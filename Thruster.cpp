#include "Thruster.h"
#include "APM.h"

namespace Thruster {
	void init() {
	
	}
	
	void set(uint8_t thruster, int16_t setting) {
		switch (thruster) {
			case Thruster::left:
				APM::outputPWM(0,setting);
				break;
			case Thruster::right:
				APM::outputPWM(1,setting);
				break;
		}
	}
}