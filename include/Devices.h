#pragma once
#include <cstdint>

/** Abstract representation of a multi-channel 0-5V ADC */
class ADCs {
public:
	/**
	 * Returns the voltage of a channel, or throws an exception if this failed
	 * @param channel the channel to read from
	 * @return the voltage of the sampled channel
	 */
	virtual float takeSample(int8_t channel) = 0;
};

enum class TC_TYPE {
	B,
	E,
	J,
	K,
	N,
	S,
	R,
	T,
	IDK // definitely don't use this one
};

/** Abstract representation of a multi-channel thermocouple IC */
class Thermocouples {
public:
	/**
	 * Returns the temperature in Celsius of a channel, or throws an exception if this is impossible
	 * @param channel the channel to read from
	 * @param tcType the type of thermocouple to read as
	 * @return the temperature of the sampled channel
	 */
	virtual float takeSample(int8_t channel, TC_TYPE tcType) = 0;
};