#include <WProgram.h>
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
    mag_offset[0] = offsetx;
    mag_offset[1] = offsety;
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
    mag_y = ((((int)buff[0]) << 8) | buff[1])*1 + mag_offset[0];    // X axis
    mag_x = ((((int)buff[4]) << 8) | buff[5])*-1 + mag_offset[1];    // Y axis
    mag_z = ((((int)buff[2]) << 8) | buff[3])*1 + mag_offset[2];    // Z axis
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
}
