#pragma once
#include <Devices.h>
#include <mraa.hpp>
#include <memory>

class LTC2984: public Thermocouples {
public:
	LTC2984(std::shared_ptr<mraa::Spi> spi): spi(std::move(spi)) {};
	RawReading takeSample(uint8_t channel) override;
	float convToCelsius(RawReading rawReading) override;
	void setTCType(uint8_t channel, TC_TYPE tcType) override;
private:
	std::shared_ptr<mraa::Spi> spi;
};