#include "LTC2448.h"
#include <thread>
using namespace std;
using namespace std::chrono;
using namespace mraa;
using namespace LTC2448;

LTC2448IC::LTC2448IC(int spiBus, int csPin, ChannelOracle oracle)
	: spi(spiBus, csPin), oracle(std::move(oracle)) {
	spi.mode(SPI_MODE0);
	spi.frequency(4'000'000); // 4 MHz
	lastSample = high_resolution_clock::from_time_t(0);
}

void LTC2448IC::makeTransaction(int8_t channel, OSR osr, uint8_t *resp) {
	// doing CS33 data lab makes sure that even weird values of channel are tolerated
	bool odd = channel & 1;
	int8_t addr = channel / 2;
	uint8_t msg[4] = { 0 };
	msg[0] = 0b1011'0000 + (odd << 3) + (addr & 0b111);
	msg[1] = osr << 4;
	Result result = spi.transfer(msg, resp, 4);
	if(result != SUCCESS) {
		printError(result);
		throw runtime_error("MRAA Error Raised (err msg is probably above)" + to_string(result));
	}
	lastChannel = channel;
	lastSample = high_resolution_clock::now();
}

//spi mode 0 / microwire
float LTC2448IC::takeSample(int8_t channel) {
	if(channel > 15 || channel < 0) {
		throw runtime_error("Invalid channel specified");
	}
	uint8_t buf[4];
	uint8_t futureChannel = oracle(channel);

	if(channel == lastChannel) {
		// on oracle hit, we only do one transaction
		waitForSampleComplete();
		makeTransaction(futureChannel, OSR64, buf);
	}
	else {
		makeTransaction(channel, OSR64, buf);
		// on oracle miss, we need to perform multiple transactions
		waitForSampleComplete();
		makeTransaction(futureChannel, OSR64, buf);
	}

	// decode the LTC2448 response
	uint8_t msByte = buf[0]; // most significant byte
	uint32_t reading = ((buf[0] & 0x1F) << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3];
	reading = reading >> 5; // bottom 5 bits are garbage
	bool busy = (reading & 0x80) != 0;
	if(busy) {
		throw runtime_error("LTC2448 is still busy! (check timing constraints)");
	}
	bool negative = (reading & 0x20) != 0;

	float volts = (5.0 * reading) / (1 << 24);
	if(negative) {
		volts *= -1;
	}
	return volts;
}

/* check datasheet for timing constraints
 * OSR=64 with TWOX=0 means each sample should take 1.33ms (rounded to 1.5 for safety)
 */
void LTC2448IC::waitForSampleComplete() {
	this_thread::sleep_until(lastSample + microseconds(1500));
}