#include "Atlas_pH.h"

namespace {
 	NewSoftSerial phSerial(PH_SERIAL_RX,PH_SERIAL_TX);
 	uint8_t ph_data[PH_DATA_BUFFER_LENGTH];
 	long timeOfLastCommand;
}

namespace Atlas_pH {
	extern float pH;

	void init() {
		phSerial.begin(38400);
		sendCommand(PH_COMMAND_STOP_ALL_READINGS);
		delay(50);
		sendCommand(PH_COMMAND_STOP_ALL_READINGS);
		delay(50);
		sendCommand(PH_COMMAND_CONTINUOUS_READINGS);
		delay(50);
		sendCommand(PH_COMMAND_CONTINUOUS_READINGS);
		delay(50);
	}
	
	void sendCommand(const char *command) {
		phSerial.print(command);
		phSerial.write('\r');
	}
	
	void sendCommandTemperature( float tempC ) {
		char buffer[6];
		sprintf(buffer,"%2.2f",tempC);
		buffer[6] = '\0';
		if ( tempC < 10 ) {
			buffer[0] = '0';
		}
		phSerial.print(buffer);
		phSerial.write('\r');
	}
	
	void read() {
		uint8_t max_reads = 5;
		if ( millis() - timeOfLastCommand > 10000 ) {
			// Occasionally tell it to reading continuously just in case.
			sendCommand(PH_COMMAND_CONTINUOUS_READINGS);
		}
		while ( max_reads-- != 0 ) {
			if ( phSerial.available() > 10 ) { // Only read if there are enough bytes
				uint8_t index = 0;
				uint8_t lastByte = 0;
				while ( lastByte != PH_DATA_EOL && index < PH_DATA_BUFFER_LENGTH ) {
					ph_data[index] = phSerial.read();
					lastByte = ph_data[index];
					index++;
				}
				if ( index == 6 ) {
					pH = atof(reinterpret_cast<const char*>(ph_data));
				}
			} else {
				break;
			}
		}	
	}
}
