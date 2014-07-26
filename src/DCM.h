#ifndef DCM_H
#define DCM_H

#include "Arduino.h"

#define Kp_ROLLPITCH 0.015
#define Ki_ROLLPITCH 0.000010
#define Kp_YAW 1.2
#define Ki_YAW 0.00005

namespace DCM {
  // Euler angles
  extern float roll;
  extern float pitch;
  extern float yaw;
  
  void init();
  
  void updateMeasurements(float omegaX,float omegaY,float omegaZ,float accX,float accY,float accZ,float dt);
  
  void normalize();
  
  void driftCorrection(float heading);
  
  void accelerationCorrection();
  
  void convertDCMtoEuler();
};

#endif
