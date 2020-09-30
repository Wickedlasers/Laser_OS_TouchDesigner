/*
* Copyright 2020 - LaserDock Limited
* Written by Tim Greiser - Prim8 Technology Consulting, LLC
* Contains related libraries from Derivative Inc. & Laser Dock Limited
*/
#include "pch.h"

#include <laserdocklib/LaserdockSample.h>

using namespace std;

CHOP_Buffer::CHOP_Buffer(uint16_t maxSamples) {
	_max = maxSamples;
}

CHOP_Buffer::~CHOP_Buffer() {
}

bool CHOP_Buffer::fillSamples(const OP_CHOPInput* cinput, CHOP_Output* output, LaserdockSample* samples, double scaleX, double scaleY,
	double scaleRed, double scaleGreen, double scaleBlue) {

	int max = cinput->numSamples > _max ? _max : cinput->numSamples;
	//cout << "fillSamples" << _max << cinput->numSamples << output->numSamples << endl;
	for (int iY = 0; iY < cinput->numSamples; iY++) {
		
		float xval = float(cinput->getChannelData(0)[iY] * scaleX);
		float yval = float(cinput->getChannelData(1)[iY] * scaleY);
		float rval = float(cinput->getChannelData(2)[iY] * scaleRed);
		float gval = float(cinput->getChannelData(3)[iY] * scaleGreen);
		float bval = float(cinput->getChannelData(4)[iY] * scaleBlue);

		if (iY < max && iY < cinput->numSamples) {
			if (output->numSamples > iY) {
				output->channels[0][iY] = xval;
				output->channels[1][iY] = yval;
				output->channels[2][iY] = rval;
				output->channels[3][iY] = gval;
				output->channels[4][iY] = bval;
			}

			uint8_t red = GetUInt8(rval);
			samples[iY].x = float_to_laserdock_xy(xval);
			samples[iY].y = float_to_laserdock_xy(yval);
			uint8_t green = 0x00FF & GetUInt8(gval);
			samples[iY].rg = (0x00FF & GetUInt8(rval)) | (green << 8);
			samples[iY].b = 0x00FF & GetUInt8(bval);
		}
	}
	return true;
}