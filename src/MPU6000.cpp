#include "MPU6000.h"
#include <SPI.h>

namespace MPU6000 {
  
  // global variables
  volatile uint8_t newdata;
  
  //Sensor variables
  float accelX;
  float accelY;
  float accelZ;
  
  float gyroX;
  float gyroY;
  float gyroZ;
  
  // MPU6000 SPI functions
  uint8_t SPI_read(uint8_t reg)
  {
    uint8_t dump;
    uint8_t return_value;
    uint8_t addr = reg | 0x80; // Set most significant bit
    digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
    dump = SPI.transfer(addr);
    return_value = SPI.transfer(0);
    digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
    return(return_value);
  }
  
  void SPI_write(uint8_t reg, uint8_t data)
  {
    uint8_t dump;
    digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
    dump = SPI.transfer(reg);
    dump = SPI.transfer(data);
    digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
  }
  
  // MPU6000 INTERRUPT ON INT0
  void data_int()
  {
    newdata++;
  }
  
  // MPU6000 Initialization and configuration
  void init()
  {
    // MPU6000 chip select setup
    pinMode(MPU6000_CHIP_SELECT_PIN, OUTPUT);
    digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
    
    // SPI initialization
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16);      // SPI at 1Mhz (on 16Mhz clock)
    delay(10);
    
    // Chip reset
    uint8_t tries;
    for ( tries = 0 ; tries<5; tries++ ) {
	    SPI_write(MPUREG_PWR_MGMT_1, BIT_H_RESET);
	    delay(100);
    
    	// Wake up device and select GyroZ clock (better performance)
    	SPI_write(MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ);
    	delay(5);
    	SPI_write(MPUREG_PWR_MGMT_2, 0x00);        // only used for wake-up in accelerometer only low power mode
        delay(1);
    	
    	uint8_t response = SPI_read(MPUREG_PWR_MGMT_1);
    	if ( response == 0x03 ) {
    		break;
    	}
    }
    if (tries == 5) {
    	Serial.println("Failed to initialize MPU6000.");
    }
    // Disable I2C bus (recommended on datasheet)
    SPI_write(MPUREG_USER_CTRL, BIT_I2C_IF_DIS);
    delay(1);
    // SAMPLE RATE
    //MPU6000_SPI_write(MPUREG_SMPLRT_DIV,0x04);     // Sample rate = 200Hz    Fsample= 1Khz/(4+1) = 200Hz     
    SPI_write(MPUREG_SMPLRT_DIV,19);     // Sample rate = 50Hz    Fsample= 1Khz/(9+1) = 50Hz     
    delay(1);
    // FS & DLPF   FS=500º/s, DLPF = 20Hz (low pass filter)
    SPI_write(MPUREG_CONFIG, BITS_DLPF_CFG_20HZ);  
    delay(1);
    SPI_write(MPUREG_GYRO_CONFIG,BITS_FS_500DPS);  // Gyro scale 500º/s
    delay(1);
    SPI_write(MPUREG_ACCEL_CONFIG,0x08);            // Accel scale 4g (4096LSB/g)
    delay(1);   
    // INT CFG => Interrupt on Data Ready
    SPI_write(MPUREG_INT_ENABLE,BIT_RAW_RDY_EN);         // INT: Raw data ready
    delay(1);
    SPI_write(MPUREG_INT_PIN_CFG,BIT_INT_ANYRD_2CLEAR);  // INT: Clear on any read
    delay(1);
    // Oscillator set
    // MPU6000_SPI_write(MPUREG_PWR_MGMT_1,MPU_CLK_SEL_PLLGYROZ);
    delay(1);
    
    // MPU_INT is connected to INT 6. Enable interrupt on INT6
    attachInterrupt(6,data_int,RISING);
    
    delay(1000);
  }
  
  // Read gyros and accel sensors on MPU6000
  void read()
  {
    uint8_t byte_H;
    uint8_t byte_L;
    
    // Read AccelX
    byte_H = SPI_read(MPUREG_ACCEL_XOUT_H);
    byte_L = SPI_read(MPUREG_ACCEL_XOUT_L);
    accelX = Accel_Scale( (byte_H<<8) | byte_L );
    
    // Read AccelY
    byte_H = SPI_read(MPUREG_ACCEL_YOUT_H);
    byte_L = SPI_read(MPUREG_ACCEL_YOUT_L);
    accelY = Accel_Scale( (byte_H<<8) | byte_L ); 
    // Read AccelZ
    byte_H = SPI_read(MPUREG_ACCEL_ZOUT_H);
    byte_L = SPI_read(MPUREG_ACCEL_ZOUT_L);
    accelZ = Accel_Scale( (byte_H<<8) | byte_L );
    
    // Read Temp
    //byte_H = MPU6000_SPI_read(MPUREG_TEMP_OUT_H);
    //byte_L = MPU6000_SPI_read(MPUREG_TEMP_OUT_L);
    //temp = (byte_H<<8)| byte_L; 
    
    // Read GyroX
    byte_H = SPI_read(MPUREG_GYRO_XOUT_H);
    byte_L = SPI_read(MPUREG_GYRO_XOUT_L);
    gyroX = Gyro_Scale( (byte_H<<8)| byte_L );
    // Read GyroY
    byte_H = SPI_read(MPUREG_GYRO_YOUT_H);
    byte_L = SPI_read(MPUREG_GYRO_YOUT_L);
    gyroY = Gyro_Scale( (byte_H<<8)| byte_L );
    // Read GyroZ
    byte_H = SPI_read(MPUREG_GYRO_ZOUT_H);
    byte_L = SPI_read(MPUREG_GYRO_ZOUT_L);
    gyroZ = Gyro_Scale( (byte_H<<8)| byte_L );
  }
  
}
