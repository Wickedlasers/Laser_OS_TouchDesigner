#pragma once

#include "pch.h"

#include <laserdocklib/LaserdockSample.h>

class CHOP_Buffer {
public:
	CHOP_Buffer(uint16_t maxSamples);
	~CHOP_Buffer();

	bool fillSamples(const OP_CHOPInput* cinput, CHOP_Output* output, LaserdockSample* samples, double scaleX, double scaleY, double scaleRed, double scaleGreen, double scaleBlue);
	uint8_t GetUInt8(float f) const;

private:
	uint16_t _max;
};

inline uint8_t CHOP_Buffer::GetUInt8(float f) const {
	if (f < 0) f = 0;
	if (f > 1) f = 1;
	return static_cast<uint8_t>(f * 255);
}

