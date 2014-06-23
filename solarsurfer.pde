#include <WProgram.h>
#include "MPU6000.h"
#include "HMC5883.h"
#include "DCM.h"

DCM filter;

float dt;
long timer;
long outputTimer;
long diagnosticTimer;

void setup() {
  Serial.begin(115200);
  Serial.println("start");

  // Set barometer CS pin high so it doesn't hog the bus. How frustrating.  
  pinMode(40,OUTPUT);
  digitalWrite(40,HIGH);
  
  MPU6000::init();
  HMC5883::init();
  filter.init();
}

void loop() {
  if ( MPU6000::newdata ) {
    diagnosticTimer = micros();
    
    dt = (micros()-timer)/1000000.0f;
    timer = micros();
    
    MPU6000::read();
    HMC5883::read();
    HMC5883::calculate(filter.roll,filter.pitch);
    filter.updateMeasurements(MPU6000::gyroY,
			      MPU6000::gyroX,
			      -MPU6000::gyroZ,
			      -MPU6000::accelY,
			      -MPU6000::accelX,
			      MPU6000::accelZ,
			      dt);
    filter.normalize();
    filter.driftCorrection(HMC5883::heading);
    filter.accelerationCorrection();
    filter.convertDCMtoEuler();
    
    /** Loop timer. According to experimentation, the loop takes 4900 us 
     * or 4.9 ms or 0.0049 s. A flight loop should run at 100 Hz so this
     * would leave 5.1 ms for other stuff if run on the flight computer.
     */
    if (false) {
      Serial.print("Time: ");Serial.println(micros()-diagnosticTimer);
    }
    
    if (false) {
		Serial.println(HMC5883::heading);
	  }
  
	  if (true) {
		Serial.print(filter.roll*180/3.14159);Serial.print(" ");
		Serial.print(filter.pitch*180/3.14159);Serial.print(" ");
		Serial.print(filter.yaw*180/3.14159);Serial.print(" ");
		Serial.print(HMC5883::heading*180/3.14159);Serial.println(" ");
	  }
  
	  if (false) {
		Serial.print(-MPU6000::accelY);Serial.print(" ");
		Serial.print(-MPU6000::accelX);Serial.print(" ");
		Serial.print(-MPU6000::accelZ);Serial.print(" ");
		Serial.print(MPU6000::gyroY);Serial.print(" ");
		Serial.print(MPU6000::gyroX);Serial.print(" ");
		Serial.print(-MPU6000::gyroZ);Serial.println(" ");
	  }
  }
}
