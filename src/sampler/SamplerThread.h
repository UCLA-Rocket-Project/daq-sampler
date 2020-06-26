#pragma once
#include <Devices.h>
#include <atomic>
#include <memory>
#include "../SharedMemory.h"

[[noreturn]] void samplerThread(SharedMemory &mem, std::unique_ptr<ADCs> adc, std::unique_ptr<Thermocouples> tc);