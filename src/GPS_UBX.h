/*
 * uBlox UBX Protocol Reader (runs on Arduino Leonardo, or equivalent)
 *
 * This file was adapted from one found here:
 * https://sites.google.com/site/wayneholder/self-driving-car---part/evaluating-the-3dr-ublox-lea-6-gps
 *
 */
#include <WProgram.h>
 
#define MAX_LENGTH 512

#define  POSLLH_MSG  0x02
#define  SBAS_MSG    0x32
#define  VELNED_MSG  0x12
#define  STATUS_MSG  0x03
#define  SOL_MSG     0x06
#define  DOP_MSG     0x04
#define  DGPS_MSG    0x31
#define  TIMEGPS_MSG 0x20

namespace GPS_UBX {

extern uint32_t time;
extern float longitude;
extern float latitude;
extern float altitude;
extern float groundSpeed;
extern float course;
extern uint8_t fix;

void enableMsg (unsigned char id, boolean enable);

void init();

void read();

// Convert 1e-7 value packed into long into decimal format
void printLatLon (long val);

void printHex (unsigned char val);

void sendCmd (unsigned char len, byte data[]);

}