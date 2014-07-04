#include <WProgram.h>

struct PowerMonitorStruct {
	float voltage[4];
	float current[4];
};

class PowerMonitor {
	private:
	Stream *stream;

	public:
	PowerMonitorStruct data;
	
	PowerMonitor(Stream *_stream);
	
	void init();
	
	void read();
};
