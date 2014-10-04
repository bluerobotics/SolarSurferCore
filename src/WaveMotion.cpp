#include "WaveMotion.h"
#include "MPU6000.h"
#include "DCM.h"
#include "GPS_UBX.h"

namespace {
	float lowPassZAccel;

	uint32_t lastCalculate;
	uint32_t lastSwellPeriod;
	uint32_t lastZeroCrossing;
	bool positiveAccel;
	uint16_t zeroCrossingCount;

	float maxGPSHeight;
	float minGPSHeight;
	uint32_t lastSwellHeight;

	float maxRollPitch;
	uint32_t lastRollPitch;
}

namespace WaveMotion {
	float rollPitchRange;
	float swellHeight;
	float swellPeriod;

	void init() {
		maxGPSHeight = -10000.0f;
		minGPSHeight = 10000.0f;
	}

	/** This function calculates the rollPitchRange, swellHeight, and swellPeriod.
	 * 
	 * Swell period is calculated with a zero-crossing counter of a low pass filter
	 * with a large time constant. A delay prevents counting "bounces".
	 *
	 * Swell height is calculated with min and max GPS altitudes. It tracks the min and
	 * max values and resets every few minutes. This cannot be
	 * very accurate but it's worth a shot. The other option is using pressure altitude,
	 * which is not available since the barometer is sealed in the box, or integrating
	 * acceleration, which is also not very accurate.
	 *
	 * Roll/Pitch range is calculated by finding the maximum angle (combined roll/pitch)
	 * and recording that value with a timeout reset. */
	void calculate() {
		float dt = (millis()-lastCalculate)/1000.0f;
		lastCalculate = millis();

		// First, calculate the zero crossings of z-accel
		static const float accelTau = 6; // seconds
		float alpha = dt/(accelTau + dt);

		lowPassZAccel = lowPassZAccel*(1-alpha) + alpha*MPU6000::accelZ;

		if ( millis() - lastZeroCrossing > 2000 ) {
			if ( positiveAccel && lowPassZAccel < 0 ) {
				positiveAccel = false;
				zeroCrossingCount++;
				lastZeroCrossing = millis();
			} else if ( !positiveAccel && lowPassZAccel > 0 ) {
				positiveAccel = true;
				zeroCrossingCount++;
				lastZeroCrossing = millis();
			}
		}

		if ( zeroCrossingCount >= 10 ) {
			swellPeriod = (millis()-lastSwellPeriod)/1000.0f/zeroCrossingCount*2;
			lastSwellPeriod = millis();
		}

		// Calculate swell height
		if ( GPS_UBX::altitude > maxGPSHeight ) {
			maxGPSHeight = GPS_UBX::altitude;
		} else if ( GPS_UBX::altitude < minGPSHeight ) {
			minGPSHeight = GPS_UBX::altitude;
		}

		static const float heightAlpha = 0.375; // 10 minute tau
		if ( millis() - lastSwellHeight > 360000 ) {
			lastSwellHeight = millis();
			swellHeight = swellHeight*(1-heightAlpha) + (maxGPSHeight-minGPSHeight)*heightAlpha;

			// Reset heights
			maxGPSHeight = -10000;
			minGPSHeight = 10000;
		}

		// Calculate max roll/pitch
		float tilt = degrees(acos(cos(DCM::roll)*cos(DCM::pitch)));

		if ( tilt > maxRollPitch ) {
			maxRollPitch = tilt;
		}

		static const float rollPitchAlpha = 0.71; // About 5 minutes
		if ( millis() - lastRollPitch > 120000 ) {
			lastRollPitch = millis();
			rollPitchRange = rollPitchRange*(1-rollPitchAlpha) + maxRollPitch*rollPitchAlpha;
			// Reset angle
			maxRollPitch = 0;
		}
	}
}