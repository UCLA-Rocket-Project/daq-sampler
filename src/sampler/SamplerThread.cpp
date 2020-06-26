#include "SamplerThread.h"
#include <chrono>
#include <spdlog/spdlog.h>

using namespace std;
using namespace chrono;

int nextChannel(int lastChannel, std::atomic<bool>* enableMap, int totalChannels);

[[noreturn]] void samplerThread(SharedMemory &mem, unique_ptr<ADCs> adc, unique_ptr<Thermocouples> tc) {
	auto lastAdcSample = steady_clock::now();
	uint8_t lastAdcChannel = -1;
	auto lastTCSample = steady_clock::now();
	uint8_t lastTcChannel = -1;

	while(true) {
		auto now = steady_clock::now();
		if(now - lastAdcSample > microseconds(mem.adcTenthsofMsPerSample * 100)) {
			try {
				int chan = nextChannel(lastAdcChannel, mem.adcEnabled, NUM_ADCS);
				lastAdcChannel = chan;
				lastAdcSample = now;
				if(chan > -1) {
					DeviceRegisters &adcReg = mem.adcRegs[chan];

					auto sampleStart = high_resolution_clock::now();
					RawReading rawReading = adc->takeSample(chan);
					auto sampleDur = duration_cast<milliseconds>(high_resolution_clock::now() - sampleStart).count();
					spdlog::info("ADC Sample Duration: {}", sampleDur);

					mem.adcRegs[chan].rawReading = rawReading;
					float reading = adc->convToVolts(rawReading);

					float humanReadable = reading * adcReg.extendedInfo;
					float callib = adcReg.scale * humanReadable + adcReg.offset;
					adcReg.humanReading = callib;
				}
			}
			catch(const exception &e) {
				spdlog::error("Error trying to sample ADC: {}", e.what());
			}
		}
		if(now - lastTCSample > microseconds(mem.tcTenthsOfMsPerSample * 100)) {
			// TODO: copy a version of the ADC code above, need to TC-ify it

		}
	}
}
// gets the next enabled channel past "lastChannel"
// guaranteed bounds: [0, totalChannels)
int nextChannel(int lastChannel, atomic<bool>* enableMap, int totalChannels)  {
	if(lastChannel < -1 || lastChannel >= totalChannels) {
		throw runtime_error("Invalid last channel specified: "+to_string(lastChannel));
	}
	for(int i = 0; i < totalChannels; i++) {
		int index = (lastChannel + i + 1) % totalChannels;
		if(enableMap[index]) {
			return index;
		}
	}
	return -1;
}