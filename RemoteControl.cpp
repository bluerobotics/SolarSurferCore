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
		static const int16_t steeringMagnitude = 200;
		return map(channel[0],1100,1900,-steeringMagnitude,steeringMagnitude);
	}

	float getPower() {
		static const int16_t powerMagnitude = 400;
		return map(channel[1],1100,1900,-powerMagnitude,powerMagnitude);
	}

	bool isManual() {
		return channel[2] > center;
	}

	bool isOther() {
		return channel[3] > center;
	}
}