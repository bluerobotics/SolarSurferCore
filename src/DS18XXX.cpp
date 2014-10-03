#include "OneWire.h"
#include "DallasTemperature.h"
#include "DS18XXX.h"

namespace {
	static const uint8_t TEMP_SENSOR_PIN = A0;

	OneWire oneWire(TEMP_SENSOR_PIN);
	DallasTemperature ts(&oneWire);
}

namespace DS18XXX {
	float temperature;

	void init() {
		ts.begin();
	}

	void read() {
		ts.requestTemperatures();

		temperature = ts.getTempCByIndex(0);
	}
}