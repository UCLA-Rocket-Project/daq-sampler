#pragma once
#include <memory>
#include <Devices.h>
#include "../SharedMemory.h"

void samplerThread(SharedMemory &mem, std::unique_ptr<ADC> adc, std::unique_ptr<Thermocouple> tc);