#pragma once
#include <Devices.h>
#include <mraa.hpp>
#include <memory>
#include <functional>
#include <chrono>

namespace LTC2448 {
	/** A function that can be called with the current channel to predict the next channel */
	typedef std::function<uint8_t(uint8_t)> ChannelOracle;

	/** Represents the possible values of the over sample rate (it's some weird code I just made a look up table) */
	enum OSR {
		OSR64 = 0b0000,
		OSR128 = 0b0001,
		OSR256 = 0b0010,
		OSR512 = 0b0011,
		OSR1024 = 0b0101,
		OSR2048 = 0b0110,
		OSR4096 = 0b0111,
		UNDEFINED
	};


	/** A class to represent the LTC2448 ADC <https://www.analog.com/media/en/technical-documentation/data-sheets/2444589fc.pdf>.
	 * NOTE: the LTC2448 is weird in that you get the return of command 1 when you send command 2. The oracle allows us to try
	 * and cheat this restriction by trying to predict the next channel assignment, so we only do 1 conversion per sample.
	 *
	 * On oracle miss, we'll have to do 2 transactions per sample (one to get the command into the chip, and another
	 * to clock out its response).
	 */
	class LTC2448IC : public ADCs {
	public:
		LTC2448IC(int spiBus, int csPin,
		        ChannelOracle oracle = [](uint8_t i) { return 0; });

		virtual float takeSample(int8_t channel);

	private:
		mraa::Spi spi;
		ChannelOracle oracle;
		int8_t lastChannel = -1;
		std::chrono::high_resolution_clock::time_point lastSample;
		void makeTransaction(int8_t channel, OSR osr, uint8_t *resp);
		void waitForSampleComplete();
 	};

};