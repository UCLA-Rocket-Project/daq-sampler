#include "SamplerThread.h"
#include <spdlog/spdlog.h>
using namespace std;
void samplerThread(SharedMemory &mem, std::unique_ptr<ADC> adc, std::unique_ptr<Thermocouple> tc) {
	spdlog::info("YO");
}

