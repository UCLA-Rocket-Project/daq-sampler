#pragma once
#include <Devices.h>
#include <mraa.hpp>
#include <memory>

class LTC2448: public ADCs {
public:
	LTC2448(std::shared_ptr<mraa::Spi> spi): spi(std::move(spi)) {};
	RawReading takeSample(uint8_t channel) override;
	float convToVolts(RawReading rawReading) override;
private:
	std::shared_ptr<mraa::Spi> spi;
};

