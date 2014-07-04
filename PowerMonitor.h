#include <WProgram.h>

class PowerMonitor {
	private:
	Stream *stream;

	public:
	float voltage[4];
	float current[4];
	
	PowerMonitor(Stream *_stream);
	
	void init();
	
	void read();
};