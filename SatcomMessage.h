#ifndef SATCOMMESSAGE_H
#define SATCOMMESSAGE_H

#include <WProgram.h>

namespace SatcomMessage {
	void init(Stream* _stream);

	void beginTransfer(uint8_t messageId);

	void send(float value);

	void endTransfer();
}

#endif
