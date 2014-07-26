#include "DCM.h"
#include "matrix.h"
#include "Vector.h"

namespace {
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
}

namespace DCM {

float roll;
float pitch;
float yaw;

void init() {
  errorCourse = 180;
  COGX = 0;
  COGY = 1;
  
  DCM_Matrix[0][0] = 1; DCM_Matrix[0][1] = 0; DCM_Matrix[0][2] = 0;
  DCM_Matrix[1][0] = 0; DCM_Matrix[1][1] = 1; DCM_Matrix[1][2] = 0;
  DCM_Matrix[2][0] = 0; DCM_Matrix[2][1] = 0; DCM_Matrix[2][2] = 1;
  
  Update_Matrix[0][0] = 0; Update_Matrix[0][1] = 1; Update_Matrix[0][2] = 2;
  Update_Matrix[1][0] = 3; Update_Matrix[1][1] = 4; Update_Matrix[1][2] = 5;    
  Update_Matrix[2][0] = 6; Update_Matrix[2][1] = 7; Update_Matrix[2][2] = 8;
}

void updateMeasurements(float omegaX,float omegaY,float omegaZ,float accX,float accY,float accZ,float dt) {
  Gyro_Vector[0]=omegaX; //gyro x roll
  Gyro_Vector[1]=omegaY; //gyro y pitch
  Gyro_Vector[2]=omegaZ; //gyro Z yaw
  
  Accel_Vector[0]=accX; // acc x
  Accel_Vector[1]=accY; // acc y
  Accel_Vector[2]=accZ; // acc z
  
  Vector_Add(&Omega[0], &Gyro_Vector[0], &Omega_I[0]);  //adding proportional term
  Vector_Add(&Omega_Vector[0], &Omega[0], &Omega_P[0]); //adding Integrator term
  
  accelerationCorrection();    //Remove centrifugal acceleration.
  
  Update_Matrix[0][0]=0;
  Update_Matrix[0][1]=-dt*Omega_Vector[2];//-z
  Update_Matrix[0][2]=dt*Omega_Vector[1];//y
  Update_Matrix[1][0]=dt*Omega_Vector[2];//z
  Update_Matrix[1][1]=0;
  Update_Matrix[1][2]=-dt*Omega_Vector[0];//-x
  Update_Matrix[2][0]=-dt*Omega_Vector[1];//-y
  Update_Matrix[2][1]=dt*Omega_Vector[0];//x
  Update_Matrix[2][2]=0;
  
  Matrix_Multiply(DCM_Matrix,Update_Matrix,Temporary_Matrix); //a*b=c
  
  for(int x=0; x<3; x++) //Matrix Addition (update)
  {
    for(int y=0; y<3; y++)
    {
      DCM_Matrix[x][y]+=Temporary_Matrix[x][y];
    } 
  }
}

void normalize() {
  float error=0;
  float temporary[3][3];
  float renorm=0;
  boolean problem=false;
  
  error= -Vector_Dot_Product(&DCM_Matrix[0][0],&DCM_Matrix[1][0])*.5; //eq.19
  
  Vector_Scale(&temporary[0][0], &DCM_Matrix[1][0], error); //eq.19
  Vector_Scale(&temporary[1][0], &DCM_Matrix[0][0], error); //eq.19
  
  Vector_Add(&temporary[0][0], &temporary[0][0], &DCM_Matrix[0][0]);//eq.19
  Vector_Add(&temporary[1][0], &temporary[1][0], &DCM_Matrix[1][0]);//eq.19
  
  Vector_Cross_Product(&temporary[2][0],&temporary[0][0],&temporary[1][0]); // c= a x b //eq.20
  
  renorm= Vector_Dot_Product(&temporary[0][0],&temporary[0][0]); 
  if (renorm < 1.5625f && renorm > 0.64f) {
    renorm= .5 * (3-renorm);                                                 //eq.21
  } else if (renorm < 100.0f && renorm > 0.01f) {
    renorm= 1. / sqrt(renorm);
  } else {
    problem = true;
  }
  Vector_Scale(&DCM_Matrix[0][0], &temporary[0][0], renorm);
  
  renorm= Vector_Dot_Product(&temporary[1][0],&temporary[1][0]); 
  if (renorm < 1.5625f && renorm > 0.64f) {
    renorm= .5 * (3-renorm);                                                 //eq.21
  } else if (renorm < 100.0f && renorm > 0.01f) {
    renorm= 1. / sqrt(renorm);  
  } else {
    problem = true;
  }
  Vector_Scale(&DCM_Matrix[1][0], &temporary[1][0], renorm);
  
  renorm= Vector_Dot_Product(&temporary[2][0],&temporary[2][0]); 
  if (renorm < 1.5625f && renorm > 0.64f) {
    renorm= .5 * (3-renorm);                                                 //eq.21
  } else if (renorm < 100.0f && renorm > 0.01f) {
    renorm= 1. / sqrt(renorm);   
  } else {
    problem = true;  
  }
  Vector_Scale(&DCM_Matrix[2][0], &temporary[2][0], renorm);
  
  if (problem) {                // Our solution is blowing up and we will force back to initial condition.  Hope we are not upside down!
      DCM_Matrix[0][0]= 1.0f;
      DCM_Matrix[0][1]= 0.0f;
      DCM_Matrix[0][2]= 0.0f;
      DCM_Matrix[1][0]= 0.0f;
      DCM_Matrix[1][1]= 1.0f;
      DCM_Matrix[1][2]= 0.0f;
      DCM_Matrix[2][0]= 0.0f;
      DCM_Matrix[2][1]= 0.0f;
      DCM_Matrix[2][2]= 1.0f;
      problem = false;  
  }
}

void driftCorrection(float heading) {
  //Compensation the Roll, Pitch and Yaw drift. 
  float mag_heading_x;
  float mag_heading_y;
  static float Scaled_Omega_P[3];
  static float Scaled_Omega_I[3];
  float Accel_magnitude;
  float Accel_weight;
  float Integrator_magnitude;
  float tempfloat;
  
  //*****Roll and Pitch***************
  
  // Calculate the magnitude of the accelerometer vector
  Accel_magnitude = sqrt(Accel_Vector[0]*Accel_Vector[0] + Accel_Vector[1]*Accel_Vector[1] + Accel_Vector[2]*Accel_Vector[2]);
  Accel_magnitude = Accel_magnitude/9.81; // Scale to gravity.
  // Dynamic weighting of accelerometer info (reliability filter)
  // Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
  Accel_weight = constrain(1 - 2*abs(1 - Accel_magnitude),0,1);  //  
  
  Vector_Cross_Product(&errorRollPitch[0],&Accel_Vector[0],&DCM_Matrix[2][0]); //adjust the ground of reference
  Vector_Scale(&Omega_P[0],&errorRollPitch[0],Kp_ROLLPITCH*Accel_weight);
  
  Vector_Scale(&Scaled_Omega_I[0],&errorRollPitch[0],Ki_ROLLPITCH*Accel_weight);
  Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);     
  
  //*****YAW***************
  
  //#if USE_MAGNETOMETER==1 
  // We make the gyro YAW drift correction based on compass magnetic heading
  errorCourse=(DCM_Matrix[0][0]*sin(heading)) - (DCM_Matrix[1][0]*cos(heading));  //Calculating YAW error
  
  Vector_Scale(errorYaw,&DCM_Matrix[2][0],errorCourse); //Applys the yaw correction to the XYZ rotation of the aircraft, depeding the position.
  
  Vector_Scale(&Scaled_Omega_P[0],&errorYaw[0],Kp_YAW);
  Vector_Add(Omega_P,Omega_P,Scaled_Omega_P);//Adding  Proportional.
  
  Vector_Scale(&Scaled_Omega_I[0],&errorYaw[0],Ki_YAW);
  Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);//adding integrator to the Omega_I   
  //   #else  // Use GPS Ground course to correct yaw gyro drift
  //   if(GPS.ground_speed>=SPEEDFILT*100)		// Ground speed from GPS is in m/s
  //   {
    //     COGX = cos(radians(GPS.ground_course/100.0));
  //     COGY = sin(radians(GPS.ground_course/100.0));
  //     errorCourse=(DCM_Matrix[0][0]*COGY) - (DCM_Matrix[1][0]*COGX);  //Calculating YAW error
  //     Vector_Scale(errorYaw,&DCM_Matrix[2][0],errorCourse); //Applys the yaw correction to the XYZ rotation of the aircraft, depeding the position.
  //   
  //     Vector_Scale(&Scaled_Omega_P[0],&errorYaw[0],Kp_YAW);
  //     Vector_Add(Omega_P,Omega_P,Scaled_Omega_P);//Adding  Proportional.
  //   
  //     Vector_Scale(&Scaled_Omega_I[0],&errorYaw[0],Ki_YAW);
  //     Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);//adding integrator to the Omega_I   
  //   }
  //#endif
  //  Here we will place a limit on the integrator so that the integrator cannot ever exceed half the saturation limit of the gyros
  Integrator_magnitude = sqrt(Vector_Dot_Product(Omega_I,Omega_I));
  if (Integrator_magnitude > radians(300)) {
    Vector_Scale(Omega_I,Omega_I,0.5f*radians(300)/Integrator_magnitude);
  }
}

void accelerationCorrection() {
  #ifdef TEMPORARY
  Accel_Vector[1] += (GPS.ground_speed/100)*Omega[2];  // Centrifugal force on Acc_y = GPS ground speed (m/s) * GyroZ
  Accel_Vector[2] -= (GPS.ground_speed/100)*Omega[1];  // Centrifugal force on Acc_z = GPS ground speed (m/s) * GyroY 
  #endif
}

void convertDCMtoEuler() {
  pitch = -asin(DCM_Matrix[2][0]);
  roll = atan2(DCM_Matrix[2][1],DCM_Matrix[2][2]);
  yaw = atan2(DCM_Matrix[1][0],DCM_Matrix[0][0]);
}

}