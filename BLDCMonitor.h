#include <WProgram.h>

class BLDCMonitor {
	private:
	Stream *stream;

	public:
	int16_t rpmA;
	int16_t rpmB;
	float currentA;
	float currentB;
	float voltage;
	
	BLDCMonitor(Stream *_stream);
	
	void init();
	
	void read();
};