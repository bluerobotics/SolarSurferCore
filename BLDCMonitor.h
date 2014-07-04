#include <WProgram.h>

struct BLDCMonitorStruct {
	int16_t rpmA;
	int16_t rpmB;
	float currentA;
	float currentB;
	float voltage;
};

class BLDCMonitor {
	private:
	Stream *stream;

	public:
	BLDCMonitorStruct data;
	
	BLDCMonitor(Stream *_stream);
	
	void init();
	
	void read();
};
