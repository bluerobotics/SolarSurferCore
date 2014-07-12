#include "RemoteControl.h"
#include "APM.h"

namespace {
	static const int16_t center = 1500;

	int16_t channel[4];
}

namespace RemoteControl {
	void init() {
		update();
	}

	void update() {
		if ( APM::getRadioInputState() ) {
				for (uint8_t i = 0 ; i < 4 ; i++ ) {
				channel[i] = APM::inputCh(i);
			}
		}
	}

	float getSteering() {
		return channel[0]-center;
	}

	float getPower() {
		return channel[1]-center;
	}

	bool isManual() {
		return channel[2] > center;
	}
}