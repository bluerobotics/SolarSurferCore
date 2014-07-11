#ifndef SATCOM_H
#define SATCOM_H

#include <WProgram.h>

namespace Satcom {
	void init(Stream* _stream);

	void beginTransfer(uint8_t messageId);

	void send(float value);

	void endTransfer();
}

#endif
