/* This file tests the data tranfer between for the SolarSurfer and the BLDC monitor 
* and the Power Monitor. It is currently written to run on an Atmega328p for testing 
* purposes. */

#include <WProgram.h>
#include <NewSoftSerial.h>
#include "../../Airmar100WX.h"

NewSoftSerial nss(6,5);

void setup() {
	Serial.begin(115200);
	nss.begin(4800);
	
	Airmar100WX::init(&nss);
	
	Serial.println("Start.");
}

void loop() {
  if (true) {
    if (Airmar100WX::readRaw()) {
      Airmar100WX::convertToAbsolute(1.0,45.0,45.0);
    }
    Serial.write(27);       // ESC command
    Serial.print("[2J");    // clear screen command
    Serial.write(27);
    Serial.print("[H");     // cursor to home command	
    Serial.println("=== Airmar 100WX Test ===");
    Serial.print("Rel. Wind Speed: ");Serial.print(Airmar100WX::apparentWindSpeed);Serial.println(" m/s");
    Serial.print("Abs. Wind Speed: ");Serial.print(Airmar100WX::windSpeed);Serial.println(" m/s");
    Serial.print("Rel. Wind Dir:   ");Serial.print(Airmar100WX::apparentWindDirection);Serial.println(" deg");
    Serial.print("Abs. Wind Dir:   ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg");
    Serial.print("Pressure:        ");Serial.print(Airmar100WX::pressure);Serial.println(" kPa");
    Serial.print("Air Temp:        ");Serial.print(Airmar100WX::airTemperature);Serial.println(" deg C");
    Serial.println("=========================");

    delay(500);
  }
  if (false) {
  	float speed, course, heading;

    // Test 0: Zero movement, check wind angle
    Airmar100WX::apparentWindSpeed = 1.0;
    Airmar100WX::apparentWindDirection = 0.0;

    speed = 0.0;
    course = 0.0;
    heading = 0.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.println("== Test 0 ==");
    Serial.print("Should be: ");Serial.println("{1.0 m/s, 0.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    speed = 0.0;
    course = 0.0;
    heading = 45.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.print("Should be: ");Serial.println("{1.0 m/s, 45.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    speed = 0.0;
    course = 0.0;
    heading = 90.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.print("Should be: ");Serial.println("{1.0 m/s, 90.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    speed = 0.0;
    course = 0.0;
    heading = -135.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.print("Should be: ");Serial.println("{1.0 m/s, -135.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    // Test 1: Zero movement, check wind angle
    Airmar100WX::apparentWindSpeed = 1.0;
    Airmar100WX::apparentWindDirection = 0.0;

    speed = 0.0;
    course = 0.0;
    heading = 0.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.println("== Test 1 ==");
    Serial.print("Should be: ");Serial.println("{1.0 m/s, 0.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    Airmar100WX::apparentWindSpeed = 1.0;
    Airmar100WX::apparentWindDirection = 45.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.print("Should be: ");Serial.println("{1.0 m/s, 45.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    Airmar100WX::apparentWindSpeed = 1.0;
    Airmar100WX::apparentWindDirection = 90.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.print("Should be: ");Serial.println("{1.0 m/s, 90.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    // Test 2: Zero movement, heading plus wind direction
    Airmar100WX::apparentWindSpeed = 1.0;
    Airmar100WX::apparentWindDirection = 0.0;

    speed = 0.0;
    course = 0.0;
    heading = 0.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.println("== Test 2 ==");
    Serial.print("Should be: ");Serial.println("{1.0 m/s, 0.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    Airmar100WX::apparentWindSpeed = 1.0;
    Airmar100WX::apparentWindDirection = 45.0;

    speed = 0.0;
    course = 0.0;
    heading = 45.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.print("Should be: ");Serial.println("{1.0 m/s, 90.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    Airmar100WX::apparentWindSpeed = 1.0;
    Airmar100WX::apparentWindDirection = -45.0;

    speed = 0.0;
    course = 0.0;
    heading = 90.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.print("Should be: ");Serial.println("{1.0 m/s, 45.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");    

    // Test 3: Movement with no apparent speed
    Airmar100WX::apparentWindSpeed = 0;
    Airmar100WX::apparentWindDirection = 0;

    speed = 1.0;
    course = 0.0;
    heading = 0.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.println("== Test 3 ==");
    Serial.print("Should be: ");Serial.println("{1.0 m/s, 0.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    speed = 1.0;
    course = 45.0;
    heading = 45.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.print("Should be: ");Serial.println("{1.0 m/s, 45.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    speed = 1.0;
    course = 90.0;
    heading = 90.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.print("Should be: ");Serial.println("{1.0 m/s, 90.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");

    // Test 3: Movement and wind
    Airmar100WX::apparentWindSpeed = sqrt(2.0);
    Airmar100WX::apparentWindDirection = -135;

    speed = 1.0;
    course = 0.0;
    heading = 0.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.println("== Test 3 ==");
    Serial.print("Should be: ");Serial.println("{1.0 m/s, -90.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");    

    Airmar100WX::apparentWindSpeed = sqrt(2.0);
    Airmar100WX::apparentWindDirection = 45;

    speed = 1.0;
    course = -90.0;
    heading = 90.0;

    Airmar100WX::convertToAbsolute(speed,course,heading);

    Serial.print("Should be: ");Serial.println("{1.0 m/s, 180.0 deg}");
    Serial.print("Is:        {");Serial.print(Airmar100WX::windSpeed);
    Serial.print(" m/s, ");Serial.print(Airmar100WX::windDirection);Serial.println(" deg}");    

  	for (;;);
  }
}
