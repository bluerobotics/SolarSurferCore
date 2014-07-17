#include "Helmsman.h"
#include "Thruster.h"

namespace {
	float desiredHeading;
	float desiredPower;
	float steeringErrorIntegral;
	float powerErrorIntegral;
	
	float steeringController(float error, float dt) {
		static const float Kp = 2.0;
		static const float Ki = 0.6;
		static const float iMax = 75;
		
		steeringErrorIntegral = constrain(steeringErrorIntegral+error*dt,-iMax,iMax);
		
		return Kp*error + Ki*steeringErrorIntegral;
	}
	
	float powerController(float error, float dt) {
		static const float Kp = 4.0;
		static const float Ki = 0.0;
		static const float iMax = 1000;
		
		powerErrorIntegral = constrain(powerErrorIntegral+error*dt,-iMax,iMax);		
		
		return Kp*error + Ki*powerErrorIntegral;
	}	
}

namespace Helmsman {
	void setHeading(float heading) {
		desiredHeading = heading;
	}
	
	void setPower(float power) {
		desiredPower = power;
	}
	
	void execute(float heading, float power) {
		const static int16_t stoppedThrottle = 1500;
		const static float maxSteering = 300;
		const static float maxPower = 400;
		
		static long timer;
		float dt = (millis() - timer)/1000.0f;
		timer = millis();
		
		float steeringError = desiredHeading-heading;
		if (steeringError > 180) {
  		steeringError -= 360;
  	} else if ( steeringError < -180 ) {
  		steeringError += 360;
  	}
		
		float steeringOutput = steeringController(steeringError,dt);
		float powerOutput = powerController(desiredPower-power,dt);
		
		steeringOutput = constrain(steeringOutput,-maxSteering,maxSteering);
		powerOutput = constrain(powerOutput,-maxPower,maxPower);
		
		Thruster::set(Thruster::left,stoppedThrottle+powerOutput+steeringOutput);
		Thruster::set(Thruster::right,stoppedThrottle+powerOutput-steeringOutput);
	}

	void executeManual(float steering, float power) {
		const static int16_t stoppedThrottle = 1500;

		resetIntegrals(); // Reset the I terms so that they do not spool up during manual control
		
		Thruster::set(Thruster::left,stoppedThrottle+power+steering);
		Thruster::set(Thruster::right,stoppedThrottle+power-steering);	
	}

	void resetIntegrals() {
		steeringErrorIntegral = 0;
		powerErrorIntegral = 0;
	}
}