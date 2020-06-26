#pragma once
#include <cstdint>

/** A typedef reminder not to use raw readings from the ICs directly */
typedef uint32_t RawReading;

/** Abstract representation of a multi-channel 0-5V ADC */
class ADCs {
public:
	/**
	 * Returns the raw reading from an ADC channel. Throws an exception if the channel is invalid
	 * @param channel the channel to read from
	 * @return the raw 32 bit response from the ADC (may not even be a true number)
	 */
	virtual RawReading takeSample(uint8_t channel) = 0;
	/**
	 * Converts a raw reading into tenths of millivolts. Throw an exception if the raw reading indicates one
	 * @param rawReading the raw 32 bit response from the ADC
	 * @return the tenths of a millivolt represented by the reading
	 */
	virtual float convToVolts(RawReading rawReading) = 0;
};

enum TC_TYPE {
	B,
	E,
	J,
	K,
	N,
	S,
	R,
	T
};

/** Abstract representation of a multi-channel thermocouple IC */
class Thermocouples {
public:
	/**
	 * Returns the raw reading from a thermocouple channel. Throws an exception if the channel is invalid
	 * @param channel the channel to read from
	 * @return the raw 32 bit response from the thermocouple (may not even be a true number)
	 */
	virtual RawReading takeSample(uint8_t channel, TC_TYPE tcType) = 0;
	/**
	 * Converts a raw reading into tenths of a degree Celsius. Throw an exception if the raw reading indicates one
	 * @param rawReading the raw 32 bit response from the thermocouple sampler
	 * @return the tenths of a Celsius represented by the reading
	 */
	virtual float convToCelsius(RawReading rawReading) = 0;
};