/** This files provides to read the Atlas Scientific pH sensor connected to an 
 * Arduino. A 3D Robotics APM 2.6 is the primary target but it can be adapted to just 
 * about anything. 
 * 
 * This code was based on the code provided on Atlas Scientific's website. 
 */

#include "Arduino.h"
#include <NewSoftSerial.h>
 
#define PH_SERIAL_RX 2
#define PH_SERIAL_TX 3

#define PH_DATA_BUFFER_LENGTH 20
#define PH_DATA_EOL '\r'

#define PH_COMMAND_ENABLE_LEDS "L1"
#define PH_COMMAND_DISABLE_LEDS "L0"
#define PH_COMMAND_TAKE_ONE_READING "R"
#define PH_COMMAND_CONTINUOUS_READINGS "C"
#define PH_COMMAND_STOP_ALL_READINGS "E"
#define PH_COMMAND_CALIBRATE_AT_SEVEN "S"
#define PH_COMMAND_CALIBRATE_AT_FOUR "F"
#define PH_COMMAND_CALIBRATE_AT_TEN "T"
#define PH_COMMAND_FACTORY_DEFAULTS "X"
#define PH_COMMAND_INFORMATION "I"
#define PH_COMMAND_SET_DEVICE_ID "#"
#define PH_COMMAND_RESET_DEVICE_ID "#!"
#define PH_COMMAND_QUERY_DEVICE_ID "#?"
#define PH_COMMAND_AUTO_BAUD_RATE "Z0"
#define PH_COMMAND_BAUD_RATE_300 "Z1"
#define PH_COMMAND_BAUD_RATE_1200 "Z2"
#define PH_COMMAND_BAUD_RATE_2400 "Z3"
#define PH_COMMAND_BAUD_RATE_9600 "Z4"
#define PH_COMMAND_BAUD_RATE_19200 "Z5"
#define PH_COMMAND_BAUD_RATE_38400 "Z6"
#define PH_COMMAND_BAUD_RATE_57600 "Z7"
#define PH_COMMAND_BAUD_RATE_115200 "Z8"
 
namespace Atlas_pH {
	float pH;

	void init();
	
	void sendCommand(const char *command);
	
	void sendCommandTemperature( float tempC );
	
	void read();
}
 
 
