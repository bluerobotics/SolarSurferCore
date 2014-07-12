#include <WProgram.h>
#include "MPU6000.h"
#include "HMC5883.h"
#include "DCM.h"
#include "GPS_UBX.h"
#include "APM.h"
#include "Navigator.h"
#include "Helmsman.h"
#include "Thruster.h"
#include "RemoteControl.h"

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
  
  APM::init();
  MPU6000::init();
  HMC5883::init();
  filter.init();
  GPS_UBX::init();
  Thruster::init();
  RemoteControl::init();
  
  if (false) {
	  HMC5883::calibrateOffsets();
	}
	HMC5883::set_offset(101, 20, 101);
}

void updateNavigationSensors() {
	static uint32_t gpsTimer;
	if ( millis() - gpsTimer > 25 ) {
		for ( uint8_t i = 0 ; i < 255 ; i++ )
			GPS_UBX::read();
		gpsTimer = millis();
	
		if (false) {
			Serial.print(GPS_UBX::time,DEC);Serial.print(" ");
			Serial.print(GPS_UBX::fix,DEC);Serial.print(" ");
			Serial.print(GPS_UBX::longitude,8);Serial.print(" ");
			Serial.print(GPS_UBX::latitude,8);Serial.print(" ");
			Serial.print(GPS_UBX::groundSpeed,1);Serial.print(" ");
			Serial.print(GPS_UBX::course,2);Serial.println(" ");
		}
	}
	
	if ( MPU6000::newdata ) {
    
    dt = (micros()-timer)/1000000.0f;
    timer = micros();
    
    MPU6000::read();
    HMC5883::read();
    HMC5883::calculate(filter.roll,filter.pitch);
    HMC5883::applyDeclination(12.4);
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
    
    if (false) {
			Serial.println(HMC5883::heading);
	  }
  
	  if (false) {
			Serial.print(degrees(filter.roll));Serial.print(" ");
			Serial.print(degrees(filter.pitch));Serial.print(" ");
			Serial.print(degrees(filter.yaw));Serial.print(" ");
			Serial.print(degrees(HMC5883::heading));Serial.println(" ");
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

void loop() {
  updateNavigationSensors();
  RemoteControl::update();
  
  Location current;
  current.latitude = GPS_UBX::latitude;
  current.longitude = GPS_UBX::longitude;
  
  Location wp;
  wp.latitude = 21.3114; // Hawaii
  wp.longitude = -157.7964; // Hawaii
  
  wp.latitude = 33.870696; // The house across from me
  wp.longitude = -118.368667; // The house across from me
  
  float desiredHeading = Navigator::getHeadingToLocation(&current,&wp);

  Helmsman::setHeading(degrees(desiredHeading));
  Helmsman::setPower(60);

  if ( RemoteControl::isManual() ) {
  	Helmsman::executeManual(RemoteControl::getSteering(),RemoteControl::getPower());
  } else {
  	Helmsman::execute(degrees(filter.yaw),60.0);
  }
  
  float distance = Navigator::getDistanceToLocation(&current,&wp);
  
  static long printTimer;
  
  if (true && millis()-printTimer > 250) {
  	printTimer = millis();
		Serial.write(27);       // ESC command
		Serial.print("[2J");    // clear screen command
		Serial.write(27);
		Serial.print("[H");     // cursor to home command
		Serial.println("SolarSurfer Live Data");
		Serial.println("=====================");
		Serial.print("Lat: ");Serial.print(GPS_UBX::latitude,6);Serial.println(" deg");
		Serial.print("Lon: ");Serial.print(GPS_UBX::longitude,6);Serial.println(" deg");		
		Serial.println("");
		Serial.print("Roll: ");Serial.print(degrees(filter.roll));Serial.println(" deg ");
		Serial.print("Pitch: ");Serial.print(degrees(filter.pitch));Serial.println(" deg");				
		Serial.println("");
  	Serial.print("Desired Heading: ");Serial.print(degrees(desiredHeading));Serial.println(" deg");
  	Serial.print("Current Heading: ");Serial.print(degrees(filter.yaw));Serial.println(" deg");
  	Serial.print("Distance to WP:  ");Serial.print(distance,1);Serial.println(" m");
  	Serial.println("");
  	Serial.print("Left Thruster:  ");Serial.print(Thruster::get(Thruster::left));Serial.println(" us");
  	Serial.print("Right Thruster: ");Serial.print(Thruster::get(Thruster::right));Serial.println(" us");
  	Serial.println("");
  	if ( RemoteControl::isManual() ) {
  		Serial.println("Manual Control Enabled");
  	} else {
  		Serial.println("AUTONOMOUS CONTROL ENABLED");
  	}
  	Serial.print("RC Steering:  ");Serial.print(RemoteControl::getSteering());Serial.println("");
  	Serial.print("RC Power:     ");Serial.print(RemoteControl::getPower());Serial.println("");
  }
}
