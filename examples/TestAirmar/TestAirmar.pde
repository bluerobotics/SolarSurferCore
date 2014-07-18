/* This file tests the data tranfer between for the SolarSurfer and the BLDC monitor 
* and the Power Monitor. It is currently written to run on an Atmega328p for testing 
* purposes. */

#include <WProgram.h>
#include <NewSoftSerial.h>
#include "../../Airmar100WX.h"

NewSoftSerial nss(2,3);

void setup() {
	Serial.begin(115200);
	nss.begin(38400);
	
	Airmar100WX::init(&nss);
	
	Serial.println("Start.");
}

void loop() {
	if (false) {
		Airmar100WX::readRaw();

		float relSpeed = Airmar100WX::windSpeed;
		float relDir = Airmar100WX::windDirection;

		Airmar100WX::convertToAbsolute(1.0,PI,PI);

		Serial.write(27);       // ESC command
		Serial.print("[2J");    // clear screen command
		Serial.write(27);
		Serial.print("[H");     // cursor to home command	
		Serial.println("=== Airmar 100WX Test ===");
		Serial.print("Rel. Wind Speed: ");Serial.print(relSpeed);Serial.println(" m/s");
		Serial.print("Abs. Wind Speed: ");Serial.print(Airmar100WX::windSpeed);Serial.println(" m/s");
		Serial.print("Rel. Wind Dir:   ");Serial.print(relDir);Serial.println(" deg");
		Serial.print("Abs. Wind Dir:   ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg");
		Serial.print("Pressure:        ");Serial.print(Airmar100WX::pressure);Serial.println(" kPa");
		Serial.print("Air Temp:        ");Serial.print(Airmar100WX::airTemperature);Serial.println(" deg C");
		Serial.println("=========================");

		delay(500);
  }
  if (true) {
  	float speed, course, heading;

  	// Test 1: Zero measured wind speed, check for proper relative wind
  	Airmar100WX::windSpeed = 0.0;
  	Airmar100WX::windDirection = 0.0;

  	speed = 1.0;
  	course = 0.0;
  	heading = 0.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.println("== Test 1 ==");
  	Serial.print("Should be: ");Serial.println("{1.0 m/s, 180 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

		speed = 1.0;
  	course = 90.0;
  	heading = 0.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{1.0 m/s, 270 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	

  	speed = 1.0;
  	course = 180.0;
  	heading = 0.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{1.0 m/s, 0 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	

  	speed = 1.0;
  	course = 270.0;
  	heading = 0.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{1.0 m/s, 90 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	

		speed = 1.0;
  	course = 0.0;
  	heading = 90.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{1.0 m/s, 180 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	  	

  	speed = 1.0;
  	course = 0.0;
  	heading = 180.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{1.0 m/s, 180 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	

  	speed = 1.0;
  	course = 0.0;
  	heading = 270.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{1.0 m/s, 180 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	

  	// Test 2: Wind measured 1.0 m/s @ 90 deg (east)
  	Airmar100WX::windSpeed = 1.0;
  	Airmar100WX::windDirection = 90.0;

		speed = 0.0;
  	course = 0.0;
  	heading = 0.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.println("== Test 1 ==");
  	Serial.print("Should be: ");Serial.println("{1.0 m/s, 90 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

  	speed = 1.0;
  	course = 0.0;
  	heading = 0.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{1.41 m/s, 45 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

		speed = 1.0;
  	course = 90.0;
  	heading = 0.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{2.0 m/s, 90 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	

  	speed = 1.0;
  	course = 180.0;
  	heading = 0.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{1.41 m/s, 135 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	

  	speed = 1.0;
  	course = 270.0;
  	heading = 0.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{0.0 m/s, --- deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	

		speed = 1.0;
  	course = 0.0;
  	heading = 90.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{0.0 m/s, --- deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	  	

  	speed = 1.0;
  	course = 0.0;
  	heading = 180.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{1.41 m/s, 45 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	

  	speed = 1.0;
  	course = 0.0;
  	heading = 270.0;

  	Airmar100WX::convertToAbsolute(speed,course,heading);

  	Serial.print("Should be: ");Serial.println("{2.0 m/s, 0 deg}");
  	Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
  	Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");  	

  	for (;;);
  }
}
