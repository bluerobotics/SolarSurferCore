#include "HMC5883.h"

namespace HMC5883 {
  int16_t mag_offset[3];
  int16_t mag_x;
  int16_t mag_y;
  int16_t mag_z;
  
  float heading;
  
  void init()
  {
    uint8_t aux_byte;
    
    Wire.begin();
    delay(10);
    
    Wire.beginTransmission(COMPASS_ADDRESS);
    Wire.send((uint8_t)ConfigRegA);
    aux_byte = (SampleAveraging_8<<5 | DataOutputRate_75HZ<<2 | NormalOperation);
    Wire.send(aux_byte);
    Wire.endTransmission();
    delay(50);
    
    Wire.beginTransmission(COMPASS_ADDRESS);
    Wire.send((uint8_t)ModeRegister);
    Wire.send((uint8_t)ContinuousConversion);        // Set continuous mode (default to 10Hz)
    Wire.endTransmission();                 // End transmission
    delay(50);
  }
  
  // set mag offsets
  void set_offset(int offsetx, int offsety, int offsetz)
  {
    mag_offset[0] = offsety;
    mag_offset[1] = offsetx;
    mag_offset[2] = offsetz;
  }
  
  // Read Sensor data in chip axis
  void read()
  {
    int i = 0;
    byte buff[6];
    
    Wire.beginTransmission(COMPASS_ADDRESS);
    Wire.send(0x03);        //sends address to read from
    Wire.endTransmission(); //end transmission
    
    //Wire.beginTransmission(COMPASS_ADDRESS);
    Wire.requestFrom(COMPASS_ADDRESS, 6);    // request 6 bytes from device
    while(Wire.available())
    {
      buff[i] = Wire.receive();  // receive one byte
      i++;
    }
    Wire.endTransmission(); //end transmission
    
    if (i==6){  // All bytes received?
			// MSB byte first, then LSB
			mag_y = ((((int)buff[0]) << 8) | buff[1])*1 - mag_offset[0];    // X axis
			mag_x = ((((int)buff[4]) << 8) | buff[5])*-1 - mag_offset[1];    // Y axis
			mag_z = ((((int)buff[2]) << 8) | buff[3])*1 - mag_offset[2];    // Z axis
    }
    
    
  }
  
  void calculate(float roll, float pitch)
  {
    float Head_X;
    float Head_Y;
    float cos_roll;
    float sin_roll;
    float cos_pitch;
    float sin_pitch;
    
    cos_roll = cos(roll);
    sin_roll = sin(roll);
    cos_pitch = cos(pitch);
    sin_pitch = sin(pitch);
    
    // Tilt compensated Magnetic field X component:
    Head_X = mag_x*cos_pitch+mag_y*sin_roll*sin_pitch+mag_z*cos_roll*sin_pitch;
    // Tilt compensated Magnetic field Y component:
    Head_Y = mag_y*cos_roll-mag_z*sin_roll;
    // Magnetic Heading
    heading = atan2(-Head_Y,Head_X);
  }
  
  void applyDeclination(float declination) {
  	heading = heading + radians(declination);
  	if (heading > PI) {
  		heading -= 2.0f*PI;
  	} else if ( heading < -PI ) {
  		heading += 2.0f*PI;
  	}
  }
  
  void calibrateOffsets() {
  	Serial.println("Calibrating in 3...");
  	delay(1000);
  	Serial.println("Calibrating in 2...");
  	delay(1000);
  	Serial.println("Calibrating in 1...");
  	delay(1000);
  	Serial.println("Calibrating now. Move board all over the place.");
  	
  	int16_t minx = 0;
  	int16_t maxx = 0;
  	int16_t miny = 0;
  	int16_t maxy = 0;
  	int16_t minz = 0;
  	int16_t maxz = 0;
  	
  	for ( uint16_t i = 0 ; i < 600 ; i++ ) {
  		read();
  		
  		if ( mag_x < minx ) { minx = mag_x; }
  		if ( mag_y < miny ) { miny = mag_y; }
  		if ( mag_z < minz ) { minz = mag_z; }
  		if ( mag_x > maxx ) { maxx = mag_x; }
  		if ( mag_y > maxy ) { maxy = mag_y; }
  		if ( mag_z > maxz ) { maxz = mag_z; }
  		
  		Serial.println(i,DEC);
  		delay(50);
  	}
  	
  	Serial.print("x_offset: ");Serial.println((maxx+minx)/2);
  	Serial.print("y_offset: ");Serial.println((maxy+miny)/2);
  	Serial.print("z_offset: ");Serial.println((maxz+minz)/2);
  	Serial.println("");
  	Serial.println("Starting 30 second delay.");
  	delay(30000);
  }
}


