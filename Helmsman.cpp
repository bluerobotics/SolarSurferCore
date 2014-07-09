#include "Helmsman.h"
#include "Thruster.h"

namespace {
	float desiredHeading;
	float desiredPower;
	float steeringErrorIntegral;
	float powerErrorIntegral;
	
	float steeringController(float error, float dt) {
		static const float Kp = 1.0;
		static const float Ki = 0.1;
		static const float iMax = 100;
		
		steeringErrorIntegral = constrain(steeringErrorIntegral+error*dt,-iMax,iMax);
		
		return Kp*error + Ki*steeringErrorIntegral;
	}
	
	float powerController(float error, float dt) {
		static const float Kp = 1.0;
		static const float Ki = 0.1;
		static const float iMax = 100;
		
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
		static long timer;
		float dt = (millis() - timer)/1000.0f;
		timer = millis();
		
		float steeringOutput = steeringController(desiredHeading-heading,dt);
		float powerOutput = powerController(desiredPower-power,dt);
		
		Thruster::set(Thruster::left,powerOutput+steeringOutput);
		Thruster::set(Thruster::right,powerOutput-steeringOutput);
	}
}