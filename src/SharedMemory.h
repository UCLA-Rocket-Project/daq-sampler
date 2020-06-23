#pragma once
#include <atomic>

// we aint got time for hipster message passing
class SharedMemory {
	std::atomic<bool> iAmDank;
};