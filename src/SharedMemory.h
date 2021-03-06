#pragma once
#include <atomic>
#include "Logfile.h"

const int NUM_ADCS = 16;
const int NUM_TCS = 10;

// all fields are atomic since they'll be written 2 threads at once
// uint32_t are 2-word wide fields, and uint16_t are 1-word wide fields

struct DeviceRegisters {
public:
	/** The uncalibrated reading returned by an associated device */
	std::atomic<float> uncalibReading;
	/** The human readable version of the reading after calibration is applied */
	std::atomic<float> humanReading;
	/** The 4-char ID of the device. Must be stored internally in byte order, so
	 * 	*((uint8_t*)&id) returns the first character
	 *  **/
	std::atomic<uint32_t> id;
	/** The scale to apply for calibration in hundredths */
	std::atomic<float> scale;
	/** The offset to apply for callibration (which is signed) */
	std::atomic<float> offset;
};
// we aint got time for hipster message passing
class SharedMemory {
public:
	/** Controls whether to log data to file or not */
	std::atomic<bool> logging;
	/** Array of whether a specific ADC channel is sampled */
	std::atomic<bool> adcEnabled[NUM_ADCS];
	/** Array of whether a specific TC should be sampled */
	std::atomic<bool> tcEnabled[NUM_ADCS];
	/** Tenths of a milllisecond between samples for ADC channels */
	std::atomic<uint16_t> adcTenthsofMsPerSample;
	/** Tenths of a millisecond between samples for TC channels */
	std::atomic<uint16_t> tcTenthsOfMsPerSample;
	/** Registers associated with each ADC channel */
	DeviceRegisters adcRegs[NUM_ADCS];
	/** Registers associated with each TC channel */
	DeviceRegisters tcRegs[NUM_TCS];
	/** The shared log file handle */
	LogFile log;
};