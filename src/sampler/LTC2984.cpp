#include "LTC2984.h"
#include <spdlog/spdlog.h>

using namespace std;
using namespace mraa;

// TODO: implementations stubbed out
RawReading LTC2984::takeSample(uint8_t channel) {
	if(channel > 10) {
		throw runtime_error("Invalid channel specified");
	}

}
float LTC2984::convToCelsius(RawReading rawReading) {
	return 0;
}
void LTC2984::setTCType(uint8_t channel, TC_TYPE tcType) {

}