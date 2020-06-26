#include "LTC2448.h"
#include <spdlog/spdlog.h>
using namespace std;
using namespace mraa;

//spi mode 0 / microwire
RawReading LTC2448::takeSample(uint8_t channel) {
	if(channel > 15) {
		throw runtime_error("Invalid channel specified");
	}
	bool odd = (channel % 2) == 1;
	uint8_t addr = channel / 2;
	uint8_t msg[2];
	msg[0] = 0b10110000;
	msg[0] |= (odd << 3);
	msg[0] |= (addr & 0b111);
	msg[1] = 0b00010000;
	RawReading response;
	Result result = spi->transfer(msg, (uint8_t*)&response, 2);
	if(result != SUCCESS) {
		throw runtime_error(string("MRAA Error Raised: ") + to_string(result));
	}
	return response;
}

float LTC2448::convToVolts(RawReading rawReading) {
	// data is clocked in in big endian format, we need to change to lil endian
	uint8_t msByte = rawReading >> 24; // most signifigant byte
	uint32_t reading = (msByte & 0x1F) | // move byte 3 to byte 0
	          ((rawReading<<8) & 0xFF0000) | // move byte 1 to byte 2
	          ((rawReading>>8) & 0xFF00) | // move byte 2 to byte 1
	          ((rawReading<<24) & 0xFF000000); // byte 0 to byte 3

	bool busy = !!(msByte & 0x8F); // TODO: should we throw on busy signal?
	if(busy) {
		spdlog::warn("Reading taken while ADC busy");
	}
	bool sign = !!(msByte & 0x2F);

	float volts = ((1.0 * reading) / (1 << 28)) * 5.0; // = reading * 5 Volts / 2^28 codes
	if(sign) {
		volts *= -1;
	}
	return volts;
}