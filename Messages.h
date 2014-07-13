#ifndef MESSAGES_H
#define MESSAGES_H

#include <WProgram.h>

namespace MessageType {

struct Status {
	uint8_t type;
	float latitude;
	float longitude;
	uint32_t time;
};

}

#endif