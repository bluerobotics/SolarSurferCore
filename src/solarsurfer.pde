#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <NewSoftSerial.h>
#include "MPU6000.h"
#include "HMC5883.h"
#include "DCM.h"
#include "GPS_UBX.h"
#include "IridiumSBD.h"
#include "APM.h"
#include "Navigator.h"
#include "Helmsman.h"
#include "Thruster.h"
#include "RemoteControl.h"
#include "Captain.h"
#include "MessageManager.h"
#include "Messages.h"
#include "Transfer.h"
#include "PowerMonitor.h"
#include "BLDCMonitor.h"
#include "Airmar100WX.h"
#include "WaypointWriter.h"

float dt;
long timer;
long outputTimer;
long diagnosticTimer;

IridiumSBD isbd(Serial2, 12);
Transfer telemTransfer;

NewSoftSerial nssBLDC(150,151);
NewSoftSerial nssPM(152,153);
NewSoftSerial nssAirmar(154,155);
NewSoftSerial nssPH(156,157);
NewSoftSerial nssCamera(158,159);

BLDCMonitor bldcMonitor(&nssBLDC);
PowerMonitor powerMonitor(&nssPM);

void setup() {
  Serial.begin(57600);
  Serial2.begin(19200); // Satcom
  Serial.println("start");

  // Set barometer CS pin high so it doesn't hog the bus. How frustrating.  
  pinMode(40,OUTPUT);
  digitalWrite(40,HIGH);
  
  APM::init();
  MPU6000::init();
  HMC5883::init();
  DCM::init();
  GPS_UBX::init();
  Airmar100WX::init(&nssAirmar);
  Thruster::init();
  RemoteControl::init();
  Captain::init(&bldcMonitor,&powerMonitor);
  MessageManager::init(&bldcMonitor,&powerMonitor);

  isbd.attachConsole(Serial);
  isbd.attachDiags(Serial);
  isbd.setPowerProfile(1); // 1 == low power

  telemTransfer.setStream(&Serial);
  
  if (false) {
	  HMC5883::calibrateOffsets();
	}
	HMC5883::set_offset(108, 6, 96);

	if (false) {
		WaypointWriter::write();
	}
}

void updateNavigationSensors() {
	// Update GPS
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
	
	// Update IMU + compass
	if ( MPU6000::newdata ) {
    
    dt = (micros()-timer)/1000000.0f;
    timer = micros();
    
    MPU6000::read();
    HMC5883::read();
    HMC5883::calculate(DCM::roll,DCM::pitch);
    HMC5883::applyDeclination(12.4);
    DCM::updateMeasurements(MPU6000::gyroY,
			      MPU6000::gyroX,
			      -MPU6000::gyroZ,
			      -MPU6000::accelY,
			      -MPU6000::accelX,
			      MPU6000::accelZ,
			      dt);
    DCM::normalize();
    DCM::driftCorrection(HMC5883::heading);
    DCM::accelerationCorrection();
    DCM::convertDCMtoEuler();
    
    if (false) {
			Serial.println(HMC5883::heading);
	  }
  
	  if (false) {
			Serial.print(degrees(DCM::roll));Serial.print(" ");
			Serial.print(degrees(DCM::pitch));Serial.print(" ");
			Serial.print(degrees(DCM::yaw));Serial.print(" ");
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

  // Update BLDC monitor
  static const uint32_t BLDCReadPeriodMS = 250;
  static uint32_t BLDCReadTimer;
  if ( millis() - BLDCReadTimer > BLDCReadPeriodMS ) {
  	BLDCReadTimer = millis();
  	// Listen on software serial here
  	nssBLDC.listen();
  	bldcMonitor.read();
  }

  // Update Power monitor
  static const uint32_t PowerReadPeriodMS = 250;
  static uint32_t PowerReadTimer;
  if ( millis() - PowerReadTimer > PowerReadPeriodMS ) {
  	PowerReadTimer = millis();
  	// Listen on software serial here
  	nssPM.listen();
  	powerMonitor.read();
  }

  // Update Airmar 100WX sensor
  static const uint32_t AirmarReadPeriodMS = 3000;
  static uint32_t AirmarReadTimer;
  if ( millis() - AirmarReadTimer > AirmarReadPeriodMS ) {
    AirmarReadTimer = millis();
  	
    nssAirmar.listen();
    if (Airmar100WX::readRaw()) {
      Airmar100WX::convertToAbsolute(GPS_UBX::groundSpeed,GPS_UBX::course,DCM::yaw);
    }
  }

  // Update temperature sensor
  static const uint32_t WTempReadPeriodMS = 3000;
  static uint32_t WTempReadTimer;
  if ( millis() - WTempReadTimer > WTempReadPeriodMS ) {
  	WTempReadTimer = millis();
  	// Insert code here.
  }

  // Update pH sensor
  static const uint32_t pHReadPeriodMS = 5000;
  static uint32_t pHReadTimer;
  if ( millis() - pHReadTimer > pHReadPeriodMS ) {
  	pHReadTimer = millis();
  	// Insert code here.
  }
}

void loop() {
	static const long printPeriod			=			500;
	static const long navPeriod 			=		 	100;
	static const long satcomPeriod 		=     60000*5; // 5 minutes
	static const long telemPeriod     =     250;

	static long printTimer;
	static long navTimer;
	static long satcomTimer;
	static long telemTimer;

	if (millis()-navTimer>navPeriod) {
		navTimer = millis();

  	updateNavigationSensors();
  	RemoteControl::update();
  
    Captain::determineState();
    Captain::determineCourseAndPower();
    Captain::execute();
  }

  if (millis()-satcomTimer>satcomPeriod) {
  	satcomTimer = millis();

  	MessageManager::updateFields();
		MessageManager::serialize(&Msg::tlmstatus);
		uint16_t length = MessageManager::getTXBufferLength();
		const uint8_t *data = MessageManager::getTXBuffer();  	

		// Call satcom function here.
  }

  if (millis()-telemTimer>telemPeriod) {
  	telemTimer = millis();

		MessageManager::updateFields();
		/*MessageManager::serialize(&Msg::tlmdiagnostic);
		uint16_t length = MessageManager::getTXBufferLength();
		const uint8_t *data = MessageManager::getTXBuffer();*/

		telemTransfer.send(&Msg::tlmdiagnostic);
  }	
  
  if (false && millis()-printTimer > printPeriod) {
  	printTimer = millis();
		Serial.write(27);       // ESC command
		Serial.print("[2J");    // clear screen command
		Serial.write(27);
		Serial.print("[H");     // cursor to home command
		Serial.println("SolarSurfer Live Data");
		Serial.println("=====================");
		Serial.print("Time: ");Serial.print(GPS_UBX::time,DEC);Serial.println(" ms");
		Serial.print("Lat: ");Serial.print(GPS_UBX::latitude,6);Serial.println(" deg");
		Serial.print("Lon: ");Serial.print(GPS_UBX::longitude,6);Serial.println(" deg");		
		Serial.print("Alt: ");Serial.print(GPS_UBX::altitude,2);Serial.println(" m");
		Serial.print("Speed: ");Serial.print(GPS_UBX::groundSpeed,2);Serial.println(" m/s");
		Serial.print("Current Course: ");Serial.print(degrees(GPS_UBX::course));Serial.println(" deg");
		Serial.println("");
		Serial.print("WP Index: ");Serial.println(Captain::waypoint.index,DEC);
		Serial.print("WP Lat: ");Serial.print(Captain::waypoint.location.latitude,6);Serial.println(" deg");
		Serial.print("WP Lon: ");Serial.print(Captain::waypoint.location.longitude,6);Serial.println(" deg");		
		Serial.println("");
		Serial.print("Roll: ");Serial.print(degrees(DCM::roll));Serial.println(" deg ");
		Serial.print("Pitch: ");Serial.print(degrees(DCM::pitch));Serial.println(" deg");				
		Serial.println("");
  	Serial.print("Desired Course:  ");Serial.print(degrees(Captain::desiredCourse));Serial.println(" deg");
  	Serial.print("Current Heading: ");Serial.print(degrees(DCM::yaw));Serial.println(" deg");
  	Serial.print("Distance to WP:  ");Serial.print(Captain::distanceToWaypoint,1);Serial.println(" m");
  	Serial.println("");
  	Serial.print("Left Thruster:  ");Serial.print(Thruster::get(Thruster::left));Serial.println(" us");
  	Serial.print("Right Thruster: ");Serial.print(Thruster::get(Thruster::right));Serial.println(" us");
  	Serial.println("");
  	if ( RemoteControl::isManual() ) {
  		Serial.println("Manual Control Enabled");
  	} else {
  		Serial.println("AUTONOMOUS CONTROL ENABLED");
  	}
  	if ( RemoteControl::isOther() ) {
  		Serial.println("'Other' Switch Disabled");
  	} else {
  		Serial.println("'Other' Switch Enabled");
  	}
  	Serial.print("RC Steering:  ");Serial.print(RemoteControl::getSteering());Serial.println("");
  	Serial.print("RC Power:     ");Serial.print(RemoteControl::getPower());Serial.println("");
  	Serial.println("");
  	Serial.print("Packet Length: ");Serial.println(MessageManager::getTXBufferLength());
		Serial.println("Packet Data:");	
		for ( uint8_t i = 0 ; i < MessageManager::getTXBufferLength() ; i++ ) {
			Serial.print((MessageManager::getTXBuffer())[i],HEX);Serial.print("\t");
			if ( (i+1) % 8 == 0 ) {
				Serial.println("");
			}
		}
  }
}
