#ifndef DCM_H
#define DCM_H

#include <WProgram.h>

#define Kp_ROLLPITCH 0.015
#define Ki_ROLLPITCH 0.000010
#define Kp_YAW 1.2
#define Ki_YAW 0.00005

class DCM {
private:
  float Accel_Vector[3]; //Store the acceleration in a vector
  float Gyro_Vector[3];//Store the gyros rutn rate in a vector
  float Omega_Vector[3]; //Corrected Gyro_Vector data
  float Omega_P[3];//Omega Proportional correction
  float Omega_I[3];//Omega Integrator
  float Omega[3];
  
  float errorRollPitch[3]; 
  float errorYaw[3];
  float errorCourse; 
  float COGX; //Course overground X axis
  float COGY; //Course overground Y axis
  
  float DCM_Matrix[3][3]; 
  float Update_Matrix[3][3]; //Gyros here
  float Temporary_Matrix[3][3];
  
public:
  
  // Euler angles
  float roll;
  float pitch;
  float yaw;
  
  void init();
  
  void updateMeasurements(float omegaX,float omegaY,float omegaZ,float accX,float accY,float accZ,float dt);
  
  void normalize();
  
  void driftCorrection(float heading);
  
  void accelerationCorrection();
  
  void convertDCMtoEuler();
};

#endif
