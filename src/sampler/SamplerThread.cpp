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
			int chan = nextChannel(lastAdcChannel, mem.adcEnabled, NUM_ADCS);
			lastAdcChannel = chan;
			lastAdcSample = now;
			if(chan > -1) {
				DeviceRegisters &adcReg = mem.adcRegs[chan];
				try {
					/* do some benchmarks on the time to sample.
					 * on oracle hit should be 0
					 * on oracle miss should be ~2ms
					 */
					auto sampleStart = high_resolution_clock::now();
					float volts = adc->takeSample(chan);
					auto sampleDur = duration_cast<milliseconds>(high_resolution_clock::now() - sampleStart).count();
					spdlog::info("ADC Sample Duration: {} ms", sampleDur);

					mem.adcRegs[chan].uncalibReading = volts;
					float calibrated = adcReg.scale * volts + adcReg.offset;
					adcReg.humanReading = calibrated;

					if(mem.logging) {
						mem.log.logNewValue(ADC, chan, calibrated, volts);
					}
				}
				catch (const exception &e) {
					spdlog::error("Error trying to sample ADC: {}", e.what());
					adcReg.uncalibReading = nanf("");
				}
			}
		}
		if(now - lastTCSample > microseconds(mem.tcTenthsOfMsPerSample * 100)) {
			// TODO: copy a version of the ADC code above, need to TC-ify it

		}
	}
}
// gets the next enabled channel past "lastChannel"
// guaranteed bounds: [-1, totalChannels)
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